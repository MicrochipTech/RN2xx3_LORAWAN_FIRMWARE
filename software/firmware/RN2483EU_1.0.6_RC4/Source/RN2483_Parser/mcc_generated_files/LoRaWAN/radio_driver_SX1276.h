/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    radio_driver_SX1276.h

  @Summary:
    This is the Radio Driver SX1276 header file which contains LoRa-specific
    Radio Driver functions declarations and defines for SX1276

  @Description:
    This header file provides LoRa-specific implementations for Radio Driver for SX1276.
    Copyright (c) 2013 - 2016 released Microchip Technology Inc.  All rights reserved.

    Microchip licenses to you the right to use, modify, copy and distribute
    Software only when embedded on a Microchip microcontroller or digital signal
    controller that is integrated into your product or third party product
    (pursuant to the sublicense terms in the accompanying license agreement).

    You should refer to the license agreement accompanying this Software for
    additional information regarding your rights and obligations.

    SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
    EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
    MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
    IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
    CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
    OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
    INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
    CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
    SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
    (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *************************************************************************
 *                           radio_driver_SX1276.h
 *
 * Radio Driver SX1276 header file
 *
 ******************************************************************************/

#ifndef RADIO_DRIVER_H
#define	RADIO_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#include "lorawan_defs.h"


typedef enum
{
    MODULATION_FSK = 0,
    MODULATION_LORA,
} RadioModulation_t;

typedef enum
{
    CR_4_5 = 1,
    CR_4_6,
    CR_4_7,
    CR_4_8,
} RadioErrorCodingRate_t;

typedef enum
{
    SF_7 = 7,
    SF_8,
    SF_9,
    SF_10,
    SF_11,
    SF_12,
} RadioDataRate_t;

typedef enum
{
    BW_125KHZ = 7,
    BW_250KHZ,
    BW_500KHZ,
} RadioLoRaBandWidth_t;

typedef enum
{
    FSK_SHAPING_NONE            = 0,
    FSK_SHAPING_GAUSS_BT_1_0,
    FSK_SHAPING_GAUSS_BT_0_5,
    FSK_SHAPING_GAUSS_BT_0_3,
} RadioFSKShaping_t;

typedef enum
{
    FSKBW_250_0KHZ              = 1,
    FSKBW_125_0KHZ,
    FSKBW_62_5KHZ,
    FSKBW_31_3KHZ,
    FSKBW_15_6KHZ,
    FSKBW_7_8KHZ,
    FSKBW_3_9KHZ,
    FSKBW_INVALID_PADDING_0,
    FSKBW_200_0KHZ,
    FSKBW_100_0KHZ,
    FSKBW_50_0KHZ,
    FSKBW_25_0KHZ,
    FSKBW_12_5KHZ,
    FSKBW_6_3KHZ,
    FSKBW_3_1KHZ,
    FSKBW_INVALID_PADDING_1,
    FSKBW_166_7KHZ,
    FSKBW_83_3KHZ,
    FSKBW_41_7KHZ,
    FSKBW_20_8KHZ,
    FSKBW_10_4KHZ,
    FSKBW_5_2KHZ,
    FSKBW_2_6KHZ,
} RadioFSKBandWidth_t;

// All errors should be negative
typedef enum
{
    ERR_NO_DATA = -32767,
    ERR_DATA_SIZE,
    ERR_BUFFER_LOCKED,
    ERR_RADIO_BUSY,
    ERR_OUT_OF_RANGE,
    ERR_NONE = 0,
} RadioError_t;

#define TIME_ON_AIR_LOAD_VALUE              ((uint32_t)20000)
#define WATCHDOG_DEFAULT_TIME               ((uint32_t)15000)

#define RADIO_FLAG_TRANSMITTING         BIT0
#define RADIO_FLAG_RECEIVING            BIT1
#define RADIO_FLAG_RXDATA               BIT2
#define RADIO_FLAG_RXERROR              BIT3
#define RADIO_FLAG_TIMEOUT              BIT4


void RADIO_Init(uint8_t *radioBuffer, uint32_t frequency);
void RADIO_SetLoRaSyncWord(uint8_t syncWord);
uint8_t RADIO_GetLoRaSyncWord(void);

RadioError_t RADIO_SetChannelFrequency(uint32_t frequency);
uint32_t RADIO_GetChannelFrequency(void);

void RADIO_SetChannelFrequencyDeviation(uint32_t frequencyDeviation);
uint32_t RADIO_GetChannelFrequencyDeviation(void);

void RADIO_SetPreambleLen(uint16_t preambleLen);
uint16_t RADIO_GetPreambleLen(void);

void RADIO_SetOutputPower(int8_t power);
uint8_t RADIO_GetOutputPower(void);

void RADIO_SetCRC(uint8_t crc);
uint8_t RADIO_GetCRC(void);

void RADIO_SetIQInverted(uint8_t iqInverted);
uint8_t RADIO_GetIQInverted(void);

void RADIO_SetBandwidth(RadioLoRaBandWidth_t bandwidth);
RadioLoRaBandWidth_t RADIO_GetBandwidth(void);

void RADIO_SetPABoost(uint8_t paBoost);
uint8_t RADIO_GetPABoost(void);

void RADIO_SetModulation(RadioModulation_t modulation);
RadioModulation_t RADIO_GetModulation(void);

void RADIO_SetFrequencyHopPeriod(uint16_t frequencyHopPeriod);
uint8_t RADIO_GetFrequencyHopPeriod(void);
void RADIO_SetFHSSChangeCallback(uint32_t (*fhssNextFrequency)(void));


void RADIO_SetErrorCodingRate(RadioErrorCodingRate_t errorCodingRate);
RadioErrorCodingRate_t RADIO_GetErrorCodingRate(void);
void RADIO_SetWatchdogTimeout(uint32_t timeout);
uint32_t RADIO_GetWatchdogTimeout(void);
void RADIO_SetFSKBitRate(uint32_t bitRate);
uint32_t RADIO_GetFSKBitRate(void);
void RADIO_SetFSKDataShaping(RadioFSKShaping_t fskDataShaping);
RadioFSKShaping_t RADIO_GetFSKDataShaping(void);
void RADIO_SetFSKRxBw(RadioFSKBandWidth_t bw);
RadioFSKBandWidth_t RADIO_GetFSKRxBw(void);
void RADIO_SetFSKAFCBw(RadioFSKBandWidth_t bw);
RadioFSKBandWidth_t RADIO_GetFSKAFCBw(void);
void RADIO_SetFSKSyncWord(uint8_t syncWordLen, uint8_t* syncWord);
uint8_t RADIO_GetFSKSyncWord(uint8_t* syncWord);


RadioError_t RADIO_Transmit(uint8_t *buffer, uint8_t bufferLen);
RadioError_t RADIO_TransmitCW(void);
RadioError_t RADIO_StopCW(void);
RadioError_t RADIO_ReceiveStart(uint16_t rxWindowSize);
void RADIO_ReceiveStop(void);

int8_t RADIO_GetMaxPower(void);

void RADIO_DIO0(void);
void RADIO_DIO1(void);
void RADIO_DIO2(void);
void RADIO_DIO3(void);
void RADIO_DIO4(void);
void RADIO_DIO5(void);

uint8_t RADIO_GetStatus(void);

RadioError_t RADIO_GetData(uint8_t *data, uint16_t *dataLen);
void RADIO_ReleaseData(void);

void RADIO_SetSpreadingFactor(RadioDataRate_t spreadingFactor);
RadioDataRate_t RADIO_GetSpreadingFactor(void);

uint16_t RADIO_ReadRandom(void);
int8_t RADIO_GetPacketSnr(void);
int16_t RADIO_GetPacketRSSI(void);

void RADIO_RegisterWrite(uint8_t reg, uint8_t value);
uint8_t RADIO_RegisterRead(uint8_t reg);


#ifdef	__cplusplus
}
#endif

#endif	/* RADIO_DRIVER_H */

/**
 End of File
*/