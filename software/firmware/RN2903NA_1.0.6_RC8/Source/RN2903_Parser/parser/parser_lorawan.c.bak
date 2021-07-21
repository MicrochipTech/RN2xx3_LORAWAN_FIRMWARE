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
 *                           parser_lorawan.c
 *
 * LoRaWAN command parser
 *
 *
 * Hardware:
 *  USB Carrier with RN2903 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#include <ctype.h>
#include "parser.h"
#include "parser_private.h"
#include "parser_tsp.h"
#include "parser_utils.h"
#include "../mcc_generated_files/LoRaWAN/lorawan.h"
#include "../mcc_generated_files/memory.h"
#include "../mcc_generated_files/LoRaWAN/lorawan_private.h"
#include "../system/system.h"
#include "../mcc_generated_files/LORAWAN/lorawan_na.h"

#define JOIN_DENY_STR_IDX   0U
#define JOIN_ACCEPT_STR_IDX 1U
#define OTAA_STR_IDX        0U
#define ABP_STR_IDX         1U
#define CNF_STR_IDX         0U
#define UNCNF_STR_IDX       1U
#define RADIO_TX_OK_STR_IDX 0U
#define MAC_TX_OK_STR_IDX   1U
#define RADIO_RX_DATA_STR_IDX 2U
#define MAC_RX_DATA_STR_IDX 3U
#define RADIO_ERR_STR_IDX   4U
#define MAC_ERR_STR_IDX     5U
#define MAC_INVALID_LEN_STR_IDX     6U
#define MAC_REJOIN_STR_IDX  7U

#define OFF_STR_IDX         0U
#define ON_STR_IDX          1U
#define PARSER_LORA_MAC_IFC 0U

parserConfiguredJoinParameters_t gParserConfiguredJoinParameters;

static void ParserRxData(uint8_t* pData, uint8_t dataLength, OpStatus_t status);
static void ParserRxJoinResponse(bool status);

static const char* gapParseJoinMode[] =
{
    {"otaa"},
    {"abp"}
};

static const char* gapParseJoinStatus[] =
{
    {"denied"},
    {"accepted"}
};

static const char* gapParserSendMode[] =
{
    {"cnf"},
    {"uncnf"}
};

static const char* gapParserRxStatus[] =
{
    {"radio_tx_ok"},
    {"mac_tx_ok"},
    {"radio_rx "},
    {"mac_rx "},
    {"radio_err"},
    {"mac_err"},
    {"invalid_data_len"},
    {"mac_rejoin"},
};

static const char* gapParseOnOff[] =
{
    {"off"},
    {"on"}
};

static const char* gapParserLorawanStatus[] =
{
    {"ok"},
    {"not_joined"},
    {"busy"},
    {"invalid_param"},
    {"keys_not_init"},
    {"silent"},
    {"fram_counter_err_rejoin_needed"},
    {"invalid_data_len"},
    {"mac_paused"},
    {"no_free_ch"},
    {"busy" PARSER_END_LINE_DELIM_STRING "mac_cmd_reply"},
    {"invalid_class"},
    {"multicast_keys_not_set"},  
    {"multicast_msg_err"},
};

uint8_t Parser_GetConfiguredJoinParameters()
{
    return gParserConfiguredJoinParameters.value;
}

void Parser_SetConfiguredJoinParameters(uint8_t val)
{
    gParserConfiguredJoinParameters.value = val;
}

void Parser_LorawanInit(void)
{
    LORAWAN_Init(ParserRxData, ParserRxJoinResponse);
    gParserConfiguredJoinParameters.value = 0x00;
}

void Parser_LoraReset (parserCmdInfo_t* pParserCmdInfo)
{
    gParserConfiguredJoinParameters.value = 0x00;
    LORAWAN_Reset();
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
}

void Parser_LoraJoin (parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t validationVal;

    //Parameter validation
    validationVal = Validate_Str1Str2AsciiValue(pParserCmdInfo->pParam1, gapParseJoinMode[OTAA_STR_IDX], gapParseJoinMode[ABP_STR_IDX]);

    if(validationVal < 2U)
    {
        status = LORAWAN_Join(validationVal);
    }

    // A status is returned immediately after the command is executed.
    // An asynchronous reply with the status will be displayed once the join process is finished.
    // While the anwers is expected other commands may be received
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraSend (parserCmdInfo_t* pParserCmdInfo)
{    
    uint8_t portValue;
    uint16_t asciiDataLen = strlen(pParserCmdInfo->pParam3);
    uint16_t  dataLen = asciiDataLen >> 1;
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t validationVal;

    validationVal = Validate_Str1Str2AsciiValue(pParserCmdInfo->pParam1, gapParserSendMode[UNCNF_STR_IDX], gapParserSendMode[CNF_STR_IDX]);

    // Parameter validation
    // MacSendIfc function expects a buffer length of max. 255 bytes. Check dataLen (uint16_t) to be less than 255 in order to avoid overflow 
    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam2, &portValue) && (dataLen <= 255) &&
       (validationVal < 2U) && Validate_HexValue(pParserCmdInfo->pParam3))
    {
        Parser_HexAsciiToInt(asciiDataLen, pParserCmdInfo->pParam3, aParserData);

        if(asciiDataLen % 2 == 1)
        {
            //Odd number of characters, an extra '0' character was added to the payload
            dataLen += 1;
        }

        status = LORAWAN_Send(validationVal, portValue, aParserData, (uint8_t)dataLen);
    }

    // A status is returned immediately after the command is executed.
    // An asynchronous reply with the possible data and tx status will be displayed once the tx process is finished.
    // While the anwers is expected other commands may be received
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
    
    //If mac command response payload is pending
    //send the response frame with 0 bytes of data
    if((status == INVALID_BUFFER_LENGTH) && (loRa.macAnswerPending))
    {
        LORAWAN_Send(0, 0, 0, 0);
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[MAC_CMD_SENT];
    }
}

void Parser_LoraSetCrtDataRate (parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t datarate;

    // Parameter validation
    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &datarate))
    {
        status = LORAWAN_SetCurrentDataRate(datarate);
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetCrtDataRate (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t crtDatarate;

    crtDatarate = LORAWAN_GetCurrentDataRate();

    itoa(aParserData, crtDatarate, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetAdr (parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t validationVal;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam1);
    if(validationVal < 2U)
    {
        LORAWAN_SetAdr(validationVal);
        status = OK;
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetAdr (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t adrEnabled;

    adrEnabled = (uint8_t)LORAWAN_GetAdr();

    pParserCmdInfo->pReplyCmd = (char*)gapParseOnOff[adrEnabled];
}

void Parser_LoraSetDevAddr(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t tempBuff[4];
    int8_t statusIdx = INVALID_PARAMETER;
    uint32_t devAddr;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(sizeof(devAddr) << 1, pParserCmdInfo->pParam1, tempBuff))
        {
            devAddr = (uint32_t)tempBuff[3];
            devAddr += ((uint32_t)tempBuff[2]) << 8;
            devAddr += ((uint32_t)tempBuff[1]) << 16;
            devAddr += ((uint32_t)tempBuff[0]) << 24;

            LORAWAN_SetDeviceAddress (devAddr);
            gParserConfiguredJoinParameters.flags.devaddr = 1;

            statusIdx = OK;
       }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraGetDevAddr(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t devAddr;
    uint8_t tempBuff[4];

    devAddr = LORAWAN_GetDeviceAddress();

    tempBuff[3] = (uint8_t)devAddr;
    tempBuff[2] = (uint8_t)(devAddr >> 8);
    tempBuff[1] = (uint8_t)(devAddr >> 16);
    tempBuff[0] = (uint8_t)(devAddr >> 24);

    Parser_IntArrayToHexAscii(4, tempBuff, aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetDevEui(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(16, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetDeviceEui (aParserData);
            gParserConfiguredJoinParameters.flags.deveui = 1;
            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraGetDevEui(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t aDevEui[8];

    LORAWAN_GetDeviceEui (aDevEui);
	Parser_IntArrayToHexAscii(8, aDevEui, aParserData);
	pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetDevEuiArray(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;
    uint8_t aux;
    uint8_t i;
    
    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(16, pParserCmdInfo->pParam1, aParserData))
        {
            for(i = 0; i < 4; i++)
            {
                aux = aParserData[i];
                aParserData[i] = aParserData[7 - i];
                aParserData[7 - i] = aux;
            }
            
            LORAWAN_SetDeviceEui (aParserData);
            gParserConfiguredJoinParameters.flags.deveui = 1;
            statusIdx = OK;
        }
    }
    
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraGetDevEuiArray(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t aDevEui[8];
    uint8_t i;
    uint8_t aux;
    
    LORAWAN_GetDeviceEui (aDevEui);
    
    for (i = 0; i < 4; i++)
    {
        aux = aDevEui[i];
        aDevEui[i] = aDevEui[7 - i];
        aDevEui[7 - i] = aux;
    }
    
    Parser_IntArrayToHexAscii(8, aDevEui, aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetAppEui(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(16, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetApplicationEui(aParserData);
            gParserConfiguredJoinParameters.flags.appeui = 1;
            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}


void Parser_LoraGetAppEui(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t aAppEui[8];

    LORAWAN_GetApplicationEui (aAppEui);
	Parser_IntArrayToHexAscii(8, aAppEui, aParserData);
	pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetAppEuiArray(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;
    uint8_t aux;
    uint8_t i;
    
    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(16, pParserCmdInfo->pParam1, aParserData))
        {
            for(i = 0; i < 4; i++)
            {
                aux = aParserData[i];
                aParserData[i] = aParserData[7 - i];
                aParserData[7 - i] = aux;
            }
            
            LORAWAN_SetApplicationEui(aParserData);
            gParserConfiguredJoinParameters.flags.appeui = 1;
            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraGetAppEuiArray(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t aAppEui[8];
    uint8_t i;
    uint8_t aux;

    LORAWAN_GetApplicationEui (aAppEui);
    
    for (i = 0; i < 4; i++)
    {
        aux = aAppEui[i];
        aAppEui[i] = aAppEui[7 - i];
        aAppEui[7 - i] = aux;
    }
    
    Parser_IntArrayToHexAscii(8, aAppEui, aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetNwkSKey(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(32U, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetNetworkSessionKey(aParserData);
            gParserConfiguredJoinParameters.flags.nwkskey = 1;

            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraSetAppKey(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(32U, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetApplicationKey(aParserData);
            gParserConfiguredJoinParameters.flags.appkey = 1;

            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraSetAppMultiSKey(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(32U, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetMcastApplicationSessionKey(aParserData);
            gParserConfiguredJoinParameters.flags.appmultiskey = 1;

            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}


void Parser_LoraGetChannelFreq(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t freq;
    uint8_t channelId;

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[INVALID_PARAMETER];


    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &channelId) &&
      (channelId < 72))
    {
        freq = LORAWAN_GetFrequency(channelId);

        ultoa(aParserData, freq, 10U);
        pParserCmdInfo->pReplyCmd = aParserData;
    }
}

void Parser_LoraSetChannelStatus (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t channelId;
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t validationVal;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam2);

    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &channelId) &&(validationVal < 2U))
    {
        status = LORAWAN_SetChannelIdStatus(channelId, validationVal);
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetChannelStatus (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t chStatus;
    uint8_t channelId;

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[INVALID_PARAMETER];

    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &channelId) &&
      (channelId < 72))
    {
        chStatus = LORAWAN_GetChannelIdStatus(channelId);
        pParserCmdInfo->pReplyCmd = (char*)gapParseOnOff[chStatus];
    }
}

void Parser_LoraSetDatarateRange (parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t channelId;
    uint8_t minDr;
    uint8_t maxDr;

    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &channelId) &&
       Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam2, &minDr) &&
       Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam3, &maxDr) &&
       minDr < 16 && maxDr < 16)
    {
        status = LORAWAN_SetDataRange(channelId, ((maxDr << 4) | minDr));
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetDatarateRange (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t channelId;
    uint8_t drRange;
    uint8_t minDr;
    uint8_t maxDr;
    uint16_t crtIdx = 0;;

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[INVALID_PARAMETER];

    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &channelId) &&
      (channelId < 72))
    {
        drRange = LORAWAN_GetDataRange(channelId);

        minDr = drRange & 0x0F;
        maxDr = (drRange >> 4) & 0x0F;

        itoa(aParserData, minDr, 10U);
        crtIdx = strlen(aParserData);
        aParserData[crtIdx] = ' ';
        crtIdx ++;
        itoa(&aParserData[crtIdx], maxDr, 10U);
        pParserCmdInfo->pReplyCmd = aParserData;
    }
}

void Parser_LoraSetTxPower (parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t txPowerIdx;

    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &txPowerIdx))
    {
        status = LORAWAN_SetTxPower(txPowerIdx);
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetTxPower (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t txPowerIdx;

    txPowerIdx = LORAWAN_GetTxPower();

    itoa(aParserData, txPowerIdx, 10);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraPause (parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t resumeInterval;

    resumeInterval = LORAWAN_Pause();

    ultoa(aParserData, resumeInterval, 10);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraResume (parserCmdInfo_t* pParserCmdInfo)
{
    LORAWAN_Resume();
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
}

void Parser_LoraSave (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t jCtr;
    uint16_t startIdx = 0;
    uint8_t tempBuff[16];
    auint32_t temp32;
    uint8_t temp;
    uint16_t tempXor = 0;
    auint16_t temp16;

    //Delete checksum as a first step
    DATAEE_WriteByte(MAX_EEPROM_PARAM_INDEX - 1, 0xFF);
    DATAEE_WriteByte(MAX_EEPROM_PARAM_INDEX, 0xFF);

    temp32.value = gParserConfiguredJoinParameters.value;
    tempXor += temp32.buffer[1];
    DATAEE_WriteByte(startIdx ++, temp32.buffer[1]);
    tempXor += temp32.buffer[0];
    DATAEE_WriteByte(startIdx ++, temp32.buffer[0]);
    
    // Save RX window 2 parameters
    LORAWAN_GetReceiveWindow2Parameters(&temp32.value, &temp);
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        tempXor += temp32.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp32.buffer[iCtr]);
    }
    tempXor += temp;
    DATAEE_WriteByte(startIdx ++, temp);
    
    //#######################################################################
    temp = (uint8_t)LORAWAN_GetClass();
    tempXor += temp;
    DATAEE_WriteByte(startIdx ++, temp);
    //########################################################################
    
    // Save uplink counter
    temp32.value = LORAWAN_GetUplinkCounter();
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        tempXor += temp32.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp32.buffer[iCtr]);
    }

    // Save downlink counter
    temp32.value = LORAWAN_GetDownlinkCounter();
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        tempXor += temp32.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp32.buffer[iCtr]);
    }
    
    LORAWAN_GetDeviceEui(tempBuff);

    //EEPROM[0..7] device EUI
    for(iCtr = 0U; iCtr < 8U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx ++, tempBuff[iCtr]);
    }

    LORAWAN_GetApplicationEui(tempBuff);
    //EEPROM[8..15] application EUI
    for(iCtr = 0U; iCtr < 8U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx++, tempBuff[iCtr]);
    }

    LORAWAN_GetApplicationKey(tempBuff);
    //EEPROM[16..31] application key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx ++, tempBuff[iCtr]);
    }    

    LORAWAN_GetNetworkSessionKey(tempBuff);
    //EEPROM[32..47] network session key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx ++, tempBuff[iCtr]);
    }
    
    LORAWAN_GetApplicationSessionKey(tempBuff);
    //EEPROM[48..63] application session key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx ++, tempBuff[iCtr]);
    }
    
    temp32.value = LORAWAN_GetDeviceAddress();
    //EEPROM[64..67] device address
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {                
        tempXor += temp32.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp32.buffer[iCtr]);
    }    

    //#############################################################
    LORAWAN_GetMcastApplicationSessionKey(tempBuff);
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx ++, tempBuff[iCtr]);
    }

    LORAWAN_GetMcastNetworkSessionKey(tempBuff);
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempXor += tempBuff[iCtr];
        DATAEE_WriteByte(startIdx ++, tempBuff[iCtr]);
    }

    temp32.value = LORAWAN_GetMcastDeviceAddress();
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {                
        tempXor += temp32.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp32.buffer[iCtr]);
    }

    temp32.value = LORAWAN_GetMcastDownCounter();
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {                
        tempXor += temp32.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp32.buffer[iCtr]);
    }

    temp = (uint8_t)LORAWAN_GetMcast();
    tempXor += temp;
    DATAEE_WriteByte(startIdx ++, temp);
    
    //################################################################
    //US specific parser code - channel list update (status and dataRange)
    //EEPROM[68..211] device address
    for(jCtr = 0U; jCtr < 72U; jCtr ++)
    {
        //Channel status
        temp = LORAWAN_GetChannelIdStatus(jCtr);
        tempXor += temp;
        DATAEE_WriteByte(startIdx ++, temp);

        //Channel DRRange
        temp = LORAWAN_GetDataRange(jCtr);
        tempXor += temp;
        DATAEE_WriteByte(startIdx ++, temp);
    }
    
    //Current data rate 
    temp = LORAWAN_GetCurrentDataRate();
    tempXor += temp;
    DATAEE_WriteByte(startIdx ++, temp);  
    
    //Current ADR state
    temp = LORAWAN_GetAdr();
    tempXor += temp;
    DATAEE_WriteByte(startIdx ++, temp);    

    //Current rx1DROffset
    temp = LORAWAN_GetReceiveOffset();
    tempXor += temp;
    DATAEE_WriteByte(startIdx ++, temp);
    
    //Current rx1Delay
    temp16.value = LORAWAN_GetReceiveDelay1();
    for(iCtr = 0U; iCtr < 2U; iCtr ++)
    {
        tempXor += temp16.buffer[iCtr];
        DATAEE_WriteByte(startIdx ++, temp16.buffer[iCtr]);
    }
    
    if(tempXor == 0xFFFF)
    {
        tempXor = 0;
    }

    //Store the checksum in EEPROM
    DATAEE_WriteByte(startIdx ++, (uint8_t)tempXor);
    DATAEE_WriteByte(startIdx ++, (uint8_t)(tempXor >> 8));

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
}

void Parser_LoraSetRx2WindowParams (parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    unsigned long freq = strtoul(pParserCmdInfo->pParam2, NULL, 10);
    uint8_t datarate;

    // Parameter validation
    // The frequency is 10 ascii characters long
    if(Validate_Uint8DecAsciiValue(pParserCmdInfo->pParam1, &datarate) &&
       Validate_UintDecAsciiValue(pParserCmdInfo->pParam2, 10, UINT32_MAX))
    {
        status = LORAWAN_SetReceiveWindow2Parameters(freq, datarate);
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetRx2WindowParams(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t freq = 0;
    uint8_t datarate = 0xFF;
    uint8_t iCount;
    uint8_t dataLen;

    //Return value of the function not checked but default values for freq and datarate are invalid
    LORAWAN_GetReceiveWindow2Parameters(&freq, &datarate);
    ultoa(aParserData, datarate, 10U);
    dataLen = strlen(aParserData);
    aParserData[dataLen ++] = ' ';
    ultoa(&aParserData[dataLen], freq, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraForceEnable (parserCmdInfo_t* pParserCmdInfo)
{
    LORAWAN_ForceEnable();
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
}


void Parser_LoraSetUplinkCounter(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t param1Value = (uint32_t)strtoul(pParserCmdInfo->pParam1, NULL, 10U);

    if (Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 10, UINT32_MAX))
    {
        LORAWAN_SetUplinkCounter(param1Value);
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
    }
    else
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[INVALID_PARAMETER];
    }
}

void Parser_LoraSetDownlinkCounter(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t param1Value = (uint32_t)strtoul(pParserCmdInfo->pParam1, NULL, 10U);

    if (Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 10, UINT32_MAX))
    {
        LORAWAN_SetDownlinkCounter(param1Value);
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
    }
    else
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[INVALID_PARAMETER];
    }
}


void Parser_LoraSetSyncWord(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t asciiDataLen = strlen(pParserCmdInfo->pParam1);
    uint8_t status = INVALID_PARAMETER;

    if (Validate_HexValue(pParserCmdInfo->pParam1) && (2 == strlen(pParserCmdInfo->pParam1)))
    {
        Parser_HexAsciiToInt(asciiDataLen, pParserCmdInfo->pParam1, aParserData);

        LORAWAN_SetSyncWord(aParserData[0]);
        status = OK;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetSyncWord(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t syncWord;

    syncWord = LORAWAN_GetSyncWord();
    Parser_IntArrayToHexAscii(1, &syncWord, aParserData);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraGetUplinkCounter(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t ctr;

    ctr = LORAWAN_GetUplinkCounter();
    ultoa(aParserData, ctr, 10U);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraGetDownlinkCounter(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t ctr;

    ctr = LORAWAN_GetDownlinkCounter();
    ultoa(aParserData, ctr, 10U);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraLinkCheck (parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t period = strtoul(pParserCmdInfo->pParam1, NULL, 10);
    LorawanError_t status = INVALID_PARAMETER;

    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 5, UINT16_MAX))
    {
        LORAWAN_LinkCheckConfigure(period);
        status = OK;
    }
    
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetPrescaler (parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t dutycyclePrescaler;

    dutycyclePrescaler = LORAWAN_GetPrescaler();
    utoa(aParserData, dutycyclePrescaler, 10);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraGetMacStatus (parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t macStatusMask;
    uint8_t tempBuff[4];

    macStatusMask = LORAWAN_GetStatus();

    tempBuff[3] = (uint8_t)macStatusMask;
    tempBuff[2] = (uint8_t)(macStatusMask >> 8);
    tempBuff[1] = (uint8_t)(macStatusMask >> 16);
    tempBuff[0] = (uint8_t)(macStatusMask >> 24);    
    Parser_IntArrayToHexAscii(4, tempBuff, aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetBatLevel (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t batLevel = atoi(pParserCmdInfo->pParam1);
    LorawanError_t status = INVALID_PARAMETER;

    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 3, UINT8_MAX))
    {
        LORAWAN_SetBattery(batLevel);
        status = OK;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraSetReTxNb(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t reTxNb = atoi(pParserCmdInfo->pParam1);
    LorawanError_t status = INVALID_PARAMETER;

    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 3, UINT8_MAX))
    {
        LORAWAN_SetNumberOfRetransmissions(reTxNb);
        status = OK;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetReTxNb(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t reTxNb;

    reTxNb = LORAWAN_GetNumberOfRetransmissions();
    utoa(aParserData, reTxNb, 10);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraGetLinkCheckMargin(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t mrgn;

    mrgn = LORAWAN_GetLinkCheckMargin();
    utoa(aParserData, mrgn, 10);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraGetLinkCheckGwCnt(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t gwCnt;

    gwCnt = LORAWAN_GetLinkCheckGwCnt();
    utoa(aParserData, gwCnt, 10);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetAutoReply(parserCmdInfo_t* pParserCmdInfo)
{
    LorawanError_t status = INVALID_PARAMETER;
    uint8_t validationVal;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam1);
    if(validationVal < 2U)
    {
        LORAWAN_SetAutomaticReply(validationVal);
        status = OK;
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetAutoReply(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t arEnabled;

    arEnabled = (uint8_t)LORAWAN_GetAutomaticReply();

    pParserCmdInfo->pReplyCmd = (char*)gapParseOnOff[arEnabled];
}

void Parser_LoraSetRxDelay1(parserCmdInfo_t* pParserCmdInfo)
{
    //Delay1 in ms
    uint16_t rxDelay1 = atoi(pParserCmdInfo->pParam1);
    LorawanError_t status = INVALID_PARAMETER;


    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 5, UINT16_MAX))
    {
        LORAWAN_SetReceiveDelay1(rxDelay1);
        status = OK;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[status];
}

void Parser_LoraGetRxDelay1(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t rxDelay1;

    rxDelay1 = LORAWAN_GetReceiveDelay1();
    utoa(aParserData, rxDelay1, 10);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraGetRxDelay2(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t rxDelay2;

    rxDelay2 = LORAWAN_GetReceiveDelay2();
    utoa(aParserData, rxDelay2, 10);

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetClass(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if ((pParserCmdInfo->pParam1[0] == 'A') || (pParserCmdInfo->pParam1[0] == 'a'))
    {
        LORAWAN_SetClass(CLASS_A);
        statusIdx = OK;
    }
    else if ((pParserCmdInfo->pParam1[0] == 'C') || (pParserCmdInfo->pParam1[0] == 'c'))
    {
        LORAWAN_SetClass(CLASS_C);
        statusIdx = OK;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraGetClass(parserCmdInfo_t* pParserCmdInfo)
{
    switch (LORAWAN_GetClass())
    {
        case CLASS_A:
            aParserData[0] = 'A';
            aParserData[1] = '\0';
            break;
        case CLASS_C:
            aParserData[0] = 'C';
            aParserData[1] = '\0';
            break;
    }

    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetMcast(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t validationVal;
    uint8_t returnVal = INVALID_PARAMETER;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam1);
    if (validationVal < 2U)
    {
        returnVal = LORAWAN_SetMcast(validationVal);       
    }
    
    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[returnVal];
}

void Parser_LoraGetMcast(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t mcastStatus;
    
    mcastStatus = LORAWAN_GetMcast();
    
    pParserCmdInfo->pReplyCmd = (char*)gapParseOnOff[mcastStatus];
}

void Parser_LoraSetMcastDownCounter(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t param1Value = (uint32_t)strtoul(pParserCmdInfo->pParam1, NULL, 10U);

    if (Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 10, UINT32_MAX))
    {
        LORAWAN_SetMcastDownCounter(param1Value);
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[OK];
    }
    else
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[INVALID_PARAMETER];
    } 
}

void Parser_LoraGetMcastDownCounter(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t ctr;

    ctr = LORAWAN_GetMcastDownCounter();
    ultoa(aParserData, ctr, 10U);

    pParserCmdInfo->pReplyCmd = aParserData;    
}

void Parser_LoraSetMcastDevAddr(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t tempBuff[4];
    int8_t statusIdx = INVALID_PARAMETER;
    uint32_t devMultiAddr;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(sizeof(devMultiAddr) << 1, pParserCmdInfo->pParam1, tempBuff))
        {
            devMultiAddr = (uint32_t)tempBuff[3];
            devMultiAddr += ((uint32_t)tempBuff[2]) << 8;
            devMultiAddr += ((uint32_t)tempBuff[1]) << 16;
            devMultiAddr += ((uint32_t)tempBuff[0]) << 24;

            LORAWAN_SetMcastDeviceAddress(devMultiAddr);
            gParserConfiguredJoinParameters.flags.devmultiaddr = 1;

            statusIdx = OK;
       }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraGetMcastDevAddr(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t devMultiAddr;
    uint8_t tempBuff[4];

    devMultiAddr = LORAWAN_GetMcastDeviceAddress();

    tempBuff[3] = (uint8_t)devMultiAddr;
    tempBuff[2] = (uint8_t)(devMultiAddr >> 8);
    tempBuff[1] = (uint8_t)(devMultiAddr >> 16);
    tempBuff[0] = (uint8_t)(devMultiAddr >> 24);

    Parser_IntArrayToHexAscii(4, tempBuff, aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_LoraSetNwkMultiSKey(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(32U, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetMcastNetworkSessionKey(aParserData);
            gParserConfiguredJoinParameters.flags.nwkmultiskey = 1;

            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

void Parser_LoraSetAppSKey(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t statusIdx = INVALID_PARAMETER;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        if(Parser_HexAsciiToInt(32U, pParserCmdInfo->pParam1, aParserData))
        {
            LORAWAN_SetApplicationSessionKey(aParserData);
            gParserConfiguredJoinParameters.flags.appskey = 1;

            statusIdx = OK;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserLorawanStatus[statusIdx];
}

static void ParserRxData(uint8_t* pData, uint8_t dataLength, OpStatus_t status)
{
    uint16_t dataLen;
    uint16_t maxDataLenToTx;

    switch(status)
    {
        case MAC_OK:
            //Successful transmission
            if(dataLength > 1U) // a dataLength of 1 is a port number with no data
            {
                // Data received
                strcpy(aParserData, gapParserRxStatus[MAC_RX_DATA_STR_IDX]);
                dataLen = strlen(aParserData);

                //Handle port: the first byte from pData represents the port number
                itoa(&aParserData[dataLen], *pData, 10);
                dataLen = strlen(aParserData);

                aParserData[dataLen] = ' ';
                dataLen ++;

                // Skip port number (&pData[1]), process only data
                maxDataLenToTx = ((dataLength - 1) <= ((PARSER_MAX_DATA_LEN - dataLen) >> 1)) ? (dataLength - 1) : ((PARSER_MAX_DATA_LEN - dataLen) >> 1);
                Parser_IntArrayToHexAscii(maxDataLenToTx, &pData[1],  &aParserData[dataLen]);
                Parser_TxAddReply(aParserData, strlen(aParserData));
            }
            else
            {
                // No data received
                Parser_TxAddReply((char*)gapParserRxStatus[MAC_TX_OK_STR_IDX], strlen((char*)gapParserRxStatus[MAC_TX_OK_STR_IDX]));
            }
            break;

        case RADIO_OK:
            if(dataLength > 0U)
            {
                // Data received
                strcpy(aParserData, gapParserRxStatus[RADIO_RX_DATA_STR_IDX]);
                dataLen = strlen(aParserData);

                aParserData[dataLen] = ' ';
                dataLen ++;

                maxDataLenToTx = (dataLength <= ((PARSER_MAX_DATA_LEN - dataLen) >> 1)) ? dataLength : ((PARSER_MAX_DATA_LEN - dataLen) >> 1);
                Parser_IntArrayToHexAscii(maxDataLenToTx, pData,  &aParserData[dataLen]);
                Parser_TxAddReply(aParserData, strlen(aParserData));
            }
            else
            {
                // No data received
                Parser_TxAddReply((char*)gapParserRxStatus[RADIO_TX_OK_STR_IDX], strlen((char*)gapParserRxStatus[RADIO_TX_OK_STR_IDX]));
            }
            break;

        case RADIO_NOT_OK:
            // Failed transmission
            Parser_TxAddReply((char*)gapParserRxStatus[RADIO_ERR_STR_IDX], strlen((char*)gapParserRxStatus[RADIO_ERR_STR_IDX]));
            break;

        case MAC_NOT_OK:
            // Failed transmission
            Parser_TxAddReply((char*)gapParserRxStatus[MAC_ERR_STR_IDX], strlen((char*)gapParserRxStatus[MAC_ERR_STR_IDX]));
            break;
            
        case INVALID_BUFFER_LEN:
            // TRansmission error based on changing the datarate 
            Parser_TxAddReply((char*)gapParserRxStatus[MAC_INVALID_LEN_STR_IDX], strlen((char*)gapParserRxStatus[MAC_INVALID_LEN_STR_IDX]));
            break;

        case MAC_REJOIN_NEEDED:
            // TRansmission error based on frame counter fatal error 
            Parser_TxAddReply((char*)gapParserRxStatus[MAC_REJOIN_STR_IDX], strlen((char*)gapParserRxStatus[MAC_REJOIN_STR_IDX]));
            break;
            
        default:
            break;
    }
}

static void ParserRxJoinResponse(bool status)
{
    uint8_t statusIdx = JOIN_DENY_STR_IDX;


    //This is called every time the join process is finished
    if(true == status)
    {
        //Sucessful join
        statusIdx = JOIN_ACCEPT_STR_IDX;
    }

    Parser_TxAddReply((char*)gapParseJoinStatus[statusIdx], strlen((char*)gapParseJoinStatus[statusIdx]));
}
