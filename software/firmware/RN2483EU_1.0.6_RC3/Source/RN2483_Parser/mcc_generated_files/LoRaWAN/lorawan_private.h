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
 *                           lorawan_private.h
 *
 * LoRaWAN private header file
 *
 ******************************************************************************/

#ifndef _LORAWAN_PRIVATE_H
#define	_LORAWAN_PRIVATE_H

#ifdef	__cplusplus
extern "C" {
#endif
    
/****************************** INCLUDES **************************************/    
#include "lorawan_defs.h"

/****************************** DEFINES ***************************************/    
    
#define INVALID_VALUE         0xFF

#define ENABLED                                 1
#define DISABLED                                0

#define ACCEPTED                                1
#define REJECTED                                0

#define RESPONSE_OK                             1
#define RESPONSE_NOT_OK                         0

#define FLAG_ERROR                              0
#define FLAG_OK                                 1

#define MCAST_ENABLED                       1
#define MCAST_DISABLED                      0    
    
#define RESERVED_FOR_FUTURE_USE                 0

#define MAXIMUM_BUFFER_LENGTH                   271

#define CLASS_C_RX_WINDOW_SIZE                  0

//Data rate (DR) encoding 
#define DR0                                     0  
#define DR1                                     1  
#define DR2                                     2  
#define DR3                                     3  
#define DR4                                     4  
#define DR5                                     5
#define DR6                                     6  
#define DR7                                     7
#define DR8                                     8
#define DR9                                     9
#define DR10                                    10
#define DR11                                    11
#define DR12                                    12
#define DR13                                    13 
#define DR14                                    14
#define DR15                                    15    
    
#define MAJOR_VERSION3                          0

#define LAST_NIBBLE                             0x0F
#define FIRST_NIBBLE                            0xF0 
    
// bit mask for MAC commands
// link ADR and RX2 setup
#define CHANNEL_MASK_ACK                        0x01
#define DATA_RATE_ACK                           0x02
#define RX1_DR_OFFSET_ACK                       0x04
#define POWER_ACK                               0x04
#define CHANNEL_MASK_FREQ_OK                    0x01
#define CHANNEL_MASK_UP_FREQ_EXISTS             0x02
    
#define FPORT_MIN                               1
#define FPORT_MAX                               223
    
#define MAX_NB_CMD_TO_PROCESS                   16     
    
//13 = sizeof(MIC) + MHDR + FHDR + sizeof (fPort);
#define HDRS_MIC_PORT_MIN_SIZE 13 
    
#define ABP_TIMEOUT_MS                          20
#define OTAA_MINUTECOUNT_MS                     ((uint32_t)60000)    

#define JA_APP_NONCE_SIZE                       3    
#define JA_NET_ID_SIZE                          3

#define MAX_FOPTS_LEN                           0x0F

/***************************** TYPEDEFS ***************************************/
    
typedef union
{
    uint8_t value;
    struct
    {
       unsigned nbRep:4;
       unsigned chMaskCntl:3;
       unsigned rfu:1;
    };
} Redundancy_t;

typedef enum
{
    IDLE                      =0,
    TRANSMISSION_OCCURRING      ,
    BEFORE_RX1                  ,         //between TX and RX1, FSK can occur
    RX1_OPEN                    ,
    BETWEEN_RX1_RX2             ,         //FSK can occur
    RX2_OPEN                    ,
    RETRANSMISSION_DELAY        ,         //used for ADR_ACK delay, FSK can occur
    ABP_DELAY                   ,         //used for delaying in calling the join callback for ABP
    CLASS_C_RX2_1_OPEN          ,
    CLASS_C_RX2_2_OPEN          ,
} LoRaMacState_t;   

// types of frames
typedef enum
{
    FRAME_TYPE_JOIN_REQ         =0x00 ,
    FRAME_TYPE_JOIN_ACCEPT            ,
    FRAME_TYPE_DATA_UNCONFIRMED_UP    ,
    FRAME_TYPE_DATA_UNCONFIRMED_DOWN  ,
    FRAME_TYPE_DATA_CONFIRMED_UP      ,
    FRAME_TYPE_DATA_CONFIRMED_DOWN    ,
    FRAME_TYPE_RFU                    ,
    FRAME_TYPE_PROPRIETARY            ,
}LoRaMacFrameType_t;

// MAC commands CID
typedef enum
{
    LINK_CHECK_CID              = 0x02,
    LINK_ADR_CID                = 0x03,
    DUTY_CYCLE_CID              = 0x04,
    RX_PARAM_SETUP_CID          = 0x05,
    DEV_STATUS_CID              = 0x06,
    NEW_CHANNEL_CID             = 0x07,
    RX_TIMING_SETUP_CID         = 0x08,
    TX_PARAM_SETUP_CID          = 0x09,
    DL_CHANNEL_CID              = 0x0A,
}LoRaMacCid_t;

//activation parameters
typedef union
{
    uint32_t value;
    uint8_t buffer[4];
} DeviceAddress_t;

typedef union
{
    uint32_t value;
    struct
    {
        uint16_t valueLow;
        uint16_t valueHigh;
    } members;
} FCnt_t;

//union used for instantiation of DeviceEui and Application Eui
typedef union
{
    uint8_t buffer[8];
    struct
    {
        uint32_t genericEuiL;
        uint32_t genericEuiH;
    }members;
} GenericEui_t;

typedef struct
{
    ActivationType_t activationType;
    DeviceAddress_t deviceAddress;
    uint8_t networkSessionKey[16];
    uint8_t applicationSessionKey[16];
    uint8_t applicationKey[16];   
    GenericEui_t applicationEui;
    GenericEui_t deviceEui;
    DeviceAddress_t mcastDeviceAddress;
    uint8_t mcastNetworkSessionKey[16];
    uint8_t mcastApplicationSessionKey[16];
} ActivationParameters_t;

typedef union
{
    uint8_t value;
    struct
    {
       unsigned fOptsLen:4;
       unsigned fPending:1;
       unsigned ack:1;
       unsigned adrAckReq:1;
       unsigned adr:1;
    };
} FCtrl_t;

// Mac header structure
typedef union
{
    uint8_t value;
    struct
    {
        uint8_t major           : 2;
        uint8_t rfu             : 3;
        uint8_t mType           : 3;
    }bits;
} Mhdr_t;

typedef union
{
    uint8_t fHdrCounter[23];
    struct
    {
        Mhdr_t mhdr             ;
        DeviceAddress_t devAddr ;
        FCtrl_t fCtrl           ;
        uint16_t fCnt           ;
        uint8_t MacCommands[15] ;
    }members;
} Hdr_t;

typedef union
{
    uint8_t value;
    struct
    {
        uint8_t rx2DataRate     : 4;
        uint8_t rx1DROffset     : 3;
        uint8_t rfu             : 1;
    }bits;
} DlSettings_t;

//Protocol parameters
typedef struct
{
    uint16_t receiveDelay1     ;
    uint16_t receiveDelay2     ;
    uint16_t joinAcceptDelay1  ;
    uint16_t joinAcceptDelay2  ;
    uint16_t maxFcntGap        ;
    uint16_t maxMultiFcntGap   ;
    uint16_t ackTimeout        ;
    uint8_t adrAckLimit        ;
    uint8_t adrAckDelay        ;
} ProtocolParams_t;

typedef struct
{
    uint8_t receivedCid;
    unsigned channelMaskAck :1;             // used for link adr answer
    unsigned dataRateAck :1;                // used for link adr answer
    unsigned powerAck :1;                   // used for link adr answer
    unsigned channelAck :1;                 // used for RX param setup request
    unsigned dataRateReceiveWindowAck :1;   // used for RX param setup request
    unsigned rx1DROffestAck :1;             // used for RX param setup request
    unsigned dataRateRangeAck :1;           // used for new channel answer
    unsigned channelFrequencyAck :1;        // used for new channel answer
    unsigned dlChannelFreqOkAck :1;         // used for DlChannel answer
    unsigned dlUplinkFreqExistsAck :1;      // used for DlChannel answer
} LorawanCommands_t;

typedef union
{
    uint16_t value;
    struct
    {
       unsigned deviceEui: 1;             //if set, device EUI was defined
       unsigned applicationEui:1;
       unsigned deviceAddress: 1;
       unsigned applicationKey:1;
       unsigned networkSessionKey:1;
       unsigned applicationSessionKey:1;
       unsigned mcastApplicationSessionKey:1;
       unsigned mcastNetworkSessionKey:1;
       unsigned mcastDeviceAddress:1;
    };
} LorawanMacKeys_t;

typedef struct
{
    uint32_t frequency;
    uint8_t dataRate;
} ReceiveWindowParameters_t;

// minimum and maximum data rate
typedef union
{
    uint8_t value;
    struct
    {
       unsigned min:4;
       unsigned max:4;
    };
} DataRange_t;

extern uint8_t macBuffer[];
extern uint8_t radioBuffer[];

extern RxAppData_t rxPayload;
bool rxParamSetupReqAnsFlag;
bool rxTimingSetupReqAnsFlag;
bool dlChannelReqAnsFlag;
uint8_t rxParamSetupAns;
uint8_t dlChannelAns;


/*************************** FUNCTIONS PROTOTYPE ******************************/

// Callback functions
void LORAWAN_ReceiveWindow1Callback (uint8_t param);

reentrant void LORAWAN_ReceiveWindow2Callback (uint8_t param);

void LORAWAN_LinkCheckCallback (uint8_t param);

void AckRetransmissionCallback (uint8_t param);

void UnconfirmedTransmissionCallback (uint8_t param);

void AutomaticReplyCallback (uint8_t param);

// Update and validation functions

void UpdateCurrentDataRate (uint8_t valueNew);

void UpdateTxPower (uint8_t txPowerNew);

void UpdateRetransmissionAckTimeoutState (void);

void UpdateJoinSuccessState(uint8_t param);

void UpdateMinMaxChDataRate (void);

void UpdateReceiveWindow2Parameters (uint32_t frequency, uint8_t dataRate);

void UpdateReceiveWindow1Parameters (uint32_t frequency);

void UpdateDlChannelWindow1Parameters (uint8_t channelIndex, uint32_t frequency);

void UpdateDLSettings(uint8_t dlRx2Dr, uint8_t dlRx1DrOffset);

LorawanError_t ValidateDataRate (uint8_t dataRate);

LorawanError_t ValidateTxPower (uint8_t txPowerNew);

LorawanError_t ValidateChMaskDataRate (uint16_t channelMask, uint8_t dataRate, uint8_t chmaskCntl);

//Initialization functions

void ResetParametersForConfirmedTransmission (void);

void ResetParametersForUnconfirmedTransmission (void);

void SetJoinFailState(void);

uint16_t Random (uint16_t max);


LorawanError_t SelectChannelForTransmission (bool transmissionType);  // transmission type is 0 means join request, transmission type is 1 means data message mode

void StartReTxTimer(void);

LorawanError_t SearchAvailableChannel (uint8_t maxChannels, bool transmissionType, uint8_t* channelIndex);

//MAC commands functions

uint8_t* ExecuteDutyCycle (uint8_t *ptr);

uint8_t* ExecuteLinkAdr (uint8_t *ptr, uint8_t *commandsLen);

uint8_t* ExecuteDevStatus (uint8_t *ptr);

uint8_t* ExecuteNewChannel (uint8_t *ptr);

uint8_t* ExecuteRxParamSetupReq   (uint8_t *ptr);

uint8_t* ExecuteDlChannelReq (uint8_t *ptr);


void ConfigureRadio(uint8_t dataRate, uint32_t freq);

uint32_t GetRx1Freq (void);

void LORAWAN_EnterContinuousReceive(void);


#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_PRIVATE_H */
