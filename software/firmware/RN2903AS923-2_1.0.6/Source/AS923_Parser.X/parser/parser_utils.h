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
 *                           parser_utils.h
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029 		   2016.09.30	
 ******************************************************************************/

#ifndef _PARSER_UTILS_H
#define _PARSER_UTILS_H


#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define PARSER_DEF_WORD_MAX_LEN      106U
#define PARSER_DEF_CMD_MAX_LEN      550U
#define PARSER_DEF_CMD_MAX_IDX      7U
#define PARSER_DEF_CMD_REPLY_LEN    550U
#define PARSER_DEF_DISPATCH_LEN     3U

bool Validate_HexValue(void* pValue);

uint8_t Parser_HexAsciiToInt(uint16_t hexAsciiLen, char* pInHexAscii, uint8_t* pOutInt);

void Parser_IntArrayToHexAscii(uint8_t arrayLen, uint8_t* pInArray, uint8_t* pOutHexAscii);
bool Validate_Uint8DecAsciiValue(void* pValue, uint8_t* pDecValue);
bool Validate_UintDecAsciiValue(void* pValue, uint8_t DigitsNb, uint32_t maxValue);
uint8_t Validate_OnOffAsciiValue(void* pValue);
uint8_t Validate_Str1Str2AsciiValue(void* pValue, void* pStr1, void* pStr2);
int8_t Pin_Index(char* pinName);

#endif	/* _PARSER_UTILS_H */

