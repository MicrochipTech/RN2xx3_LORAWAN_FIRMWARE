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
 *                           parser_radio.h
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#ifndef _PARSER_RADIO_H
#define _PARSER_RADIO_H


#include "parser_private.h"

void Parser_RadioSetChannelFreq (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetChannelFreq (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetOutputPwr(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetOutputPwr(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetSf(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetSf(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetModulation(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetModulation(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetCrc(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetCrc(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetBw(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetBw(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetIqInverted(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetIqInverted(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetPreambleLen (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetPreambleLen (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioTransmit(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioTransmitCW(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioReceive(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioStopRx(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetFdev (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetFdev (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetCr(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetCr(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetBt(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetBt(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetAfcBw(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetAfcBw(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetRxBw(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetRxBw(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetWdt (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetWdt (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetBitrate (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetBitrate (parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetSnr(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetRSSI(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetSyncWord(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetSyncWord(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioSetReg(parserCmdInfo_t* pParserCmdInfo);
void Parser_RadioGetReg(parserCmdInfo_t* pParserCmdInfo);


#endif /* _PARSER_RADIO_H */