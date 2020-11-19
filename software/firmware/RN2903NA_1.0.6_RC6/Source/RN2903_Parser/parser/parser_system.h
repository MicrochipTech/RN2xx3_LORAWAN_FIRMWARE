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
 *                           parser_system.h
 * Hardware:
 *  USB Carrier with RN2903 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M15723          2014.07.29        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#ifndef _PARSER_SYSTEM_H
#define _PARSER_SYSTEM_H


#include "parser_private.h"

void Parser_ExitFromSleep(void);

void Parser_SystemBootload(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemSleep(parserCmdInfo_t* pParserCmdInfo);
void Parser_SystemReboot(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemFactReset(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemSetNvm(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemGetNvm(parserCmdInfo_t* pParserCmdInfo);
void Parser_SystemGetVer(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemSetPinMode(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemSetPinDig(parserCmdInfo_t* pParserCmdInfo);
void Parser_SystemGetPinDig(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemGetPinAnalog(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemGetBattery(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemGetHwEui(parserCmdInfo_t* pParserCmdInfo);

void Parser_SystemSetABDTime(parserCmdInfo_t* pParserCmdInfo);
void Parser_SystemGetABDTime(parserCmdInfo_t* pParserCmdInfo);


#if defined(DEBUGLINKADR)
void Parser_DebugLinkADR(parserCmdInfo_t* pParserCmdInfo);
#endif //DEBUGLINKADR


#endif /* _PARSER_SYSTEM_H */
