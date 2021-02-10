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
 *                           parser_radio.c
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

#include <stdlib.h>
#include <ctype.h>
#include "parser_radio.h"
#include "parser_utils.h"
#include "parser.h"
#include "../mcc_generated_files/LoRaWAN/radio_interface.h"
#include "../mcc_generated_files/LoRaWAN/radio_driver_SX1276.h"

static const char* gapParserRadioStatus[] =
{
    {"ok"},
    {"invalid_param"},
    {"err"},
    {"busy"}
};

static const char* gaParserRadioSf[] =
{
    {"sf7"},
    {"sf8"},
    {"sf9"},
    {"sf10"},
    {"sf11"},
    {"sf12"},
};

static const char* gapParserRadioModulation[] =
{
    {"fsk"},
    {"lora"},
};

static const char* gapParseRadioOnOff[] =
{
    {"off"},
    {"on"}
};

static const char* gaParserRadioBw[] =
{
    {"125"},
    {"250"},
    {"500"},
};

static const char* gaParserRadioCr[] =
{
    {"4/5"},
    {"4/6"},
    {"4/7"},
    {"4/8"},
};

static const char* gaParserRadioBt[] =
{
    {"none"},
    {"1.0"},
    {"0.5"},
    {"0.3"},
};

static const char* gaParserRadioAfc[] =
{
    {"250"},
    {"125"},
    {"62.5"},
    {"31.3"},
    {"15.6"},
    {"7.8"},
    {"3.9"},
    NULL,
    {"200"},
    {"100"},
    {"50"},
    {"25"},
    {"12.5"},
    {"6.3"},
    {"3.1"},
    NULL,
    {"166.7"},
    {"83.3"},
    {"41.7"},
    {"20.8"},
    {"10.4"},
    {"5.2"},
    {"2.6"},
};

void Parser_RadioSetChannelFreq (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    unsigned long freq = strtoul(pParserCmdInfo->pParam1, NULL, 10);

    // The frequency is 10 ascii characters long
    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 10, UINT32_MAX) &&
       ((freq >= 902000000) && (freq <= 928000000)))
    {
        if(RADIO_SetChannelFrequency(freq) == 0)
        {
            status = OK_STATUS_IDX;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetChannelFreq(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t freq;

    freq = RADIO_GetChannelFrequency();
    ultoa(aParserData, freq, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioSetOutputPwr (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    uint16_t asciiLen = strlen(pParserCmdInfo->pParam1);
    int16_t outputPwr = atoi(pParserCmdInfo->pParam1);
    uint8_t iCtr;
    char* pParam1 = (char*)pParserCmdInfo->pParam1;

    if((asciiLen <=3) && ((outputPwr == 20) || ((outputPwr <= 17) && (outputPwr >= 2))) &&
       (((pParam1[0] == '+') ||(pParam1[0] == '-') && (asciiLen > 1)) ||
       isdigit(pParam1[0])))
    {
        for(iCtr = 1; iCtr < asciiLen; iCtr++)
        {
            if(isdigit(pParam1[iCtr]) == 0)
            {
                break;
            }
        }
        if((iCtr == asciiLen) || (asciiLen == 1))
        {
            RADIO_SetOutputPower(outputPwr);
            status = OK_STATUS_IDX;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetOutputPwr(parserCmdInfo_t* pParserCmdInfo)
{
    int8_t outputPwr;

    outputPwr = RADIO_GetOutputPower();
    itoa(aParserData, outputPwr, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioSetSf(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gaParserRadioSf)/sizeof(gaParserRadioSf[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gaParserRadioSf[iCtr]))
        {
            //SF7 = 7
            RADIO_SetSpreadingFactor(iCtr + 7);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetSf(parserCmdInfo_t* pParserCmdInfo)
{
    //SF7 = 7
    pParserCmdInfo->pReplyCmd = (char*)gaParserRadioSf[RADIO_GetSpreadingFactor() - 7];
}

void Parser_RadioSetModulation(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gapParserRadioModulation)/sizeof(gapParserRadioModulation[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gapParserRadioModulation[iCtr]))
        {
            RADIO_SetModulation(iCtr);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetModulation(parserCmdInfo_t* pParserCmdInfo)
{
    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioModulation[RADIO_GetModulation()];
}

void Parser_RadioSetCrc(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t validationVal;
    uint8_t status = INVALID_PARAM_IDX;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam1);
    if(validationVal < 2U)
    {
        RADIO_SetCRC(validationVal);
        status = OK_STATUS_IDX;
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetCrc(parserCmdInfo_t* pParserCmdInfo)
{
    pParserCmdInfo->pReplyCmd = (char*)gapParseRadioOnOff[RADIO_GetCRC()];
}

void Parser_RadioSetBw(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gaParserRadioBw)/sizeof(gaParserRadioBw[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gaParserRadioBw[iCtr]))
        {
            //125khz = 7
            RADIO_SetBandwidth(iCtr + 7);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetBw(parserCmdInfo_t* pParserCmdInfo)
{
    //125khz = 7
    pParserCmdInfo->pReplyCmd = (char*)gaParserRadioBw[RADIO_GetBandwidth() - 7];
}

void Parser_RadioSetIqInverted(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t validationVal;
    uint8_t status = INVALID_PARAM_IDX;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam1);
    if(validationVal < 2U)
    {
        RADIO_SetIQInverted(validationVal);
        status = OK_STATUS_IDX;
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetIqInverted(parserCmdInfo_t* pParserCmdInfo)
{
    pParserCmdInfo->pReplyCmd = (char*)gapParseRadioOnOff[RADIO_GetIQInverted()];
}

void Parser_RadioSetPreambleLen (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    uint16_t preambleLen = atol(pParserCmdInfo->pParam1);

    // The uint16_t is max 5 ascii characters long
    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 5, UINT16_MAX))
    {
        RADIO_SetPreambleLen(preambleLen);
        status = OK_STATUS_IDX;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetPreambleLen(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t preambleLen;

    preambleLen = RADIO_GetPreambleLen();
    ultoa(aParserData, preambleLen, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}


void Parser_RadioTransmitCW(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t validationVal;
    uint8_t status = INVALID_PARAM_IDX;

    validationVal = Validate_OnOffAsciiValue(pParserCmdInfo->pParam1);
    if (validationVal == 1U)
    {
        RADIO_TransmitCW();
        status = OK_STATUS_IDX;
    }
    else if (validationVal == 0U)
    {
        RADIO_StopCW();
        RESET();
    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioTransmit(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t asciiDataLen = strlen(pParserCmdInfo->pParam1);
    uint8_t status = INVALID_PARAM_IDX;
    RadioError_t radioStatus;
    uint16_t dataLen = asciiDataLen >> 1;

    //Max 255 bytes for tx
    if((asciiDataLen <= 510) &&
       Validate_HexValue(pParserCmdInfo->pParam1))
    {
        Parser_HexAsciiToInt(asciiDataLen, pParserCmdInfo->pParam1, aParserData);
        if(asciiDataLen % 2 == 1)
        {
            //Odd number of characters, an extra '0' character was added to the payload
            dataLen += 1;
        }
        radioStatus = RADIO_Transmit(aParserData, dataLen);
        switch(radioStatus)
        {
            case ERR_NONE:
                status = OK_STATUS_IDX;
                break;

            case ERR_DATA_SIZE:
                //do nothing, status already set to invalid
                break;

            default:
                status = BUSY_STATUS_IDX;
        }
        
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioReceive(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    unsigned long rxTimeout = strtoul(pParserCmdInfo->pParam1, NULL, 10);

    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 5, UINT16_MAX))
    {
        if(RADIO_ReceiveStart(rxTimeout) == 0)
        {
            status = OK_STATUS_IDX;
        }
        else
        {
            status = BUSY_STATUS_IDX;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioSetFdev (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    uint32_t frequencyDeviation = atol(pParserCmdInfo->pParam1);

    // The frequencyDeviation is max 200kHz from SX1276 DS
    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 6, 200000))
    {
        RADIO_SetChannelFrequencyDeviation(frequencyDeviation);
        status = OK_STATUS_IDX;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetFdev(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t frequencyDeviation;

    frequencyDeviation = RADIO_GetChannelFrequencyDeviation();
    ultoa(aParserData, frequencyDeviation, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioSetCr(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gaParserRadioCr)/sizeof(gaParserRadioCr[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gaParserRadioCr[iCtr]))
        {
            //CR_4_5 = 1
            RADIO_SetErrorCodingRate(iCtr + 1);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetCr(parserCmdInfo_t* pParserCmdInfo)
{
    //CR_4_5 = 1
    pParserCmdInfo->pReplyCmd = (char*)gaParserRadioCr[RADIO_GetErrorCodingRate() - 1];
}

void Parser_RadioSetBt(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gaParserRadioBt)/sizeof(gaParserRadioBt[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gaParserRadioBt[iCtr]))
        {
            RADIO_SetFSKDataShaping(iCtr);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetBt(parserCmdInfo_t* pParserCmdInfo)
{
    pParserCmdInfo->pReplyCmd = (char*)gaParserRadioBt[RADIO_GetFSKDataShaping()];
}

void Parser_RadioSetAfcBw(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gaParserRadioAfc)/sizeof(gaParserRadioAfc[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gaParserRadioAfc[iCtr]))
        {
            RADIO_SetFSKAFCBw(iCtr);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetAfcBw(parserCmdInfo_t* pParserCmdInfo)
{
    pParserCmdInfo->pReplyCmd = (char*)gaParserRadioAfc[RADIO_GetFSKAFCBw()];
}

void Parser_RadioSetRxBw(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t iCtr;
    uint8_t finalCtr = sizeof(gaParserRadioAfc)/sizeof(gaParserRadioAfc[0]);
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;

    //TODO: optimize this
    for(iCtr = 0; iCtr < finalCtr; iCtr++ )
    {
        if(0 == stricmp(pParam1, gaParserRadioAfc[iCtr]))
        {
            RADIO_SetFSKRxBw(iCtr);
            status = OK_STATUS_IDX;
            break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetRxBw(parserCmdInfo_t* pParserCmdInfo)
{
    pParserCmdInfo->pReplyCmd = (char*)gaParserRadioAfc[RADIO_GetFSKRxBw()];
}

void Parser_RadioSetWdt (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    unsigned long wdt = strtoul(pParserCmdInfo->pParam1, NULL, 10);

    // The frequency is 10 ascii characters long
    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 10, UINT32_MAX))
    {
        RADIO_SetWatchdogTimeout(wdt);
        status = OK_STATUS_IDX;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetWdt(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t wdt;

    wdt = RADIO_GetWatchdogTimeout();
    ultoa(aParserData, wdt, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioSetBitrate (parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t status = INVALID_PARAM_IDX;
    unsigned long bitrate = strtoul(pParserCmdInfo->pParam1, NULL, 10);

    if(Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 6, 300000) &&
       (bitrate > 0))
    {
        RADIO_SetFSKBitRate(bitrate);
        status = OK_STATUS_IDX;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetBitrate(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t bitrate;

    bitrate = RADIO_GetFSKBitRate();
    ultoa(aParserData, bitrate, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioGetSnr(parserCmdInfo_t* pParserCmdInfo)
{
    int8_t packetSNR;

    packetSNR = RADIO_GetPacketSnr();
    itoa(aParserData, packetSNR, 10U);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioGetSyncWord(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t syncWord[8];
    uint8_t syncWordLen;
    if (MODULATION_FSK == RADIO_GetModulation())
    {
        syncWordLen = RADIO_GetFSKSyncWord(syncWord);
        Parser_IntArrayToHexAscii(syncWordLen, syncWord, aParserData);
    }
    else
    {
        syncWord[0] = RADIO_GetLoRaSyncWord();
        Parser_IntArrayToHexAscii(1, syncWord, aParserData);
    }
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_RadioSetSyncWord(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t asciiDataLen = strlen(pParserCmdInfo->pParam1);
    uint8_t status = INVALID_PARAM_IDX;
    uint8_t mod;
    uint16_t dataLen = asciiDataLen >> 1;

    mod = RADIO_GetModulation();

    //Max 8 bytes for sync word
    //MODULATION_FSK = 0, MODULATION_LORA = 1
    if((((mod == 0) && (asciiDataLen <= 16)) ||
        ((mod == 1) && (asciiDataLen == 2))) &&
       Validate_HexValue(pParserCmdInfo->pParam1))
    {
        Parser_HexAsciiToInt(asciiDataLen, pParserCmdInfo->pParam1, aParserData);

        if(asciiDataLen % 2 == 1)
        {
            //Odd number of characters, an extra '0' character was added to the payload
            dataLen += 1;
        }

        if(mod == 1)
        {
            RADIO_SetLoRaSyncWord(aParserData[0]);
        }
        else
        {
            RADIO_SetFSKSyncWord(dataLen, &aParserData[0]);
        }
        status = OK_STATUS_IDX;
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioSetReg(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t regAddr;
    uint8_t regValue;
    uint8_t status = INVALID_PARAM_IDX;

    if(Validate_HexValue(pParserCmdInfo->pParam1) && Validate_HexValue(pParserCmdInfo->pParam2))
    {
        regAddr = (uint8_t)strtoul(pParserCmdInfo->pParam1, NULL, 16U);
        regValue = (uint8_t)strtoul(pParserCmdInfo->pParam2, NULL, 16U);
        if((strlen(pParserCmdInfo->pParam1) == 2 && (regAddr > 0 || strcmp("00", pParserCmdInfo->pParam1) == 0)) &&
          (strlen(pParserCmdInfo->pParam2) == 2 && (regValue > 0 || strcmp("00", pParserCmdInfo->pParam2) == 0)))
        {
            RADIO_RegisterWrite(regAddr, regValue);
            status = OK_STATUS_IDX;

        }

    }
    pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[status];
}

void Parser_RadioGetReg(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t regAddr;
    uint8_t regValue;

    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        regAddr = (uint8_t)strtoul(pParserCmdInfo->pParam1, NULL, 16U);
        if(strlen(pParserCmdInfo->pParam1) == 2 && (regAddr > 0 || strcmp("00", pParserCmdInfo->pParam1) == 0))
        {
            regValue = RADIO_RegisterRead(regAddr);
            ultoa(aParserData, regValue, 16U);

            pParserCmdInfo->pReplyCmd = aParserData;
        }
        else
        {
            pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[INVALID_PARAM_IDX];
        }
    }
    else
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserRadioStatus[INVALID_PARAM_IDX];
    }
}