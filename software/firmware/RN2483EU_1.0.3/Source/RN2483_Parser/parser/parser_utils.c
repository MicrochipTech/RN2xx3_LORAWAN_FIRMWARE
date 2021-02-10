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
 *                           parser_utils.c
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser_utils.h"

// 0 = false;
// 1 = true;
// 2 = invalid parameter
#define OFF_STR_IDX         0U
#define ON_STR_IDX          1U
#define INVALID_STR_IDX     2U

#define FIRST_STR_IDX    0U
#define SECOND_STR_IDX   1U
#define NONE_STR_IDX     2U

static const char* gapParseOnOff[] =
{
    {"off"},
    {"on"}
};

bool Validate_HexValue(void* pValue)
{
    bool flag = true;
    char* character;

    for(character = pValue; *character; character++)
    {
        if(isxdigit(*character) == 0)
        {
            flag = false;
            break;
        }
    }

    return flag;
}

uint8_t Parser_HexAsciiToInt(uint16_t hexAsciiLen, char* pInHexAscii, uint8_t* pOutInt)
{
    uint16_t rxHexAsciiLen = strlen(pInHexAscii);
    uint16_t iCtr = 0;
    uint16_t jCtr = rxHexAsciiLen >> 1;
    uint8_t retValue = 0;
    char tempBuff[3];

    if(rxHexAsciiLen % 2 == 0)
    {
        jCtr --;
    }

    if(hexAsciiLen == rxHexAsciiLen)
    {
        while(rxHexAsciiLen > 0)
        {
            if(rxHexAsciiLen >= 2U)
            {
                tempBuff[iCtr] = *(((char*)pInHexAscii) + (rxHexAsciiLen - 2));
                iCtr ++;
                tempBuff[iCtr] = *(((char*)pInHexAscii) + (rxHexAsciiLen - 1));

                rxHexAsciiLen -= 2U;
            }
            else
            {
                tempBuff[iCtr] = '0';
                iCtr ++;
                tempBuff[iCtr] = *(((char*)pInHexAscii) + (rxHexAsciiLen - 1));

                rxHexAsciiLen --;
            }

            iCtr ++;
            tempBuff[iCtr] = '\0';
            *(pOutInt + jCtr) = xtoi(tempBuff);

            iCtr = 0;
            jCtr --;
        }

        retValue = 1;
    }

    return retValue;
}

void Parser_IntArrayToHexAscii(uint8_t arrayLen, uint8_t* pInArray, uint8_t* pOutHexAscii)
{
    uint8_t iCtr = 0U;

    for(iCtr = 0; iCtr < arrayLen; iCtr ++)
    {
        itoa(&pOutHexAscii[iCtr << 1], pInArray[iCtr], 16);

        if(pInArray[iCtr] <= 15)
        {
            /* Special treatment for figures [0..9]  */
            pOutHexAscii[(iCtr << 1) + 1] = pOutHexAscii[iCtr << 1];
            pOutHexAscii[iCtr << 1] = '0';
        }
    }

    pOutHexAscii[(iCtr << 1)] = '\0';
}

bool Validate_Uint8DecAsciiValue(void* pValue, uint8_t* pDecValue)
{
    bool flag = false;
    char* character;
    uint8_t valueLen = strlen(pValue);
    int16_t value = atoi(pValue);

    // Maximum 3 ascii characters 0-> 255
    if((valueLen <= 3U) && (value >= 0) && (value <= 255))
    {
        flag = true;

        for(character = pValue; *character; character++)
        {
            if(isdigit(*character) == 0)
            {
                flag = false;
                break;
            }
        }
    }

    if(flag)
    {
        *pDecValue = (uint8_t)value;
    }

    return flag;
}

//bool Validate_Uint16DecAsciiValue(void* pValue)
//{
//    bool flag = false;
//    char* character;
//    uint8_t valueLen = strlen(pValue);
//    unsigned long value = strtoul(pValue, NULL, 10);
//
//    // Maximum 5 ascii characters 0-> 65535
//    if((valueLen <= 5U) && (value >= 0) && (value <= 65535))
//    {
//        flag = true;
//
//        for(character = pValue; *character; character++)
//        {
//            if(isdigit(*character) == 0)
//            {
//                flag = false;
//                break;
//            }
//        }
//    }
//
//    return flag;
//}

bool Validate_UintDecAsciiValue(void* pValue, uint8_t DigitsNb, uint32_t maxValue)
{
    bool flag = false;
    uint8_t buf[11];
    unsigned long value = strtoul(pValue, NULL, 10);

    ultoa(buf, value, 10U);
    
    flag = true;
    
    if (0 != strcmp((uint8_t*)pValue, buf))
    {
        flag = false;
    }
    
    if (value > maxValue)
    {
        flag = false;
    }

    return flag;
}

// Possible return values:
// 0 = false;
// 1 = true;
// 2 = invalid parameter
uint8_t Validate_OnOffAsciiValue(void* pValue)
{
    uint8_t result = INVALID_STR_IDX;

    if(0U == stricmp(pValue, gapParseOnOff[OFF_STR_IDX]))
    {
        result = OFF_STR_IDX;
    }
    else if (0U == stricmp(pValue, gapParseOnOff[ON_STR_IDX]))
    {
        result = ON_STR_IDX;
    }

    return result;
}

// Possible return values:
// 0 = pValue equal to pStr1;
// 1 = pValue equal to pStr2;
// 2 = pValue not equal to pStr1 or pStr1
uint8_t Validate_Str1Str2AsciiValue(void* pValue, void* pStr1, void* pStr2)
{
    uint8_t result = NONE_STR_IDX;

    if(0U == stricmp(pValue, pStr1))
    {
        result = FIRST_STR_IDX;
    }
    else if (0U == stricmp(pValue, pStr2))
    {
        result = SECOND_STR_IDX;
    }

    return result;
}

int8_t Pin_Index(char* pinName)
{
    int8_t pinNumber = -1;
    
    if(0 == strncmp(pinName, "GPIO", 4))
    {
        if(Validate_UintDecAsciiValue(&pinName[4], 2, 13))
        {
            pinNumber = atoi(&pinName[4]);
        }
    }
    else if (0 == strcmp(pinName, "UART_RTS"))
    {
        pinNumber = 14;
    }
    else if (0 == strcmp(pinName, "UART_CTS"))
    {
        pinNumber = 15;
    }
    else if (0 == strcmp(pinName, "TEST0"))
    {
        pinNumber = 16;
    }
    else if (0 == strcmp(pinName, "TEST1"))
    {
        pinNumber = 17;
    }
    
    return pinNumber;
}