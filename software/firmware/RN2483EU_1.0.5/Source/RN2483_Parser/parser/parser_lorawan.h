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
 *                           parser_lorawan.h
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#ifndef _PARSER_LORAWAN_H
#define _PARSER_LORAWAN_H


#include "parser_private.h"

void Parser_LorawanInit(void);

void Parser_LoraReset (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraJoin (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSend (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetCrtDataRate (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetCrtDataRate (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetAdr (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetAdr (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetDevAddr(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetDevAddr(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetDevEui(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetDevEui(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetAppEui(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetAppEui(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetNwkSKey(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetAppSKey(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetAppKey(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetChannelFreq (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetChannelFreq (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetChannelStatus (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetChannelStatus (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetDatarateRange (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetDatarateRange (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetChannelDutyCycle(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetChannelDutyCycle(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetTxPower (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetTxPower (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraPause(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraResume(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSave (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetRx2WindowParams (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetRx2WindowParams (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraForceEnable (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraLinkCheck (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetPrescaler (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetMacStatus (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetBatLevel (parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetReTxNb(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetReTxNb(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetLinkCheckMargin(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetLinkCheckGwCnt(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetAutoReply(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetAutoReply(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetRxDelay1(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetRxDelay1(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetRxDelay2(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetIsm(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetSyncWord(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetSyncWord(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetUplinkCounter(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetDownlinkCounter(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetUplinkCounter(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetDownlinkCounter(parserCmdInfo_t* pParserCmdInfo);

void Parser_LoraSetClass(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetClass(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetMcastDevAddr(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetMcastDevAddr(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetNwkMultiSKey(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetAppMultiSKey(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetMcast(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetMcast(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraSetMcastDownCounter(parserCmdInfo_t* pParserCmdInfo);
void Parser_LoraGetMcastDownCounter(parserCmdInfo_t* pParserCmdInfo);
#endif /* _PARSER_LORAWAN_H */