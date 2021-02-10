/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*
 *************************************************************************
 *
 *                           parser_tsp.c
 *
 * Parser transport source file
 *
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date                Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#include <stdlib.h>
#include "parser_tsp.h"
#include "parser.h"
#include "parser_lorawan.h"
#include "parser_utils.h"
#include "../mcc_generated_files/eusart1.h"
#include "../system/system_low_power.h"

typedef struct parserRxCmd_tag
{
    char cmd[PARSER_DEF_CMD_MAX_LEN];
    uint16_t wordLen[PARSER_DEF_CMD_MAX_IDX];
    uint16_t wordStartPos[PARSER_DEF_CMD_MAX_IDX];
    uint8_t bCmdStatus;
    uint8_t crtWordIdx;
    uint16_t crtCmdPos;
    uint16_t crtWordPos;
}parserRxCmd_t;

volatile parserRxCmd_t mRxParserCmd;
static const char* gpParserLineDelim = {PARSER_END_LINE_DELIM_STRING};

static const char* gapParserTspStatus[] =
{
    {"ok"},
    {"invalid_param"},
    {"err"}
};

void Parser_RxClearBuffer(void)
{
    mRxParserCmd.bCmdStatus = 0;
    mRxParserCmd.crtWordIdx = 0;
    mRxParserCmd.crtCmdPos = 0;
    mRxParserCmd.crtWordPos = 0;
    
    memset(mRxParserCmd.wordLen, 0, PARSER_DEF_CMD_MAX_IDX << 1);
    memset(mRxParserCmd.wordStartPos, 0, PARSER_DEF_CMD_MAX_IDX << 1);
}

void Parser_RxAddChar(uint8_t rxChar)
{
    uint8_t retStatus = STATUS_OK;
    bool bIsEndLine = false;
    uint8_t iCount;
    
    // Process special character: '\b'
    if(rxChar == '\b')
    {
        /* process delete character '\b' */
        if(mRxParserCmd.crtCmdPos > 0U)
        {
            // Check for ' '. This was previously replaced with '\0'
            //mRxParserCmd.crtCmdPos always indicates the first free position
            if(mRxParserCmd.cmd[mRxParserCmd.crtCmdPos - 1] == '\0')
            {
                mRxParserCmd.crtWordIdx --;
                mRxParserCmd.crtWordPos = mRxParserCmd.wordLen[mRxParserCmd.crtWordIdx];
                mRxParserCmd.wordLen[mRxParserCmd.crtWordIdx] = 0U;
                mRxParserCmd.wordStartPos[mRxParserCmd.crtWordIdx] = 0U;
            }
            else
            {
                mRxParserCmd.crtWordPos --;
            }

            mRxParserCmd.crtCmdPos --;
        }

        return;
    }

    // Regular command
    if(mRxParserCmd.crtCmdPos < PARSER_DEF_CMD_MAX_LEN - 1)
    {
        if(rxChar == ' ')
        {
            if(mRxParserCmd.crtWordIdx < PARSER_DEF_CMD_MAX_IDX - 1)
            {
                /* Command separator received */

                /* Replace ' ' with \0 */
                mRxParserCmd.cmd[mRxParserCmd.crtCmdPos ++] = '\0';

                /* Save last filled position in mRxParserCmd.wordLen[mRxParserCmd.crtWordIdx] */
                mRxParserCmd.wordLen[mRxParserCmd.crtWordIdx] = mRxParserCmd.crtWordPos;
                mRxParserCmd.wordStartPos[mRxParserCmd.crtWordIdx] = mRxParserCmd.crtCmdPos - mRxParserCmd.crtWordPos - 1;

                /* Prepare to receive next word */
                mRxParserCmd.crtWordIdx ++;
                mRxParserCmd.crtWordPos = 0;
            }
            else
            {
                retStatus = STATUS_ERROR;
            }
        }
        else
        {
            /* Save the character */

            mRxParserCmd.cmd[mRxParserCmd.crtCmdPos ++] = rxChar;
            mRxParserCmd.crtWordPos ++;

            if(mRxParserCmd.crtCmdPos >= strlen(gpParserLineDelim))
            {
                bIsEndLine = true;

                for(iCount = strlen(gpParserLineDelim); (iCount > 0U) && bIsEndLine; iCount --)
                {
                    if(mRxParserCmd.cmd[mRxParserCmd.crtCmdPos - iCount] != gpParserLineDelim[strlen(gpParserLineDelim) - iCount])
                    {
                        bIsEndLine = false;
                    }
                }
            }
            if(bIsEndLine)
            {
                /* Entire command received */

                /* Replace new line with \0 */
                mRxParserCmd.cmd[mRxParserCmd.crtCmdPos - strlen(gpParserLineDelim)] = '\0';

                mRxParserCmd.wordLen[mRxParserCmd.crtWordIdx] = mRxParserCmd.crtWordPos - strlen(gpParserLineDelim);
                mRxParserCmd.wordStartPos[mRxParserCmd.crtWordIdx] = mRxParserCmd.crtCmdPos - mRxParserCmd.crtWordPos;

                mRxParserCmd.bCmdStatus = 1;
            }
        }
    }
    else
    {
        retStatus = STATUS_ERROR;
    }

    if(STATUS_ERROR == retStatus)
    {
        Parser_RxClearBuffer();
        /* Send reply code */
        Parser_TxAddReply((char*)gapParserTspStatus[ERR_STATUS_IDX], strlen(gapParserTspStatus[ERR_STATUS_IDX]));
    }

}

void Parser_TxAddReply(char* pReplyStr, uint16_t replyStrLen)
{
    uint16_t iCtr;

    //GIE=0;   /* Atomic check */   
    for(iCtr = 0; iCtr < replyStrLen; iCtr ++)
    {
        EUSART1_Write(pReplyStr[iCtr]);
    }
    for(iCtr = 0; iCtr < strlen(gpParserLineDelim); iCtr ++)
    {
        EUSART1_Write(gpParserLineDelim[iCtr]);
    }
    
}
