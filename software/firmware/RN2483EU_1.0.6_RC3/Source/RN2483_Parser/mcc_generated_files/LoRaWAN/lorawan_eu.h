 /********************************************************************
 * Copyright (C) 2016 Microchip Technology Inc. and its subsidiaries
 * (Microchip).  All rights reserved.
 *
 * You are permitted to use the software and its derivatives with Microchip
 * products. See the license agreement accompanying this software, if any, for
 * more info about your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP, SMSC, OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH
 * OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR OTHER SIMILAR COSTS. To the fullest
 * extend allowed by law, Microchip and its licensors liability will not exceed
 * the amount of fees, if any, that you paid directly to Microchip to use this
 * software.
 *************************************************************************
 *
 *                           lorawan.h
 *
 * LoRaWAN EU header file
 *
 *
 * Hardware:
 *  RN-2xx3-PICTAIL
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M17319          2016.03.29        0.1
 ******************************************************************************/

#ifndef _LORAWAN_EU_H
#define	_LORAWAN_EU_H

#ifdef	__cplusplus
extern "C" {
#endif

/****************************** INCLUDES **************************************/ 
    
#include <xc.h>
#include "lorawan_defs.h"

/****************************** DEFINES ***************************************/     


//maximum number of channels
#define MAX_EU_SINGLE_BAND_CHANNELS             16 // 16 channels numbered from 0 to 15

#define ALL_CHANNELS                            1
#define WITHOUT_DEFAULT_CHANNELS                0

//dutycycle definition
#define DUTY_CYCLE_DEFAULT                               302  //0.33 %  
#define DUTY_CYCLE_JOIN_1HOUR                            302  //0.33%
#define DUTY_CYCLE_JOIN_11HOUR                           3029 //0.033%    
#define DUTY_CYCLE_JOIN_REQUEST                          30290 //0.0033%  
#define DUTY_CYCLE_DEFAULT_NEW_CHANNEL                   999  //0.1%

//EU default channels for 868 Mhz
#define LC0_868                   {868100000, 868100000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC1_868                   {868300000, 868300000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC2_868                   {868500000, 868500000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}

//EU default channels for 433 Mhz (the same channels are for join request)
#define LC0_433                   {433175000, 433175000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC1_433                   {433375000, 433375000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}
#define LC2_433                   {433575000, 433575000, ENABLED,  { ( ( DR5 << SHIFT4 ) | DR0 ) },   302, 0, 1, 0xFF}

#define TXPOWER_MIN                                 0
#define TXPOWER_MAX                                 7

#define SIZE_JOIN_ACCEPT_WITH_CFLIST                33
#define NUMBER_CFLIST_FREQUENCIES                   5

// masks for channel parameters
#define FREQUENCY_DEFINED                   0x01
#define DATA_RANGE_DEFINED                  0x02
#define DUTY_CYCLE_DEFINED                  0x04
    
#define EU868_TX_MIN_DATARATE               DR0
#define EU868_TX_MAX_DATARATE               DR7
    
/***************************** TYPEDEFS ***************************************/    

typedef union
{
    uint8_t joinAcceptCounter[29];
    struct
    {
        Mhdr_t mhdr;
        uint8_t appNonce[3];
        uint8_t networkId[3];
        DeviceAddress_t deviceAddress;
        DlSettings_t DLSettings;
        uint8_t rxDelay;
        uint8_t cfList[16];
    } members;
} JoinAccept_t;

//Channel parameters
typedef struct
{
        uint32_t frequency;
        uint32_t downFrequency;
        bool status;
        DataRange_t dataRange;
        uint16_t dutyCycle;
        uint32_t channelTimer;
        bool joinRequestChannel;
        uint8_t parametersDefined;
} ChannelParams_t;

typedef union
{
    uint16_t value;
    struct
    {
        unsigned ackRequiredFromNextDownlinkMessage:1;  //if set, the next downlink message should have the ACK bit set because an ACK is needed for the end device
        unsigned ackRequiredFromNextUplinkMessage:1;    //if set, the next uplink message should have the ACK bit set because an ACK is needed for the server
        unsigned joining: 1;
        unsigned fPending:1;
        unsigned adrAckRequest:1;
        unsigned synchronization:1;                      //if set, there is no need to send an immediate packet because the application sent one from the callback
    };
} LorawanMacStatus_t;

typedef struct
{
    LorawanMacStatus_t lorawanMacStatus;
    LorawanStatus_t macStatus;
    FCnt_t fCntUp;
    FCnt_t fCntDown;
    FCnt_t fMcastCntDown;
    LoRaClass_t deviceClass;
    ReceiveWindowParameters_t receiveWindow1Parameters;
    ReceiveWindowParameters_t receiveWindow2Parameters;
    ActivationParameters_t activationParameters;
    ChannelParams_t channelParameters;
    ProtocolParams_t protocolParameters;
    IsmBand_t ismBand;
    LorawanMacKeys_t macKeys;
    uint8_t crtMacCmdIndex;
    LorawanCommands_t macCommands[MAX_NB_CMD_TO_PROCESS];
    uint32_t lastTimerValue;
    uint32_t periodForLinkCheck;
    uint16_t adrAckCnt;
    uint16_t devNonce;
    uint16_t lastPacketLength;
    uint8_t maxRepetitionsUnconfirmedUplink;
    uint8_t maxRepetitionsConfirmedUplink;
    uint8_t counterRepetitionsUnconfirmedUplink;
    uint8_t counterRepetitionsConfirmedUplink;
    uint8_t lastUsedChannelIndex;
    uint16_t prescaler;
    uint8_t linkCheckMargin;
    uint8_t linkCheckGwCnt;
    uint8_t currentDataRate;
    uint8_t batteryLevel;
    uint8_t txPower;
    uint8_t joinAccept1TimerId;
    uint8_t joinAccept2TimerId;
    uint8_t receiveWindow1TimerId;
    uint8_t receiveWindow2TimerId;
    uint8_t automaticReplyTimerId;
    uint8_t linkCheckTimerId;
    uint8_t ackTimeoutTimerId;
    uint8_t dutyCycleTimerId;
    uint8_t unconfirmedRetransmisionTimerId;
    uint8_t minDataRate;
    uint8_t maxDataRate;
    uint8_t maxChannels;
    uint8_t counterAdrAckDelay;
    uint8_t offset;
    bool macInitialized;
    bool rx2DelayExpired;
    bool abpJoinStatus;
    uint8_t abpJoinTimerId;
    uint8_t syncWord;
    bool rxDnFrame;
	uint8_t otaaJoinTimerId;
    uint8_t otaaHourCount;
    uint8_t otaaMinuteCount;
} LoRa_t;

extern LoRa_t loRa;

/*************************** FUNCTIONS PROTOTYPE ******************************/


#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_EU_H */
