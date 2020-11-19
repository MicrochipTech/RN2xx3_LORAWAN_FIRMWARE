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
 * LoRaWAN NA header file
 *
 ******************************************************************************/

#ifndef _LORAWAN_NA_H
#define	_LORAWAN_NA_H

#ifdef	__cplusplus
extern "C" {
#endif 

/****************************** INCLUDES **************************************/  
#include "lorawan_defs.h"
    
/****************************** DEFINES ***************************************/    

#define MAX_US_CHANNELS                         72
#define MAX_US_CHANNELS_BANDWIDTH_125           64
#define MAX_US_CHANNELS_BANDWIDTH_500            8

//US channels for 915 Mhz
#define LC0_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC1_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC2_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC3_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC4_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC5_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC6_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC7_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC8_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC9_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC10_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC11_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC12_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC13_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC14_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC15_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC16_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC17_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC18_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC19_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC20_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC21_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC22_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC23_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC24_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC25_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC26_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC27_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC28_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC29_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC30_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC31_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC32_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC33_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC34_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC35_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC36_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC37_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC38_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC39_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC40_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC41_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC42_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC43_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC44_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC45_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC46_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC47_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC48_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC49_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC50_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC51_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC52_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC53_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC54_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC55_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC56_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC57_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC58_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC59_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC60_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC61_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC62_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC63_915                   { ENABLED,  { ( ( DR3 << SHIFT4 ) | DR0 ) } }
#define LC64_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC65_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC66_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC67_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC68_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC69_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC70_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }
#define LC71_915                   { ENABLED,  { ( ( DR4 << SHIFT4 ) | DR4 ) } }

#define TXPOWER_MIN                                 0
#define TXPOWER_MAX                                 10

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
    } members;
} JoinAccept_t;

//Channel parameters
typedef struct
{
        bool status;
        DataRange_t dataRange;
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
        unsigned synchronization:1;                      //if set, there is no need to send an immediat packet because the application sent one from the callback
        unsigned alternativeChannelUs:1;
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
    ProtocolParams_t protocolParameters;
    LorawanMacKeys_t macKeys;
    uint8_t crtMacCmdIndex;
    uint8_t crtMacCmdNewIndex;
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
    uint8_t transmissionErrorTimerId;
    uint8_t syncWord;
    bool rxDnFrame;
    bool macAnswerPending;
    uint32_t abdTimeout;
} LoRa_t;

extern LoRa_t loRa;


/*************************** FUNCTIONS PROTOTYPE ******************************/


#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_NA_H */
