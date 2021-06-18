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
 *                           lorawan.c
 *
 * LoRaWAN file
 *
 ******************************************************************************/

/****************************** INCLUDES **************************************/
#include "lorawan.h"
#include "lorawan_aes.h"
#include "lorawan_aes_cmac.h"
#include "lorawan_defs.h"
#include "lorawan_private.h"
#include "AES.h"
#include "radio_interface.h"
#include "sw_timer.h"
#include "interrupt_manager_lora_addons.h"
#include <math.h>

#include "lorawan_eu.h"

/****************************** VARIABLES *************************************/

//CID = LinkCheckReq     = 2
//CID = LinkADRAns       = 3
//CID = DutyCycleAns     = 4
//CID = RX2SetupAns      = 5
//CID = DevStatusAns     = 6
//CID = NewChannelAns    = 7
//CID = RXTimingSetupAns = 8
//CID = TXParamSetupAns  = 9
//CID = DlChannelAns     = 10
// Index in macEndDevCmdReplyLen = CID - 2
static const uint8_t macEndDevCmdReplyLen[] = {1, 2, 1, 2, 3, 2, 1, 1, 2};
//Excludes command id
static const uint8_t macEndDevCmdReqLen[] = {2, 4, 1, 4, 0, 5, 1, 1, 4};

LoRa_t loRa;

uint8_t macBuffer[MAXIMUM_BUFFER_LENGTH];
uint8_t radioBuffer[MAXIMUM_BUFFER_LENGTH];
static uint8_t aesBuffer[AES_BLOCKSIZE];
RxAppData_t rxPayload;

extern const uint8_t maxPayloadSize[];

extern const uint8_t maxPayloadSizeDwellLimited[];

extern ChannelParams_t Channels[];
extern const uint8_t rxWindowSize[];

/************************ FUNCTION PROTOTYPES *************************/

static void UpdateReceiveDelays (uint8_t delay);

static uint8_t LORAWAN_GetMaxPayloadSize (void);

static void AssemblePacket (bool confirmed, uint8_t port, uint8_t *buffer, uint16_t bufferLength);

static bool FindSmallestDataRate (void);

static uint8_t* ExecuteLinkCheck (uint8_t *ptr);

static uint8_t* ExecuteRxTimingSetup (uint8_t *ptr);

static uint8_t PrepareJoinRequestFrame (void);

static void PrepareSessionKeys (uint8_t* sessionKey, uint8_t* appNonce, uint8_t* networkId);

static void ComputeSessionKeys (JoinAccept_t *joinAcceptBuffer);

static uint8_t CountfOptsLength (void);

static void IncludeMacRxParamSetupResponse(uint8_t* macBuffer, uint8_t* pBufferIndex);

static void IncludeMacRxTimingSetupResponse (uint8_t* macBuffer, uint8_t* pBufferIndex);

static void IncludeMacDlChannelResponse (uint8_t* macBuffer, uint8_t* pBufferIndex);

static void IncludeMacCommandsResponse (uint8_t* macBuffer, uint8_t* pBufferIndex, uint8_t bIncludeInFopts );

static void IncludeMacRxTimingSetupResponse (uint8_t* macBuffer, uint8_t* pBufferIndex);

static void UpdateJoinInProgress(uint8_t state);

static void CheckFlags (Hdr_t* hdr);

static uint8_t CheckMcastFlags (Hdr_t* hdr);

static void AssembleEncryptionBlock (uint8_t dir, uint32_t frameCounter, uint8_t blockId, uint8_t firstByte, uint8_t multicastStatus);

static uint32_t ExtractMic (uint8_t *buffer, uint8_t bufferLength);

static uint32_t ComputeMic ( uint8_t *key, uint8_t* buffer, uint8_t bufferLength);

static void EncryptFRMPayload (uint8_t* buffer, uint8_t bufferLength, uint8_t dir, uint32_t frameCounter, uint8_t* key, uint8_t macBufferIndex, uint8_t* bufferToBeEncrypted, uint8_t multicastStatus);

static uint8_t* MacExecuteCommands (uint8_t *buffer, uint8_t fOptsLen);

static void SetReceptionNotOkState (void);

static void ConfigureRadioRx(uint8_t dataRate, uint32_t freq);


extern void UpdateCfList (uint8_t bufferLength, JoinAccept_t *joinAccept);

uint8_t localDioStatus;

/****************************** PUBLIC FUNCTIONS ******************************/

LorawanError_t LORAWAN_Join(ActivationType_t activationTypeNew)
{
    uint8_t bufferIndex;
    LorawanError_t result;

    if (loRa.macStatus.macPause == ENABLED)
    {
        return MAC_PAUSED;                               // Any further transmissions or receptions cannot occur is macPaused is enabled.
    }

    if (loRa.macStatus.silentImmediately == ENABLED)
    {
        return SILENT_IMMEDIATELY_ACTIVE;
    }

    if (loRa.macStatus.macState == CLASS_C_RX2_2_OPEN && CLASS_C == loRa.deviceClass)
    {
        RADIO_ReceiveStop();
    }
    else if (loRa.macStatus.macState  != IDLE)
    {
        return MAC_STATE_NOT_READY_FOR_TRANSMISSION;
    }

    loRa.activationParameters.activationType = activationTypeNew;

    if (OTAA == activationTypeNew)
    {
        //OTAA
        if ( (loRa.macKeys.deviceEui == 0) || (loRa.macKeys.applicationEui == 0) || (loRa.macKeys.applicationKey == 0) )
        {
            return KEYS_NOT_INITIALIZED;
        }
        else
        {
            bufferIndex = PrepareJoinRequestFrame ();
            result = SelectChannelForTransmission (0);

            if (result == OK)
            {
                if (RADIO_Transmit(macBuffer, bufferIndex) == OK)
                {
                    UpdateJoinInProgress(TRANSMISSION_OCCURRING);
                    return OK;
                }
                else
                {
                    return MAC_STATE_NOT_READY_FOR_TRANSMISSION;
                }
            }
            else
            {
                return result;
            }              
        }
    }
    else
    {
        //ABP
        if ( (loRa.macKeys.applicationSessionKey == 0) || (loRa.macKeys.networkSessionKey == 0) || (loRa.macKeys.deviceAddress == 0) )
        {
            return KEYS_NOT_INITIALIZED;
        }
        else
        {
            UpdateJoinInProgress(ABP_DELAY);
            SwTimerSetTimeout(loRa.abpJoinTimerId, MS_TO_TICKS_SHORT(ABP_TIMEOUT_MS));
            SwTimerStart(loRa.abpJoinTimerId);
            
            return OK; 
        }       
    }
}

LorawanError_t LORAWAN_Send (TransmissionType_t confirmed, uint8_t port,  void *buffer, uint8_t bufferLength)
 {
    LorawanError_t result;

    if (loRa.macStatus.macPause == ENABLED)
    {
        return MAC_PAUSED;                               // Any further transmissions or receptions cannot occur is macPaused is enabled.
    }

    if (loRa.macStatus.silentImmediately == ENABLED)  // The server decided that any further uplink transmission is not possible from this end device.
    {
        return SILENT_IMMEDIATELY_ACTIVE;
    }

    if (loRa.macStatus.networkJoined == DISABLED)          //The network needs to be joined before sending
    {
        return NETWORK_NOT_JOINED;
    }

    if ( (port < FPORT_MIN) && (bufferLength != 0) )   //Port number should be <= 1 if there is data to send. If port number is 0, it indicates only Mac commands are inside FRM Payload
    {
        return INVALID_PARAMETER;
    }

    //validate date length using MaxPayloadSize
    if (bufferLength > LORAWAN_GetMaxPayloadSize ())
    {
        return INVALID_BUFFER_LENGTH;
    }

    if (loRa.fCntUp.value == UINT32_MAX)
    {
        // Inform application about rejoin in status
        loRa.macStatus.rejoinNeeded = 1;          
        return FRAME_COUNTER_ERROR_REJOIN_NEEDED;
    }

    if ((loRa.macStatus.macState != IDLE) && (CLASS_A == loRa.deviceClass))
    {
        return MAC_STATE_NOT_READY_FOR_TRANSMISSION;      
    }

    result = SelectChannelForTransmission (1);
    if (result != OK)
    {
        return result;
    }
    else
    {
       if (CLASS_C == loRa.deviceClass)
       {
           RADIO_ReceiveStop();
       }

       AssemblePacket (confirmed, port, buffer, bufferLength);

       if (RADIO_Transmit (&macBuffer[16], loRa.lastPacketLength) == OK)
       {
           loRa.fCntUp.value ++;   // the uplink frame counter increments for every new transmission (it does not increment for a retransmission)

           if (CNF == confirmed)
           {
               loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage = ENABLED;
           }
           loRa.lorawanMacStatus.synchronization = ENABLED;  //set the synchronization flag because one packet was sent (this is a guard for the the RxAppData of the user)
           loRa.macStatus.macState = TRANSMISSION_OCCURRING; // set the state of MAC to transmission occurring. No other packets can be sent afterwards
       }
       else
       {
           return MAC_STATE_NOT_READY_FOR_TRANSMISSION;
       }
    }

    return OK;
}

//Set and get functions
LorawanError_t LORAWAN_SetMcast(bool status)
{
    if (CLASS_A == loRa.deviceClass)
    {
        return INVALID_CLASS; // it works only for Class B and Class C
    }

    // Only ABP shall be checked
    if (CLASS_C == loRa.deviceClass)
    {
        if(ENABLED == status)
        {
            if ((0 == loRa.macKeys.mcastApplicationSessionKey) ||
                (0 == loRa.macKeys.mcastNetworkSessionKey) ||
                (0 == loRa.macKeys.mcastDeviceAddress) )
            {
                return MCAST_PARAM_ERROR;
            }

            loRa.macStatus.mcastEnable = ENABLED;
        }
        else
        {
            loRa.macStatus.mcastEnable = DISABLED;
        }
    }
    
    return OK;
}

bool LORAWAN_GetMcast(void)
{
    return loRa.macStatus.mcastEnable;
}

void LORAWAN_SetMcastDeviceAddress (uint32_t mcastDeviceAddressNew)
{
   loRa.activationParameters.mcastDeviceAddress.value = mcastDeviceAddressNew;
   loRa.macKeys.mcastDeviceAddress = 1;
}

uint32_t LORAWAN_GetMcastDeviceAddress (void)
{
    return loRa.activationParameters.mcastDeviceAddress.value;
}

void LORAWAN_SetMcastNetworkSessionKey (uint8_t *mcastNetworkSessionKeyNew)
{
    memcpy(loRa.activationParameters.mcastNetworkSessionKey, mcastNetworkSessionKeyNew, 16);
    loRa.macKeys.mcastNetworkSessionKey = 1;
}

void LORAWAN_SetMcastApplicationSessionKey (uint8_t *mcastApplicationSessionKeyNew)
{
    memcpy( loRa.activationParameters.mcastApplicationSessionKey, mcastApplicationSessionKeyNew, 16);
    loRa.macKeys.mcastApplicationSessionKey = 1;
}

void LORAWAN_GetMcastApplicationSessionKey (uint8_t *mcastApplicationSessionKey)
{
    if (mcastApplicationSessionKey != NULL)
    {
        memcpy (mcastApplicationSessionKey, loRa.activationParameters.mcastApplicationSessionKey, 16);
    }
}

void LORAWAN_GetMcastNetworkSessionKey (uint8_t *mcastNetworkSessionKey)
{
    if (mcastNetworkSessionKey != NULL)
    {
        memcpy(mcastNetworkSessionKey, loRa.activationParameters.mcastNetworkSessionKey, sizeof(loRa.activationParameters.mcastNetworkSessionKey) );
    }
}

void LORAWAN_SetDeviceEui (uint8_t *deviceEuiNew)
{
   if (deviceEuiNew != NULL)
   {
       memcpy(loRa.activationParameters.deviceEui.buffer, deviceEuiNew, sizeof(loRa.activationParameters.deviceEui) );
       loRa.macKeys.deviceEui = 1;
       loRa.macStatus.networkJoined = DISABLED; // this is a guard against overwriting any of the addresses after one join was already done. If any of the addresses change, rejoin is needed
   }
}

void LORAWAN_GetDeviceEui (uint8_t *deviceEui)
{
   memcpy(deviceEui, loRa.activationParameters.deviceEui.buffer, sizeof(loRa.activationParameters.deviceEui) );
}

void LORAWAN_SetApplicationEui (uint8_t *applicationEuiNew)
{
   if (applicationEuiNew != NULL)
   {
       memcpy(loRa.activationParameters.applicationEui.buffer, applicationEuiNew, 8);
       loRa.macKeys.applicationEui = 1;
       loRa.macStatus.networkJoined = DISABLED; // this is a guard against overwriting any of the addresses after one join was already done. If any of the addresses change, rejoin is needed
   }
}

void LORAWAN_GetApplicationEui (uint8_t *applicationEui)
{
   memcpy (applicationEui, loRa.activationParameters.applicationEui.buffer, sizeof(loRa.activationParameters.applicationEui) );
}

void LORAWAN_SetDeviceAddress (uint32_t deviceAddressNew)
{
   loRa.activationParameters.deviceAddress.value = deviceAddressNew;
   loRa.macKeys.deviceAddress = 1;
   loRa.macStatus.networkJoined = DISABLED; // this is a guard against overwriting any of the addresses after one join was already done. If any of the addresses change, rejoin is needed
}

uint32_t LORAWAN_GetDeviceAddress (void)
{
   return loRa.activationParameters.deviceAddress.value;
}

void LORAWAN_SetNetworkSessionKey (uint8_t *networkSessionKeyNew)
{
   if (networkSessionKeyNew != NULL)
   {
       memcpy(loRa.activationParameters.networkSessionKey, networkSessionKeyNew, 16);
       loRa.macKeys.networkSessionKey = 1;
       loRa.macStatus.networkJoined = DISABLED; // this is a guard against overwriting any of the addresses after one join was already done. If any of the addresses change, rejoin is needed
   }
}

void LORAWAN_GetNetworkSessionKey (uint8_t *networkSessionKey)
{
    memcpy (networkSessionKey, loRa.activationParameters.networkSessionKey, sizeof(loRa.activationParameters.networkSessionKey) );
}

void LORAWAN_SetApplicationSessionKey (uint8_t *applicationSessionKeyNew)
{
    if (applicationSessionKeyNew != NULL)
    {
        memcpy( loRa.activationParameters.applicationSessionKey, applicationSessionKeyNew, 16);
        loRa.macKeys.applicationSessionKey = 1;
        loRa.macStatus.networkJoined = DISABLED; // this is a guard against overwriting any of the addresses after one join was already done. If any of the addresses change, rejoin is needed
    }
}

void LORAWAN_GetApplicationSessionKey (uint8_t *applicationSessionKey)
{
    memcpy (applicationSessionKey, loRa.activationParameters.applicationSessionKey, sizeof(loRa.activationParameters.applicationSessionKey) );
}

void LORAWAN_SetApplicationKey (uint8_t *applicationKeyNew)
{
    if (applicationKeyNew != NULL)
    {
        memcpy( loRa.activationParameters.applicationKey, applicationKeyNew, 16);
        loRa.macKeys.applicationKey = 1;
        loRa.macStatus.networkJoined = DISABLED; // this is a guard against overwriting any of the addresses after one join was already done. If any of the addresses change, rejoin is needed
    }
}

void LORAWAN_GetApplicationKey (uint8_t *applicationKey)
{
    memcpy (applicationKey, loRa.activationParameters.applicationKey, sizeof(loRa.activationParameters.applicationKey) );
}

void LORAWAN_SetAdr (bool status)
{
    loRa.macStatus.adr = status;
    loRa.adrAckCnt = DISABLED;
    loRa.lorawanMacStatus.adrAckRequest = DISABLED; // this flag should only be on when ADR is set and the adr ack counter is bigger than adr ack limit
}

void LORAWAN_SetUplinkDwell (bool status)
{
    loRa.macStatus.uplinkDwellTimeFlag = status;
    
}

void LORAWAN_SetDownlinkDwell (bool status)
{
    loRa.macStatus.downlinkDwellTimeFlag = status;
    
}

bool LORAWAN_GetAdr (void)
{
    return loRa.macStatus.adr;
}

LorawanError_t LORAWAN_SetCurrentDataRate (uint8_t valueNew)
{
    // the current data rate cannot be smaller than the minimum data rate defined for all the channels or bigger than the maximum data rate defined for all the channels

    if ( (valueNew < loRa.minDataRate) || (valueNew > loRa.maxDataRate) || (ValidateDataRate(valueNew) != OK) )
    {
        return INVALID_PARAMETER;
    }
    else
    {
        UpdateCurrentDataRate (valueNew);
        return OK;
    }
}

uint8_t LORAWAN_GetCurrentDataRate (void)
{
    return loRa.currentDataRate;
}

LorawanError_t LORAWAN_SetTxPower (uint8_t txPowerNew)
{
   LorawanError_t result = OK;

    if (ValidateTxPower (txPowerNew) == OK)
    {
        UpdateTxPower (txPowerNew);
    }
    else
    {
        result = INVALID_PARAMETER;
    }
    return result;
}

uint8_t LORAWAN_GetTxPower (void)
{
    return loRa.txPower;
}

uint8_t LORAWAN_GetSyncWord (void)
{
    return loRa.syncWord;
}

void LORAWAN_SetSyncWord (uint8_t syncWord)
{
    loRa.syncWord = syncWord;
}

void LORAWAN_SetUplinkCounter (uint32_t ctr)
{
    loRa.fCntUp.value = ctr;
}

uint32_t LORAWAN_GetUplinkCounter (void)
{
    return loRa.fCntUp.value;
}

void LORAWAN_SetDownlinkCounter (uint32_t ctr)
{
    loRa.fCntDown.value = ctr;
}

uint32_t LORAWAN_GetDownlinkCounter (void)
{
    return loRa.fCntDown.value;
}

// Set and get functions for protocol parameters
void LORAWAN_SetReceiveDelay1 (uint16_t receiveDelay1New)
{
    loRa.protocolParameters.receiveDelay1 = receiveDelay1New;
    loRa.protocolParameters.receiveDelay2 = loRa.protocolParameters.receiveDelay1 + 1000;  // 1 second after receive delay 1
}

void LORAWAN_SetReceiveOffset (uint8_t offset)
{
    loRa.offset = offset;
}

uint8_t LORAWAN_GetReceiveOffset (void)
{
    return loRa.offset;
}

uint16_t LORAWAN_GetReceiveDelay1 (void)
{
    return loRa.protocolParameters.receiveDelay1;
}

uint16_t LORAWAN_GetReceiveDelay2 (void)
{
    return loRa.protocolParameters.receiveDelay2;
}

void LORAWAN_SetJoinAcceptDelay1 (uint16_t joinAcceptDelay1New)
{
    loRa.protocolParameters.joinAcceptDelay1 = joinAcceptDelay1New;
}

uint16_t LORAWAN_GetJoinAcceptDelay1 (void)
{
    return loRa.protocolParameters.joinAcceptDelay1;
}

void LORAWAN_SetJoinAcceptDelay2 (uint16_t joinAcceptDelay2New)
{
    loRa.protocolParameters.joinAcceptDelay2 = joinAcceptDelay2New;
}

uint16_t LORAWAN_GetJoinAcceptDelay2 (void)
{
    return loRa.protocolParameters.joinAcceptDelay2;
}

void LORAWAN_SetMaxFcntGap (uint16_t maxFcntGapNew)
{
    loRa.protocolParameters.maxFcntGap = maxFcntGapNew;
}

uint16_t LORAWAN_GetMaxFcntGap (void)
{
    return loRa.protocolParameters.maxFcntGap;
}

void LORAWAN_SetAdrAckLimit (uint8_t adrAckLimitNew)
{
    loRa.protocolParameters.adrAckLimit = adrAckLimitNew;
}

uint8_t LORAWAN_GetAdrAckLimit (void)
{
    return loRa.protocolParameters.adrAckLimit;
}

void LORAWAN_SetAdrAckDelay(uint8_t adrAckDelayNew)
{
    loRa.protocolParameters.adrAckDelay = adrAckDelayNew;
}

uint8_t LORAWAN_GetAdrAckDelay (void)
{
    return loRa.protocolParameters.adrAckDelay;
}

void LORAWAN_SetAckTimeout(uint16_t ackTimeoutNew)
{
    loRa.protocolParameters.ackTimeout = ackTimeoutNew;
}

uint16_t LORAWAN_GetAckTimeout (void)
{
    return loRa.protocolParameters.ackTimeout;
}

void LORAWAN_SetClass (LoRaClass_t deviceClass)
{
    loRa.deviceClass = deviceClass;
    loRa.macStatus.mcastEnable = 0;

    if (CLASS_C == deviceClass)
    {
        RADIO_SetWatchdogTimeout(0);
    }
    else if (deviceClass == CLASS_A)
    {
        loRa.macStatus.macState = IDLE;
        RADIO_SetWatchdogTimeout(WATCHDOG_DEFAULT_TIME);
        RADIO_ReceiveStop();
    }
}

LoRaClass_t LORAWAN_GetClass (void)
{
    return loRa.deviceClass;
}

void LORAWAN_SetMcastDownCounter(uint32_t newCnt)
{
    loRa.fMcastCntDown.value = newCnt;
}

uint32_t LORAWAN_GetMcastDownCounter()
{
    return loRa.fMcastCntDown.value;
}

void LORAWAN_SetNumberOfRetransmissions (uint8_t numberRetransmissions)
{
    loRa.maxRepetitionsConfirmedUplink = numberRetransmissions;
}

// for confirmed frames, default value is 8. The number of retransmissions includes also the first transmission
uint8_t LORAWAN_GetNumberOfRetransmissions (void)
{
    return loRa.maxRepetitionsConfirmedUplink;
}

void LORAWAN_GetReceiveWindow2Parameters (uint32_t* frequency, uint8_t* dataRate)
{
    *dataRate = loRa.receiveWindow2Parameters.dataRate;
    *frequency = loRa.receiveWindow2Parameters.frequency;   
}

// battery Level: 0 - external power source,  1-254 level, 255: the end device was not able to measure the battery level
// default value for battery is 255 - the end device was not able to measure the battery level
void LORAWAN_SetBattery (uint8_t batteryLevelNew)
{
    loRa.batteryLevel = batteryLevelNew;
}

// the LORAWAN_GetPrescaler function returns the prescaler value that is sent by the server to the end device via the Mac Command
// not user configurable
uint16_t LORAWAN_GetPrescaler (void)
{
    return loRa.prescaler;
}
 
// if status is enabled, responses to ACK and MAC commands will be sent immediately
void LORAWAN_SetAutomaticReply (bool status)
{
    loRa.macStatus.automaticReply = status;
}

bool LORAWAN_GetAutomaticReply (void)
{
    return loRa.macStatus.automaticReply;
}

// not user configurable
uint32_t LORAWAN_GetStatus (void)
{
    uint32_t status = loRa.macStatus.value;

    loRa.macStatus.channelsModified = DISABLED;
    loRa.macStatus.txPowerModified = DISABLED;
    loRa.macStatus.nbRepModified = DISABLED; 
    loRa.macStatus.prescalerModified = DISABLED;
    loRa.macStatus.secondReceiveWindowModified = DISABLED;
    loRa.macStatus.rxTimingSetup = DISABLED;

    return status;
}

// not user configurable
uint8_t LORAWAN_GetLinkCheckMargin (void)
{
    return loRa.linkCheckMargin;
}

// not user configurable
uint8_t LORAWAN_GetLinkCheckGwCnt (void)
{
    return loRa.linkCheckGwCnt;
}

/* This function is called when there is a need to send data outside the MAC layer
 It can be called when MAC is in idle, before RX1, between RX1 and RX2 or retransmission delay state
 It will return how much time other transmissions can occur*/
uint32_t LORAWAN_Pause (void)
{
    uint32_t timeToPause;

    switch (loRa.macStatus.macState)
    {
        case IDLE:
        {
            timeToPause = UINT32_MAX;
        } break;

        case BEFORE_RX1:
        {
            if (loRa.lorawanMacStatus.joining == ENABLED)
            {
                timeToPause = TICKS_TO_MS(SwTimerReadValue (loRa.joinAccept1TimerId));
            }
            else if (loRa.macStatus.networkJoined == ENABLED)
            {
                timeToPause = TICKS_TO_MS(SwTimerReadValue (loRa.receiveWindow1TimerId));
            }
        } break;

        case BETWEEN_RX1_RX2:
        {
            if (loRa.lorawanMacStatus.joining == ENABLED)
            {
                timeToPause = SwTimerReadValue (loRa.joinAccept2TimerId);
            }
            else if (loRa.macStatus.networkJoined == ENABLED)
            {
                timeToPause = SwTimerReadValue (loRa.receiveWindow2TimerId);
            }
            timeToPause = TICKS_TO_MS(timeToPause);
        } break;

        case RETRANSMISSION_DELAY:
        {
            timeToPause = SwTimerReadValue (loRa.ackTimeoutTimerId);
            timeToPause = TICKS_TO_MS(timeToPause);
        } break;

        default:
        {
            timeToPause = 0;
        } break;
    }

    if (timeToPause >= 200)
    {
        timeToPause = timeToPause - 50; //this is a guard in case of non-syncronization
        loRa.macStatus.macPause = ENABLED;
    }
    else
    {
        timeToPause = 0;
        loRa.macStatus.macPause = DISABLED;
    }
    
    return timeToPause;
}

void LORAWAN_Resume (void)  
{
    loRa.macStatus.macPause = DISABLED;
}

// period will be in seconds
void LORAWAN_LinkCheckConfigure (uint16_t period)
{
    uint8_t iCtr;

    loRa.periodForLinkCheck = period * 1000UL;
    // max link check period is 18 hours, period 0 means disabling the link check mechanism, default is disabled
    if (period == 0)
    {
        SwTimerStop(loRa.linkCheckTimerId); // stop the link check timer
        loRa.macStatus.linkCheck = DISABLED;
        for(iCtr = 0; iCtr < loRa.crtMacCmdIndex; iCtr ++)
        {
            if(loRa.macCommands[iCtr].receivedCid == LINK_CHECK_CID)
            {
                //disable the link check mechanism 
                //Mark this CID as invalid
                loRa.macCommands[iCtr].receivedCid = INVALID_VALUE;
                loRa.crtMacCmdIndex --;
            }
        }        
    }
    else
    {
        loRa.macStatus.linkCheck = ENABLED;

        // if network is joined, the timer can start, otherwise after the network is joined the link check timer will start counting automatially
        if (loRa.macStatus.networkJoined == ENABLED)
        {
           SwTimerSetTimeout(loRa.linkCheckTimerId, MS_TO_TICKS(loRa.periodForLinkCheck));
           SwTimerStart(loRa.linkCheckTimerId);
        }
    }

}

// if LORAWAN_ForceEnable is sent, the Silent Immediately bit sent by the end device is discarded and transmission is possible again
void LORAWAN_ForceEnable (void)
{
    loRa.macStatus.silentImmediately = DISABLED;
}

void LORAWAN_ReceiveWindow1Callback (uint8_t param)
{
    uint32_t freq;
    
    if(loRa.macStatus.macPause == DISABLED)
    {
        if (CLASS_C == loRa.deviceClass)
        {
            RADIO_ReceiveStop();
        }

        if (loRa.receiveWindow1Parameters.dataRate >= loRa.offset)
        {
//            loRa.receiveWindow1Parameters.dataRate = loRa.receiveWindow1Parameters.dataRate - loRa.offset;
        }
        else
        {
            loRa.receiveWindow1Parameters.dataRate = DR0;
        }

        freq = GetRx1Freq();
        
        loRa.macStatus.macState = RX1_OPEN;
        
        RADIO_ReleaseData();
        
        ConfigureRadioRx(loRa.receiveWindow1Parameters.dataRate, freq);

        RADIO_ReceiveStart(rxWindowSize[loRa.receiveWindow1Parameters.dataRate]);
    }
}

reentrant void LORAWAN_ReceiveWindow2Callback(uint8_t param)
{
    // Make sure the radio is not currently receiving (because a long packet is being received on RX window 1
    if (loRa.macStatus.macPause == DISABLED)
    {
        if((RADIO_GetStatus() & RADIO_FLAG_RECEIVING) == 0)
        {
            loRa.macStatus.macState = RX2_OPEN;
            
            RADIO_ReceiveStop();
            RADIO_ReleaseData();
                    
            ConfigureRadioRx(loRa.receiveWindow2Parameters.dataRate, loRa.receiveWindow2Parameters.frequency);


            if (RADIO_ReceiveStart(rxWindowSize[loRa.receiveWindow2Parameters.dataRate]) != OK)
            {
                ResetParametersForConfirmedTransmission ();
                ResetParametersForUnconfirmedTransmission ();
                if (rxPayload.RxAppData != NULL)
                {
                    rxPayload.RxAppData (NULL, 0, MAC_NOT_OK);
                }
            }
        }

        else
        {
            // A packet is currently being received in RX1 Window
            //This flag is used when the reception in RX1 is overlapping the opening of RX2
            loRa.rx2DelayExpired = 1;
        }
    }
    else
    {
        //Transceiver standalone
        //MAC transmission ended during radio standalone function
        if (loRa.lorawanMacStatus.joining == 1)
        {
            loRa.lorawanMacStatus.joining = 0;
            loRa.macStatus.networkJoined = 0;
        }

        ResetParametersForUnconfirmedTransmission();
        ResetParametersForConfirmedTransmission();
    }
}

void LORAWAN_LinkCheckCallback (uint8_t param)
{
    uint8_t iCtr = 0;
    
    if ((loRa.macStatus.macPause == DISABLED) && (loRa.macStatus.linkCheck == ENABLED))
    {        
        // Check to see if there are other LINK_CHECK_CID added. In case there are, do nothing
        for(iCtr = 0; iCtr < loRa.crtMacCmdIndex; iCtr ++)
        {
            if(loRa.macCommands[iCtr].receivedCid == LINK_CHECK_CID)
            {
                break;
            }
        }
        
        if(iCtr == loRa.crtMacCmdIndex)
        {
            loRa.macCommands[loRa.crtMacCmdIndex].receivedCid = LINK_CHECK_CID;

            if(loRa.crtMacCmdIndex < MAX_NB_CMD_TO_PROCESS)
            {
                loRa.crtMacCmdIndex ++;            
            }              
        }              
    }

    //Set link check timeout to the configured interval
    if (loRa.macStatus.linkCheck == ENABLED)
    {
        SwTimerSetTimeout(loRa.linkCheckTimerId, MS_TO_TICKS(loRa.periodForLinkCheck));
        SwTimerStart(loRa.linkCheckTimerId);
    }
}

void LORAWAN_EnterContinuousReceive(void)
{

    RADIO_ReceiveStop();
    RADIO_ReleaseData();

    ConfigureRadioRx(loRa.receiveWindow2Parameters.dataRate, loRa.receiveWindow2Parameters.frequency);

    if (RADIO_ReceiveStart(CLASS_C_RX_WINDOW_SIZE) != OK)
    {
        ResetParametersForConfirmedTransmission ();
        ResetParametersForUnconfirmedTransmission ();
        loRa.macStatus.macState = IDLE;
        if (rxPayload.RxAppData != NULL)
        {
            rxPayload.RxAppData (NULL, 0, MAC_NOT_OK);
        }
    }
}
void AckRetransmissionCallback (uint8_t param)
{
    uint8_t maximumPacketSize;

    if (loRa.macStatus.macPause == DISABLED)
    {
        if (CLASS_C == loRa.deviceClass)
        {
            RADIO_ReceiveStop();
        }
        if (loRa.counterRepetitionsConfirmedUplink <= loRa.maxRepetitionsConfirmedUplink)
         {
                maximumPacketSize = LORAWAN_GetMaxPayloadSize () + HDRS_MIC_PORT_MIN_SIZE;

                // after changing the dataRate, we must check if the size of the packet is still valid
                if (loRa.lastPacketLength <= maximumPacketSize)
                {
                    if (SelectChannelForTransmission (1) == OK)
                    {
                        //resend the last packet
                        if (RADIO_Transmit (&macBuffer[16], loRa.lastPacketLength) == OK)
                        {
                            loRa.macStatus.macState = TRANSMISSION_OCCURRING;
                            loRa.counterRepetitionsConfirmedUplink ++ ; //for each retransmission (if possible or not), the counter increments

                            // change dataRate (smaller) and check payload size again with this new dataRate, dataRate should change from 2 to 2 transmissions (transmission 3, 5, 7...)
                            if ( ( (loRa.counterRepetitionsConfirmedUplink % 2) == 0) && (loRa.currentDataRate != DR0) && (loRa.counterRepetitionsConfirmedUplink != (loRa.maxRepetitionsConfirmedUplink + 1) ) )
                            {
                                FindSmallestDataRate();
                            }
                        }
                        else
                        {
                            // if transmission was not possible, we must wait another ACK timeout seconds period of time to initiate a new transmission
                            UpdateRetransmissionAckTimeoutState ();
                        }
                    }
                    else
                    {
                        // if transmission was not possible, we must wait another ACK timeout seconds period of time to initiate a new transmission
                        UpdateRetransmissionAckTimeoutState ();
                    }
                }
                else
                {
                    ResetParametersForConfirmedTransmission ();
                    if (rxPayload.RxAppData != NULL)
                    {
                        rxPayload.RxAppData(NULL, 0, INVALID_BUFFER_LEN);
                    }
                }
            }
            else if ( (loRa.counterRepetitionsConfirmedUplink > loRa.maxRepetitionsConfirmedUplink) && (loRa.macStatus.macPause == DISABLED) )
            {
                ResetParametersForConfirmedTransmission ();
                if (rxPayload.RxAppData != NULL)
                {
                    rxPayload.RxAppData(NULL, 0, MAC_NOT_OK);
                }
            }
    }
    else 
    {
        ResetParametersForConfirmedTransmission ();
    }
}

void UnconfirmedTransmissionCallback (uint8_t param)
{
        //resend the last packet if the radio transmit function succeeds
    if ( (SelectChannelForTransmission (1) == OK) && (RADIO_Transmit (&macBuffer[16], loRa.lastPacketLength) == OK) )
    {
        loRa.counterRepetitionsUnconfirmedUplink ++ ; //for each retransmission, the counter increments
        loRa.macStatus.macState = TRANSMISSION_OCCURRING; // set the state of MAC to transmission occurring. No other packets can be sent afterwards
    }
    else
    // if radio cannot transmit, then no more retransmissions will occur for this packet
    {
        ResetParametersForUnconfirmedTransmission ();
        if (rxPayload.RxAppData != NULL)
        {
            rxPayload.RxAppData(NULL, 0, MAC_NOT_OK);  // inform the application layer that no message was received back from the server
        }
    }
}

void AutomaticReplyCallback (uint8_t param)
{
    loRa.macStatus.macState = IDLE;
    LORAWAN_Send (0, 0, 0, 0);  // send an empty unconfirmed packet
    loRa.lorawanMacStatus.fPending = DISABLED; //clear the fPending flag
}

void UpdateCurrentDataRate (uint8_t valueNew)
{
    loRa.currentDataRate = valueNew;
}

void UpdateTxPower (uint8_t txPowerNew)
{
    loRa.txPower = txPowerNew;
}

void UpdateRetransmissionAckTimeoutState (void)
{
    loRa.macStatus.macState = RETRANSMISSION_DELAY;
    SwTimerSetTimeout(loRa.ackTimeoutTimerId, MS_TO_TICKS_SHORT(loRa.protocolParameters.ackTimeout));
    SwTimerStart(loRa.ackTimeoutTimerId);
}

void UpdateJoinSuccessState(uint8_t param)
{
    loRa.lorawanMacStatus.joining = 0;  //join was done
    loRa.macStatus.networkJoined = 1;   //network is joined
    loRa.macStatus.macState = IDLE;

    loRa.adrAckCnt = 0;  // adr ack counter becomes 0, it increments only for ADR set
    loRa.counterAdrAckDelay = 0;

    // if the link check mechanism was enabled, then its timer will begin counting
    if (loRa.macStatus.linkCheck == ENABLED)
    {
        SwTimerSetTimeout(loRa.linkCheckTimerId, MS_TO_TICKS(loRa.periodForLinkCheck));
        SwTimerStart(loRa.linkCheckTimerId);
    }
    
    if (rxPayload.RxJoinResponse != NULL)
    {
        rxPayload.RxJoinResponse(ACCEPTED); // inform the application layer that join was successful via a callback
    }
}

void UpdateMinMaxChDataRate (void)
{
    uint8_t i;
    // after updating the data range of a channel we need to check if the minimum dataRange has changed or not.
    // The user cannot set the current data rate outside the range of the data range
    loRa.minDataRate = DR7;
    
    if(loRa.macStatus.uplinkDwellTimeFlag = ENABLED)
        loRa.maxDataRate = DR2;
    else
        loRa.maxDataRate = DR0;

    for (i = 0; i < loRa.maxChannels; i++)
    {
        if ( (Channels[i].dataRange.min < loRa.minDataRate) && (Channels[i].status == ENABLED) )
        {
            loRa.minDataRate = Channels[i].dataRange.min;
        }
        if ( (Channels[i].dataRange.max > loRa.maxDataRate) && (Channels[i].status == ENABLED) )
        {
            loRa.maxDataRate = Channels[i].dataRange.max;
        }
    }

}

void UpdateMaxChDataRate (bool upDwellTimeFlag)
{
    uint8_t i;
    // after updating the data range of a channel we need to check if the minimum dataRange has changed or not.
    // The user cannot set the current data rate outside the range of the data range
    if (loRa.macStatus.uplinkDwellTimeFlag == ENABLED)
    {
        loRa.minDataRate = DR2;
    }
    else
    {
        loRa.minDataRate = DR0;
    }

    Channels[0].dataRange.min = loRa.minDataRate;
    Channels[1].dataRange.min = loRa.minDataRate;
    
    for (i = 2; i < loRa.maxChannels; i++)
    {
        if ( (Channels[i].dataRange.min < loRa.minDataRate) && (Channels[i].status == ENABLED) )
        {
            Channels[i].dataRange.min = loRa.minDataRate;
        }
    }
}

void UpdateReceiveWindow2Parameters (uint32_t frequency, uint8_t dataRate)
{
    loRa.receiveWindow2Parameters.dataRate = dataRate;
    loRa.receiveWindow2Parameters.frequency = frequency;
}

void UpdateReceiveWindow1Parameters (uint32_t frequency)
{
    loRa.receiveWindow1Parameters.frequency = frequency;
}

void UpdateDlChannelWindow1Parameters (uint8_t channelIndex, uint32_t frequency)
{
    Channels[channelIndex].downFrequency = frequency;
}

void ResetParametersForConfirmedTransmission (void)
{
    loRa.macStatus.macState = IDLE;
    loRa.counterRepetitionsConfirmedUplink = 1;
    loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage = DISABLED;
}

void ResetParametersForUnconfirmedTransmission (void)
{
    loRa.macStatus.macState = IDLE;
    loRa.counterRepetitionsUnconfirmedUplink = 1;
    loRa.crtMacCmdIndex = 0;
}

void SetJoinFailState(void)
{
    loRa.macStatus.networkJoined = 0;
    loRa.lorawanMacStatus.joining = 0;
    loRa.macStatus.macState = IDLE;
    if (rxPayload.RxJoinResponse != NULL)
    {
        rxPayload.RxJoinResponse(REJECTED); // inform the application layer that join failed via callback
    }
}

//Generates a 16-bit random number based on some measurements made by the radio (seed is initialized inside mac Reset)
uint16_t Random (uint16_t max)
{
    return (rand () % max);
}

LorawanError_t LORAWAN_RxDone (uint8_t *buffer, uint8_t bufferLength)
{
    uint32_t computedMic, extractedMic;
    Mhdr_t mhdr;
    uint8_t fPort, bufferIndex, channelIndex;
    uint8_t frmPayloadLength;
    uint8_t *packet;
    bool response;
    uint8_t temp;
    FCnt_t tempFcnt;

    RADIO_ReleaseData();

    if (loRa.macStatus.macPause == DISABLED)
    {
        mhdr.value = buffer[0];
        if ( (mhdr.bits.mType == FRAME_TYPE_JOIN_ACCEPT) && (loRa.activationParameters.activationType == 0) )
        {
            temp = bufferLength - 1; //MHDR not encrypted
            while (temp > 0) 
            {                
               //Decode message
               AESEncodeLoRa (&buffer[bufferLength - temp], loRa.activationParameters.applicationKey);
               if (temp > AES_BLOCKSIZE)
               {
                   temp -= AES_BLOCKSIZE;
               }
               else
               {
                   temp = 0;
               }               
            }

            //verify MIC
            computedMic = ComputeMic (loRa.activationParameters.applicationKey, buffer, bufferLength - sizeof(extractedMic));
            extractedMic = ExtractMic (buffer, bufferLength);
            if (extractedMic != computedMic)
            {
                if ((loRa.macStatus.macState == RX2_OPEN) || ((loRa.macStatus.macState == RX1_OPEN) && (loRa.rx2DelayExpired)))
                {
                    if(loRa.macStatus.networkJoined == 0)
                    {
                        SetJoinFailState();
                    }
                    else
                    {
                        loRa.macStatus.macState = IDLE;
                    }
                }
                if (CLASS_C == loRa.deviceClass)
                {
                    loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                    LORAWAN_EnterContinuousReceive();
                }
                return INVALID_PARAMETER;
            }

            // if the join request message was received during receive window 1, receive window 2 should not open any more, so its timer will be stopped
            if (loRa.macStatus.macState == RX1_OPEN)
            {
                SwTimerStop (loRa.joinAccept2TimerId);
            }

            JoinAccept_t *joinAccept;
            joinAccept = (JoinAccept_t*)buffer;

            loRa.activationParameters.deviceAddress.value = joinAccept->members.deviceAddress.value; //device address is saved

            UpdateReceiveDelays (joinAccept->members.rxDelay & LAST_NIBBLE); //receive delay 1 and receive delay 2 are updated according to the rxDelay field from the join accept message

            UpdateDLSettings(joinAccept->members.DLSettings.bits.rx2DataRate, joinAccept->members.DLSettings.bits.rx1DROffset);

            UpdateCfList (bufferLength, joinAccept);

            ComputeSessionKeys (joinAccept); //for activation by personalization, the network and application session keys are computed

            UpdateJoinSuccessState(0);
            
            loRa.fCntUp.value = 0;   // uplink counter becomes 0
            loRa.fCntDown.value = 0; // downlink counter becomes 0              
            
            if (CLASS_C == loRa.deviceClass)
            {
                loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                LORAWAN_EnterContinuousReceive();
            }
            return OK;
        }
        else if ( (mhdr.bits.mType == FRAME_TYPE_DATA_UNCONFIRMED_DOWN) || (mhdr.bits.mType == FRAME_TYPE_DATA_CONFIRMED_DOWN) )
        {
            loRa.crtMacCmdIndex = 0;   // clear the macCommands requests list

            Hdr_t *hdr;
            hdr=(Hdr_t*)buffer;

            // CLASS C MULTICAST
            if ( (CLASS_C == loRa.deviceClass) && (hdr->members.devAddr.value == loRa.activationParameters.mcastDeviceAddress.value) && (ENABLED == loRa.macStatus.mcastEnable) )
            {
                
                if (FLAG_ERROR == CheckMcastFlags(hdr))
                {
                    loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                    LORAWAN_EnterContinuousReceive();
                    return MCAST_MSG_ERROR;
                }

                //Calculate temporary Fcnt
                //Need to simplify the code for next release based on the tempFcnt Changes

                tempFcnt.members.valueLow = hdr->members.fCnt;
                if(tempFcnt.members.valueLow >= loRa.fMcastCntDown.members.valueLow)
                    tempFcnt.members.valueHigh = loRa.fMcastCntDown.members.valueHigh;
                else
                    tempFcnt.members.valueHigh = loRa.fMcastCntDown.members.valueHigh + 1;
                
                AssembleEncryptionBlock (1, tempFcnt.value, bufferLength - sizeof (computedMic), 0x49, MCAST_ENABLED);
                memcpy (&radioBuffer[0], aesBuffer, sizeof (aesBuffer));
                memcpy (&radioBuffer[16], buffer, bufferLength-sizeof(computedMic));
                AESCmac(loRa.activationParameters.mcastNetworkSessionKey, aesBuffer, &radioBuffer[0], bufferLength - sizeof(computedMic) + sizeof (aesBuffer));

                memcpy(&computedMic, aesBuffer, sizeof(computedMic));
                extractedMic = ExtractMic (&buffer[0], bufferLength);

                if (computedMic != extractedMic)
                {
                    loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                    LORAWAN_EnterContinuousReceive();
                    return MCAST_MSG_ERROR;
                }

                if (hdr->members.fCnt >= loRa.fMcastCntDown.members.valueLow)
                {
                    if ( (hdr->members.fCnt - loRa.fMcastCntDown.members.valueLow) > loRa.protocolParameters.maxMultiFcntGap )
                    {
                        if (rxPayload.RxAppData != NULL)
                        {
                            rxPayload.RxAppData (NULL, 0, MCAST_RE_KEYING_NEEDED);
                        }

                        loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                        LORAWAN_EnterContinuousReceive();
                        return MCAST_MSG_ERROR;
                    }
                    else
                    {
                        loRa.fMcastCntDown.members.valueLow = hdr->members.fCnt;  //frame counter received is OK, so the value received from the server is kept in sync with the value stored in the end device
                    }
                }
                else
                {
                    if (((UINT16_MAX - loRa.fMcastCntDown.members.valueLow) + hdr->members.fCnt) < loRa.protocolParameters.maxMultiFcntGap)
                    {
                        loRa.fMcastCntDown.members.valueLow = 0;
                        loRa.fMcastCntDown.members.valueHigh ++;
                    }
                    else
                    {
                        if (rxPayload.RxAppData != NULL)
                        {
                            rxPayload.RxAppData (NULL, 0, MCAST_RE_KEYING_NEEDED);
                        }

                        loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                        LORAWAN_EnterContinuousReceive();
                        return MCAST_MSG_ERROR;
                    }
                }

                if (loRa.fMcastCntDown.value == UINT32_MAX)
                {
                    if (rxPayload.RxAppData != NULL)
                    {
                        rxPayload.RxAppData (NULL, 0, MCAST_RE_KEYING_NEEDED);
                    }

                    loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                    LORAWAN_EnterContinuousReceive();
                    return FRAME_COUNTER_ERROR_REJOIN_NEEDED;
                }
                
                if (CLASS_C_RX2_1_OPEN == loRa.macStatus.macState)
                {
                    SwTimerStop (loRa.receiveWindow1TimerId);
                    SwTimerStop (loRa.receiveWindow2TimerId);
                }
                else if (RX1_OPEN == loRa.macStatus.macState)
                {
                    SwTimerStop (loRa.receiveWindow2TimerId);
                }

                buffer = buffer + 8; // TODO: magic number

                if ( (sizeof(extractedMic) + hdr->members.fCtrl.fOptsLen + 8) != bufferLength)     //we have port and FRM Payload in the reception packet
                {
                    fPort = *(buffer++);

                    frmPayloadLength = bufferLength - 8 - sizeof (extractedMic); //frmPayloadLength includes port
                    bufferIndex = 16 + 9;

                    EncryptFRMPayload (buffer, frmPayloadLength - 1, 1, loRa.fMcastCntDown.value, loRa.activationParameters.mcastApplicationSessionKey, bufferIndex, radioBuffer, MCAST_ENABLED);
                    packet = buffer - 1;
                }
                else
                {
                    frmPayloadLength = 0;
                    packet = NULL;
                }

                loRa.macStatus.rxDone = 1;
                loRa.macStatus.macState = IDLE;

                if (rxPayload.RxAppData != NULL)
                {
                    //loRa.lorawanMacStatus.synchronization = 0; // ?? //clear the synchronization flag, because if the user will send a packet in the callback there is no need to send an empty packet

                    rxPayload.RxAppData (packet, frmPayloadLength, MAC_OK);
                }

                loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                LORAWAN_EnterContinuousReceive();
                return MAC_OK;
            }

            //  verify if the device address stored in the activation parameters is the same with the device address piggybacked in the received packet, if not ignore packet
            if (hdr->members.devAddr.value != loRa.activationParameters.deviceAddress.value)
            {
                SetReceptionNotOkState();
                return INVALID_PARAMETER;
            }

            //Calculate temporary Fcnt
            //Need to simplify the code for next release based on the tempFcnt Changes           
            tempFcnt.members.valueLow = hdr->members.fCnt;
            if(tempFcnt.members.valueLow >= loRa.fCntDown.members.valueLow)
                tempFcnt.members.valueHigh = loRa.fCntDown.members.valueHigh;
            else
                tempFcnt.members.valueHigh = loRa.fCntDown.members.valueHigh + 1;
            
            AssembleEncryptionBlock (1, tempFcnt.value, bufferLength - sizeof (computedMic), 0x49, MCAST_DISABLED);
            memcpy (&radioBuffer[0], aesBuffer, sizeof (aesBuffer));
            memcpy (&radioBuffer[16], buffer, bufferLength-sizeof(computedMic));
            AESCmac(loRa.activationParameters.networkSessionKey, aesBuffer, &radioBuffer[0], bufferLength - sizeof(computedMic) + sizeof (aesBuffer));

            memcpy(&computedMic, aesBuffer, sizeof(computedMic));
            extractedMic = ExtractMic (&buffer[0], bufferLength);

            // verify if the computed MIC is the same with the MIC piggybacked in the packet, if not ignore packet
            if (computedMic != extractedMic)
            {
                SetReceptionNotOkState();
                return INVALID_PARAMETER;
            }

             //  frame counter check, frame counter received should be less than last frame counter received, otherwise it  was an overflow
            if (hdr->members.fCnt >= loRa.fCntDown.members.valueLow)
            {
                if ((hdr->members.fCnt - loRa.fCntDown.members.valueLow) > loRa.protocolParameters.maxFcntGap) //if this difference is greater than the value of max_fct_gap then too many data frames have been lost then subsequesnt will be discarded
                {
                    loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage = 0; // reset the flag
                    loRa.macStatus.macState = IDLE;
                    if (rxPayload.RxAppData != NULL)
                    {
                        loRa.lorawanMacStatus.synchronization = 0; //clear the synchronization flag, because if the user will send a packet in the callback there is no need to send an empty packet
                        rxPayload.RxAppData (NULL, 0, MAC_NOT_OK);
                    }
                    loRa.macStatus.rxDone = 0; 
                    
                    // Inform application about rejoin in status
                    loRa.macStatus.rejoinNeeded = 1;
                    if (CLASS_C == loRa.deviceClass)
                    {
                        loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                        LORAWAN_EnterContinuousReceive();
                    }
                    return FRAME_COUNTER_ERROR_REJOIN_NEEDED;
                }
                else
                {
                    loRa.fCntDown.members.valueLow = hdr->members.fCnt;  //frame counter received is OK, so the value received from the server is kept in sync with the value stored in the end device                    
                }                
            }
            else
            {                
                if(((65535 - loRa.fCntDown.members.valueLow) + hdr->members.fCnt) < loRa.protocolParameters.maxFcntGap)
                {
                    //Frame counter rolled over
                    loRa.fCntDown.members.valueLow = hdr->members.fCnt;
                    loRa.fCntDown.members.valueHigh ++;                    
                }
                else
                {
                    SetReceptionNotOkState();
                    //Reject packet
                    return INVALID_PARAMETER;
                }
            }
            
            rxParamSetupReqAnsFlag = false;
            rxTimingSetupReqAnsFlag = false;
            dlChannelReqAnsFlag = 0;
            dlChannelCurrentIndex = 0;

            if (loRa.fCntDown.value == UINT32_MAX)
            {

                // Inform application about rejoin in status
                loRa.macStatus.rejoinNeeded = 1;
                if (CLASS_C == loRa.deviceClass)
                {
                    loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                    LORAWAN_EnterContinuousReceive();
                }
                return FRAME_COUNTER_ERROR_REJOIN_NEEDED;
            }

            // if the downlink message was received during receive window 1, receive window 2 should not open any more, so its timer will be stopped
            if (loRa.macStatus.macState == RX1_OPEN)
            {
                SwTimerStop (loRa.receiveWindow2TimerId);
            }

            loRa.counterRepetitionsUnconfirmedUplink = 1; // this is a guard for LORAWAN_RxTimeout, for any packet that is received, the last uplink packet should not be retransmitted

            CheckFlags (hdr);

            if (hdr->members.fCtrl.fOptsLen != 0)
            {
                if(*(hdr->members.MacCommands + hdr->members.fCtrl.fOptsLen) != 0) //if not port 0
                {
                    buffer = MacExecuteCommands(hdr->members.MacCommands, hdr->members.fCtrl.fOptsLen);
                }
            }
            else
            {
                buffer = buffer + 8;  // 8 bytes for size of header without mac commands
            }
            if ( (sizeof(extractedMic) + hdr->members.fCtrl.fOptsLen + 8) != bufferLength)     //we have port and FRM Payload in the reception packet
            {
                fPort = *(buffer++);

                frmPayloadLength = bufferLength - 8 - hdr->members.fCtrl.fOptsLen - sizeof (extractedMic); //frmPayloadLength includes port
                bufferIndex = 16 + 8 + hdr->members.fCtrl.fOptsLen + sizeof (fPort);                 

                if (fPort != 0)
                {
                    EncryptFRMPayload (buffer, frmPayloadLength - 1, 1, loRa.fCntDown.value, loRa.activationParameters.applicationSessionKey, bufferIndex, radioBuffer, MCAST_DISABLED);
                    packet = buffer - 1;
                }
                else
                {
                    // 13 = sizeof(extractedMic) + hdr->members.fCtrl.fOptsLen + 8 + sizeof (fPort);
                    if(bufferLength > (HDRS_MIC_PORT_MIN_SIZE + hdr->members.fCtrl.fOptsLen))
                    {
                        // Decrypt port 0 payload
                        EncryptFRMPayload (buffer, frmPayloadLength - 1, 1, loRa.fCntDown.value, loRa.activationParameters.networkSessionKey, bufferIndex, radioBuffer, MCAST_DISABLED);
                        if(hdr->members.fCtrl.fOptsLen == 0)
                        {
                            buffer = MacExecuteCommands(buffer, frmPayloadLength - 1 );
                        }
                    }

                    frmPayloadLength = 0;  // we have only MAC commands, so no application payload
                    packet = NULL;
                }
            }
            else
            {
                frmPayloadLength = 0;
                packet = NULL;
            }

            loRa.counterRepetitionsUnconfirmedUplink = 1; // reset the counter

            loRa.adrAckCnt = 0; // if a packet comes and is correct after device address and MIC, the counter will start counting again from 0 (any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter)
            loRa.counterAdrAckDelay = 0; // if a packet was received, the counter for adr ack limit will become 0
            loRa.lorawanMacStatus.adrAckRequest = 0; //reset the flag for ADR ACK request

            loRa.macStatus.rxDone = 1;  //packet is ready for reception for the application layer

            if ( loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage == 1 ) //if last uplink packet was confirmed;
            {
                if  (hdr->members.fCtrl.ack == 1) // if ACK was received
                {
                    loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage = 0; // reset the flag
                    response = MAC_OK;
                    loRa.macStatus.macState = IDLE;
                    if (rxPayload.RxAppData != NULL)
                    {
                        loRa.lorawanMacStatus.synchronization = 0; //clear the synchronization flag, because if the user will send a packet in the callback there is no need to send an empty packet
                        rxPayload.RxAppData (packet, frmPayloadLength, response);
                    }
                    loRa.macStatus.rxDone = 0;
                    if ( (loRa.macStatus.automaticReply == 1) && (loRa.lorawanMacStatus.synchronization == 0) && ( (loRa.lorawanMacStatus.ackRequiredFromNextUplinkMessage == 1) || (loRa.lorawanMacStatus.fPending == ENABLED) ) )
                    {
                        if (SearchAvailableChannel (loRa.maxChannels, 1, &channelIndex) == OK)
                        {
                            LORAWAN_Send (0, 0, 0, 0);  // send an empty unconfirmed packet
                            loRa.lorawanMacStatus.fPending = DISABLED; //clear the fPending flag
                        }
                        else
                        {
                            //searches for the minimum channel timer and starts a software timer for it
                            StartReTxTimer();
                        }
                    }
                }
                //if ACK was required, but not received, then retransmission will happen
                else
                {
                    UpdateRetransmissionAckTimeoutState ();
                }
            }
            else
            {
                response = MAC_OK;
                loRa.macStatus.macState = IDLE;

                if (rxPayload.RxAppData != NULL)
                {
                    loRa.lorawanMacStatus.synchronization = 0; //clear the synchronization flag, because if the user will send a packet in the callback there is no need to send an empty packet
                    rxPayload.RxAppData (packet, frmPayloadLength, response);
                }
                // if the user sent a packet via the callback, the synchronization bit will be 1 and there is no need to send an empty packet
                if ( (loRa.macStatus.automaticReply == 1) && (loRa.lorawanMacStatus.synchronization == 0) && ( (loRa.lorawanMacStatus.ackRequiredFromNextUplinkMessage == 1) || (loRa.lorawanMacStatus.fPending == ENABLED) ) )
                {
                    if (SearchAvailableChannel (loRa.maxChannels, 1, &channelIndex) == OK)
                    {
                        LORAWAN_Send (0, 0, 0, 0);  // send an empty unconfirmed packet
                        loRa.lorawanMacStatus.fPending = DISABLED; //clear the fPending flag
                    }
                    else
                    {
                        //searches for the minimum channel timer and starts a software timer for it
                        StartReTxTimer();
                    }
                }
            }

            if (CLASS_C == loRa.deviceClass)
            {
                //In the case of a auto reply.. the device may be in transmit mode
                if((RADIO_GetStatus() & RADIO_FLAG_TRANSMITTING) == 0)
                {
                loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                LORAWAN_EnterContinuousReceive();
                }
            }
        }
        else
        {
            //if the mType is incorrect, set reception not OK state
            SetReceptionNotOkState ();
            return INVALID_PARAMETER;
        }
    }
    else
    {
        //Standalone radio reception OK, using the same callback as in LoRaWAN rx
        if ( rxPayload.RxAppData != NULL )
        {
            if ((RADIO_GetStatus() & RADIO_FLAG_RXERROR) == 0)
            {
                rxPayload.RxAppData(buffer, bufferLength, RADIO_OK);
            }
            else
            {
               rxPayload.RxAppData(buffer, bufferLength, RADIO_NOT_OK);
            }
        }
    }
    return 1;
}

static uint8_t LORAWAN_GetMaxPayloadSize (void)
{
    uint8_t result = 0;
    uint8_t macCommandsLength;

    macCommandsLength = CountfOptsLength();
    
    if (loRa.crtMacCmdIndex == 0)  //there are no MAC commands to either respond or interrogate
    {
         if(loRa.macStatus.uplinkDwellTimeFlag == ENABLED)
            result = maxPayloadSizeDwellLimited[loRa.currentDataRate];
         else
            result = maxPayloadSize[loRa.currentDataRate];
       
    }
    else
    {
        if(loRa.macStatus.uplinkDwellTimeFlag == ENABLED)
            result = maxPayloadSizeDwellLimited[loRa.currentDataRate] - macCommandsLength;
        else
            result = maxPayloadSize[loRa.currentDataRate] - macCommandsLength ;
        
    }

    return result;    
}

static uint8_t* MacExecuteCommands (uint8_t *buffer, uint8_t fOptsLen)
{
    bool done = false;
    uint8_t *ptr;
    ptr = buffer;
    uint8_t temp;
    while ( (ptr < ( buffer + fOptsLen )) && (done == false) )
    {
        //Clean structure before using it         
        loRa.macCommands[loRa.crtMacCmdIndex].channelMaskAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].dataRateAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].powerAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].channelAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].dataRateReceiveWindowAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].rx1DROffestAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].dataRateRangeAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].channelFrequencyAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].dlChannelFreqOkAck = 0;
        loRa.macCommands[loRa.crtMacCmdIndex].dlUplinkFreqExistsAck = 0;

        //Reply has the same value as request
        loRa.macCommands[loRa.crtMacCmdIndex].receivedCid = *ptr;
        temp = *ptr;
        
        if(((ptr+macEndDevCmdReqLen[temp-2]) > (buffer+fOptsLen)) && (temp > 0x01) && (temp < 0x0B))
        {
            loRa.macCommands[loRa.crtMacCmdIndex].receivedCid = INVALID_VALUE;
            break;
        }
        
        switch (*ptr ++)
        {
            case LINK_CHECK_CID:
            {
                ptr = ExecuteLinkCheck (ptr );
                // No reply to server is needed 
                loRa.macCommands[loRa.crtMacCmdIndex].receivedCid = INVALID_VALUE;
            } break;

            case LINK_ADR_CID:
            {
                ptr = ExecuteLinkAdr (ptr, (buffer+fOptsLen) );
            } break;

            case DUTY_CYCLE_CID:
            {
                ptr = ExecuteDutyCycle(ptr);
            } break;

            case RX_PARAM_SETUP_CID:
            {
                ptr = ExecuteRxParamSetupReq (ptr);
            } break;

            case DEV_STATUS_CID:
            {
                 ptr = ExecuteDevStatus (ptr);
            } break;

            case NEW_CHANNEL_CID:
            {
                ptr = ExecuteNewChannel (ptr);

            } break;

            case RX_TIMING_SETUP_CID:
            {
                ptr = ExecuteRxTimingSetup (ptr);
            } break;
            
            case DL_CHANNEL_CID:
            {
                ptr = ExecuteDlChannelReq(ptr);
            } break;
            
            case TX_PARAM_SETUP_CID:
            {
                ptr = ExecuteTxParamSetupReq(ptr);
            } break;
            
            default:
            {
                done = true;  // Unknown MAC commands cannot be skipped and the first unknown MAC command terminates the processing of the MAC command sequence.
                ptr = buffer + fOptsLen;
                loRa.macCommands[loRa.crtMacCmdIndex].receivedCid = INVALID_VALUE;
            } break;
        }

        if((loRa.macCommands[loRa.crtMacCmdIndex].receivedCid != INVALID_VALUE) &&
           (loRa.crtMacCmdIndex < MAX_NB_CMD_TO_PROCESS))
        {
            loRa.crtMacCmdIndex ++;
        }
    }
    return ptr;
}

static uint8_t* ExecuteLinkCheck (uint8_t *ptr)
{
   loRa.linkCheckMargin = *(ptr++);
   loRa.linkCheckGwCnt = *(ptr++);
   return ptr;
}

static uint8_t* ExecuteRxTimingSetup (uint8_t *ptr)
{
   uint8_t delay;

   delay = (*ptr) & LAST_NIBBLE;
   ptr++;

   UpdateReceiveDelays (delay);
   loRa.macStatus.rxTimingSetup = ENABLED;
   
   rxTimingSetupReqAnsFlag = true;

   return ptr;
}

static void AssemblePacket (bool confirmed, uint8_t port, uint8_t *buffer, uint16_t bufferLength)
{
    Mhdr_t mhdr;
    uint8_t bufferIndex = 16;
    FCtrl_t fCtrl;
    uint8_t macCmdIdx = 0;

    memset (&mhdr, 0, sizeof (mhdr) );    //clear the header structure Mac header
    memset (&macBuffer[0], 0, sizeof (macBuffer) ); //clear the mac buffer
    memset (aesBuffer, 0, sizeof (aesBuffer) );  //clear the transmission buffer

    if (confirmed == 1)
    {
        mhdr.bits.mType = FRAME_TYPE_DATA_CONFIRMED_UP;
        loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage = 1;
    }
    else
    {
        mhdr.bits.mType = FRAME_TYPE_DATA_UNCONFIRMED_UP;
    }
    mhdr.bits.major = 0;
    mhdr.bits.rfu = 0;
    macBuffer[bufferIndex++] = mhdr.value;

    memcpy (&macBuffer[bufferIndex], loRa.activationParameters.deviceAddress.buffer, sizeof (loRa.activationParameters.deviceAddress.buffer) );
    bufferIndex = bufferIndex + sizeof(loRa.activationParameters.deviceAddress.buffer);

    fCtrl.value = 0; //clear the fCtrl value

    if (loRa.macStatus.adr == ENABLED) 
    {
        fCtrl.adr = ENABLED; // set the ADR bit in the packet
        if(loRa.currentDataRate > loRa.minDataRate)
        {
            fCtrl.adrAckReq = ENABLED; 
            loRa.lorawanMacStatus.adrAckRequest = ENABLED; // set the flag that to remember that this bit was set
            loRa.adrAckCnt ++;   // if adr is set, each time the uplink frame counter is incremented, the device increments the adr_ack_cnt, any received downlink frame following an uplink frame resets the ADR_ACK_CNT counter

            if ( loRa.adrAckCnt == loRa.protocolParameters.adrAckLimit )
            {
                loRa.counterAdrAckDelay = 0;
                fCtrl.adrAckReq = DISABLED;
                loRa.lorawanMacStatus.adrAckRequest = DISABLED;
            }
            else
            {
                if (loRa.adrAckCnt > loRa.protocolParameters.adrAckLimit)  
                {
                    // the ADRACKREQ bit is set if ADR_ACK_CNT >= ADR_ACK_LIMIT and the current data rate is greater than the device defined minimum data rate, it is cleared in other conditions
                    loRa.counterAdrAckDelay ++ ; //we have to check how many packets we sent without any response            

                    // If no reply is received within the next ADR_ACK_DELAY uplinks, the end device may try to regain connectivity by switching to the next lower data rate    
                    if (loRa.counterAdrAckDelay >= loRa.protocolParameters.adrAckDelay)
                    {
                        loRa.counterAdrAckDelay = 0;

                        if(false == FindSmallestDataRate())
                        {
                            //Minimum data rate is reached
                            loRa.adrAckCnt = 0;                    
                            fCtrl.adrAckReq = DISABLED;
                            loRa.lorawanMacStatus.adrAckRequest = DISABLED;
                        }
                    }
                }
                else
                {
                    fCtrl.adrAckReq = DISABLED;
                    loRa.lorawanMacStatus.adrAckRequest = DISABLED;
                }
            }
        }
        else
        {
            loRa.lorawanMacStatus.adrAckRequest = DISABLED;
        }
    }

    if (loRa.lorawanMacStatus.ackRequiredFromNextUplinkMessage == ENABLED)
    {
        fCtrl.ack = ENABLED;
        loRa.lorawanMacStatus.ackRequiredFromNextUplinkMessage = DISABLED;
    }

    fCtrl.fPending = RESERVED_FOR_FUTURE_USE;  //fPending bit is ignored for uplink packets

    if ( (loRa.crtMacCmdIndex == 0) || (bufferLength == 0) ) // there is no MAC command in the queue or there are MAC commands to respond, but the packet does not include application payload (in this case the response to MAC commands will be included inside FRM payload)
    {
        fCtrl.fOptsLen = 0;         // fOpts field is absent
        if ( rxParamSetupReqAnsFlag )
        {
            fCtrl.fOptsLen += 2;
        }
         if ( rxTimingSetupReqAnsFlag )
        {
             fCtrl.fOptsLen += 1;
         }
         if ( dlChannelReqAnsFlag )
        {
             fCtrl.fOptsLen = 2 * dlChannelReqAnsFlag;
         }
    }

    else
    {
        fCtrl.fOptsLen = CountfOptsLength();
    }
    macBuffer[bufferIndex++] = fCtrl.value;

    memcpy (&macBuffer[bufferIndex], &loRa.fCntUp.members.valueLow, sizeof (loRa.fCntUp.members.valueLow) );

    bufferIndex = bufferIndex + sizeof(loRa.fCntUp.members.valueLow);

    if ( (loRa.crtMacCmdIndex != 0) && (bufferLength != 0) ) // the response to MAC commands will be included inside FOpts
    {
        IncludeMacCommandsResponse (macBuffer, &bufferIndex, 1);
    }
    else if ( loRa.crtMacCmdIndex == 0 )
    {
        if ( dlChannelReqAnsFlag )
        {
            //Always add a DlChannelReq answer until a downlink is received
            IncludeMacDlChannelResponse(macBuffer, &bufferIndex);
        }
        if ( rxParamSetupReqAnsFlag )
        {
            //Always add a RxParamSetupReq answer until a downlink is received
            IncludeMacRxParamSetupResponse(macBuffer, &bufferIndex);
        }
        if ( rxTimingSetupReqAnsFlag )
        {
		    //Always add a RxTimingSetupReq answer until a downlink is received
            IncludeMacRxTimingSetupResponse(macBuffer, &bufferIndex);
        }
     }
   macBuffer[bufferIndex++] = port;     // the port field is present if the frame payload field is not empty

   if (bufferLength != 0)
   {
        memcpy (&macBuffer[bufferIndex], buffer, bufferLength);
        EncryptFRMPayload (buffer, bufferLength, 0, loRa.fCntUp.value, loRa.activationParameters.applicationSessionKey, bufferIndex, macBuffer, MCAST_DISABLED);
        bufferIndex = bufferIndex + bufferLength;
   }
   else if ( (loRa.crtMacCmdIndex > 0) ) // if answer is needed to MAC commands, include the answer here because there is no app payload
   {
       // Use networkSessionKey for port 0 data
       //Use radioBuffer as a temporary buffer. The encrypted result is found in macBuffer
       IncludeMacCommandsResponse (radioBuffer, &macCmdIdx, 0 );
       EncryptFRMPayload (radioBuffer, macCmdIdx, 0, loRa.fCntUp.value, loRa.activationParameters.networkSessionKey, bufferIndex, macBuffer, MCAST_DISABLED);
       bufferIndex = bufferIndex + macCmdIdx;
   }

   AssembleEncryptionBlock (0, loRa.fCntUp.value, bufferIndex - 16, 0x49, MCAST_DISABLED);
   memcpy (&macBuffer[0], aesBuffer, sizeof (aesBuffer));

   AESCmac (loRa.activationParameters.networkSessionKey, aesBuffer, macBuffer, bufferIndex  );

   memcpy (&macBuffer[bufferIndex], aesBuffer, 4);
   bufferIndex = bufferIndex + 4; // 4 is the size of MIC

   loRa.lastPacketLength = bufferIndex - 16;
}

static uint8_t PrepareJoinRequestFrame (void)
{
    uint8_t bufferIndex = 0, iCtr;
    Mhdr_t mhdr;
    uint32_t mic;

    memset (macBuffer, 0, sizeof(macBuffer) );  // clear the mac buffer

    mhdr.bits.mType = FRAME_TYPE_JOIN_REQ;  //prepare the mac header to include mtype as frame type join request
    mhdr.bits.major = MAJOR_VERSION3;
    mhdr.bits.rfu = RESERVED_FOR_FUTURE_USE;

    macBuffer[bufferIndex++] = mhdr.value;  // add the mac header to the buffer

    for(iCtr = 0; iCtr < 8; iCtr ++)
    {
        macBuffer[bufferIndex + iCtr] = loRa.activationParameters.applicationEui.buffer[7 - iCtr];
    }
    bufferIndex = bufferIndex + sizeof(loRa.activationParameters.applicationEui);

    //memcpy ( &macBuffer[bufferIndex], &loRa.activationParameters.deviceEui, sizeof(loRa.activationParameters.deviceEui));
    for (iCtr = 0; iCtr < 8; iCtr ++)
    {
        macBuffer[bufferIndex + iCtr] = loRa.activationParameters.deviceEui.buffer[7 - iCtr];
    }
    bufferIndex = bufferIndex + sizeof( loRa.activationParameters.deviceEui );

    loRa.devNonce = Random (UINT16_MAX);
    memcpy (&macBuffer[bufferIndex], &loRa.devNonce, sizeof (loRa.devNonce) );
    bufferIndex = bufferIndex + sizeof( loRa.devNonce );

    mic = ComputeMic (loRa.activationParameters.applicationKey, macBuffer, bufferIndex);

    memcpy ( &macBuffer[bufferIndex], &mic, sizeof (mic));
    bufferIndex = bufferIndex + sizeof(mic);

    return bufferIndex;
}

static void IncludeMacRxParamSetupResponse (uint8_t* macBuffer, uint8_t* pBufferIndex)
{
    uint8_t bufferIndex = *pBufferIndex;
    
    macBuffer[bufferIndex++] = RX_PARAM_SETUP_CID;
    macBuffer[bufferIndex] = rxParamSetupAns;

    bufferIndex ++;
    *pBufferIndex = bufferIndex;
}

static void IncludeMacRxTimingSetupResponse (uint8_t* macBuffer, uint8_t* pBufferIndex)
{
    uint8_t bufferIndex = *pBufferIndex;
    
    macBuffer[bufferIndex++] = RX_TIMING_SETUP_CID;

    *pBufferIndex = bufferIndex;
}

static void IncludeMacDlChannelResponse (uint8_t* macBuffer, uint8_t* pBufferIndex)
{
    uint8_t bufferIndex = *pBufferIndex;
    uint8_t dlResponse;
    
    for(dlResponse = 0; dlResponse < dlChannelReqAnsFlag; dlResponse++)
    {
        macBuffer[bufferIndex++] = DL_CHANNEL_CID;
        macBuffer[bufferIndex++] = dlChannelAns[dlResponse];
    }

    *pBufferIndex = bufferIndex;
}

static void IncludeMacCommandsResponse (uint8_t* macBuffer, uint8_t* pBufferIndex, uint8_t bIncludeInFopts )
{
    uint8_t i = 0;
    uint8_t bufferIndex = *pBufferIndex;
    
    for(i = 0; i < loRa.crtMacCmdIndex ; i++)
    {        
        if((bIncludeInFopts) && (loRa.macCommands[i].receivedCid != INVALID_VALUE))
        {
            if((bufferIndex - (*pBufferIndex) + macEndDevCmdReplyLen[loRa.macCommands[i].receivedCid - 2]) > MAX_FOPTS_LEN)
            {
                break;
            }
        }
        switch (loRa.macCommands[i].receivedCid)
        {
            case LINK_ADR_CID:
            {
                macBuffer[bufferIndex++] = LINK_ADR_CID;
                macBuffer[bufferIndex] = 0x00;
                if (loRa.macCommands[i].channelMaskAck == 1)
                {
                    macBuffer[bufferIndex] |= CHANNEL_MASK_ACK;
                }

                if (loRa.macCommands[i].dataRateAck == 1)
                {
                    macBuffer[bufferIndex] |= DATA_RATE_ACK;
                }

                if (loRa.macCommands[i].powerAck == 1)
                {
                    macBuffer[bufferIndex] |= POWER_ACK;
                }
                bufferIndex ++;            
            }
            break;

            case RX_PARAM_SETUP_CID:
            {
                macBuffer[bufferIndex++] = RX_PARAM_SETUP_CID;
                macBuffer[bufferIndex] = 0x00;
                if (loRa.macCommands[i].channelAck == 1)
                {
                    macBuffer[bufferIndex] |= CHANNEL_MASK_ACK;
                }

                if (loRa.macCommands[i].dataRateReceiveWindowAck == 1)
                {
                    macBuffer[bufferIndex] |= DATA_RATE_ACK;
                }
                
                if (loRa.macCommands[i].rx1DROffestAck == 1)
                {
                    macBuffer[bufferIndex] |= RX1_DR_OFFSET_ACK;
                }    
                
                rxParamSetupAns = macBuffer[bufferIndex];               
                bufferIndex ++;
            }
            break;    

            case DEV_STATUS_CID:
            {
                macBuffer[bufferIndex++] = DEV_STATUS_CID;
                macBuffer[bufferIndex++] = loRa.batteryLevel;
                if ((RADIO_GetPacketSnr() < -32) || (RADIO_GetPacketSnr() > 31))
                {
                    macBuffer[bufferIndex++] = 0x20;  //if the value returned by the radio is out of range, send the minimum (-32)
                }
                else
                {
                    macBuffer[bufferIndex++] = ((uint8_t)RADIO_GetPacketSnr() & 0x3F);  //bits 7 and 6 are RFU, bits 5-0 are the margin level response;
                }
            }
            break;    

            case NEW_CHANNEL_CID:
            {
                macBuffer[bufferIndex++] = NEW_CHANNEL_CID;
                macBuffer[bufferIndex] = 0x00;
                if (loRa.macCommands[i].channelFrequencyAck == 1)
                {
                    macBuffer[bufferIndex] |= CHANNEL_MASK_ACK;
                }

                if (loRa.macCommands[i].dataRateRangeAck == 1)
                {
                    macBuffer[bufferIndex] |= DATA_RATE_ACK;
                }
                bufferIndex ++;
            }
            break;
            
            case LINK_CHECK_CID:
            {
                loRa.linkCheckMargin = 255; // reserved
                loRa.linkCheckGwCnt = 0;
                macBuffer[bufferIndex++] = loRa.macCommands[i].receivedCid;
            }
            break;                 

            case RX_TIMING_SETUP_CID: 
            {
                macBuffer[bufferIndex++] = loRa.macCommands[i].receivedCid;
            }
            break;
            
            case DUTY_CYCLE_CID:
            {
                macBuffer[bufferIndex++] = loRa.macCommands[i].receivedCid;
            }
            break;          
            
            case DL_CHANNEL_CID:
            {
                macBuffer[bufferIndex++] = DL_CHANNEL_CID;
                macBuffer[bufferIndex] = 0x00;
                if (loRa.macCommands[i].dlChannelFreqOkAck == 1)
                {
                    macBuffer[bufferIndex] |= CHANNEL_MASK_FREQ_OK;
                }

                if (loRa.macCommands[i].dlUplinkFreqExistsAck == 1)
                {
                    macBuffer[bufferIndex] |= CHANNEL_MASK_UP_FREQ_EXISTS;
                }
                dlChannelAns[dlChannelCurrentIndex++] = macBuffer[bufferIndex];
                bufferIndex ++;
            }
            break;
            
            case TX_PARAM_SETUP_CID:
            {
                macBuffer[bufferIndex++] = loRa.macCommands[i].receivedCid;            
            }
            break;     
            
            default:
                //CID = 0xFF
            break;
        }
    }
    
    *pBufferIndex = bufferIndex;
}

// This function checks which value can be assigned to the current data rate.
static bool FindSmallestDataRate (void)
{
    uint8_t  i = 0, dataRate;
    bool found = false;

    if (loRa.currentDataRate > loRa.minDataRate)
    {
        dataRate = loRa.currentDataRate - 1;

        while ( (found == false) && (dataRate >= loRa.minDataRate) )
        {
            for ( i = 0; i < loRa.maxChannels; i++ )
            {
                if ( (dataRate >= Channels[i].dataRange.min) && (dataRate <= Channels[i].dataRange.max ) && ( Channels[i].status == ENABLED ) )
                {
                    found = true;
                    break;
                }
            }
            if ( (found == false) &&  (dataRate > loRa.minDataRate) ) // if no channels were found after one search, then the device will switch to the next lower data rate possible
            {
                dataRate = dataRate - 1;
            }
        }

        if (found == true)
        {
            loRa.currentDataRate = dataRate;
        }
    }
    
    return found;
}

static void SetReceptionNotOkState (void)
{
    if ( (loRa.macStatus.macState == RX2_OPEN) || ( (loRa.macStatus.macState == RX1_OPEN) && (loRa.rx2DelayExpired) ) )
    {
        loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage = 0; // reset the flag
        loRa.macStatus.macState = IDLE;
        
        if ((loRa.deviceClass == CLASS_A) && (rxPayload.RxAppData != NULL))
        {
            loRa.lorawanMacStatus.synchronization = 0; //clear the synchronization flag, because if the user will send a packet in the callback there is no need to send an empty packet
            rxPayload.RxAppData (NULL, 0, MAC_NOT_OK);
        }
        loRa.macStatus.rxDone = 0;
    }

    if (loRa.deviceClass == CLASS_C)
    {
        loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
        LORAWAN_EnterContinuousReceive();           
    }
}

static void ConfigureRadioRx(uint8_t dataRate, uint32_t freq)
{
    ConfigureRadio(dataRate, freq);
    RADIO_SetCRC(DISABLED);
    RADIO_SetIQInverted(ENABLED); 
}

static void UpdateReceiveDelays (uint8_t delay)
{
    loRa.protocolParameters.receiveDelay1 = 1000 * delay ;
    if (delay == 0)
    {
        loRa.protocolParameters.receiveDelay1 = 1000;
    }

    loRa.protocolParameters.receiveDelay2 = loRa.protocolParameters.receiveDelay1 + 1000;
}

static void UpdateJoinInProgress(uint8_t state)
{
    // set the states and flags accordingly
    loRa.macStatus.networkJoined = 0; //last join (if any) is not considered any more, a new join is requested
    loRa.lorawanMacStatus.joining = ENABLED;
    loRa.macStatus.macState = state;
}

static void PrepareSessionKeys (uint8_t* sessionKey, uint8_t* appNonce, uint8_t* networkId)
{
    uint8_t index = 0;

    memset (&sessionKey[index], 0, sizeof(aesBuffer));  //appends 0-es to the buffer so that pad16 is done
    index ++; // 1 byte for 0x01 or 0x02 depending on the key type

    memcpy(&sessionKey[index], appNonce, JA_APP_NONCE_SIZE);
    index = index + JA_APP_NONCE_SIZE;

    memcpy(&sessionKey[index], networkId, JA_NET_ID_SIZE);
    index = index + JA_NET_ID_SIZE;

    memcpy(&sessionKey[index], &loRa.devNonce, sizeof(loRa.devNonce) );

}

static void ComputeSessionKeys (JoinAccept_t *joinAcceptBuffer)
{
    PrepareSessionKeys(loRa.activationParameters.applicationSessionKey, joinAcceptBuffer->members.appNonce, joinAcceptBuffer->members.networkId);
    loRa.activationParameters.applicationSessionKey[0] = 0x02; // used for Network Session Key
    AESEncodeLoRa(loRa.activationParameters.applicationSessionKey, loRa.activationParameters.applicationKey);

    PrepareSessionKeys(loRa.activationParameters.networkSessionKey, joinAcceptBuffer->members.appNonce, joinAcceptBuffer->members.networkId);
    loRa.activationParameters.networkSessionKey[0] = 0x01; // used for Network Session Key
    AESEncodeLoRa(loRa.activationParameters.networkSessionKey, loRa.activationParameters.applicationKey);
}

//Based on the last packet received, this function checks the flags and updates the state accordingly
static void CheckFlags (Hdr_t* hdr)
{
    if (hdr->members.fCtrl.adr == ENABLED)
    {
        loRa.macStatus.adr = ENABLED;
    }

    if (hdr->members.fCtrl.fPending == ENABLED)
    {
        loRa.lorawanMacStatus.fPending = ENABLED;
    }

    if (hdr->members.fCtrl.adrAckReq == ENABLED)
    {
        loRa.lorawanMacStatus.adrAckRequest = ENABLED;
    }

    if (hdr->members.mhdr.bits.mType == FRAME_TYPE_DATA_CONFIRMED_DOWN)
    {
        loRa.lorawanMacStatus.ackRequiredFromNextUplinkMessage = ENABLED;  //next uplink packet should have the ACK bit set
    }
}

static uint8_t CheckMcastFlags (Hdr_t* hdr)
{

    /*
     * Multicast message conditions:
     * - ACK == 0
     * - ADRACKReq == 0
     * - FOpt == empty => FOptLen == 0 (no mac commands in fopt)
     * - FPort == 0 (no commands in payload)
     * - MType == UNCNF Data downlink
     */

    if ((0 != hdr->members.fCtrl.ack) || (0 != hdr->members.fCtrl.adrAckReq) || (FRAME_TYPE_DATA_UNCONFIRMED_DOWN != hdr->members.mhdr.bits.mType))
    {
        return FLAG_ERROR;
    }

    if (0 != hdr->members.fCtrl.fOptsLen)
    {
        return FLAG_ERROR;
    }
    else
    {
        if ( 0 == *(((uint8_t *)hdr) + 8)) // Port vlaue in case of FOpts empty
        {
            return FLAG_ERROR;
        }
    }

    if (hdr->members.fCtrl.fPending == ENABLED)
    {
        loRa.lorawanMacStatus.fPending = ENABLED;
    }

    return FLAG_OK;
}

static uint8_t CountfOptsLength (void)
{
    uint8_t i, macCommandLength=0;

    for (i = 0; i < loRa.crtMacCmdIndex; i++)
    {
        if(loRa.macCommands[i].receivedCid != INVALID_VALUE)
        {
            if((macCommandLength + macEndDevCmdReplyLen[loRa.macCommands[i].receivedCid - 2]) <= MAX_FOPTS_LEN)
            {
                macCommandLength += macEndDevCmdReplyLen[loRa.macCommands[i].receivedCid - 2];
            }
            else
            {
                break;
            }
        }
    }
    
    return macCommandLength;
}

static void AssembleEncryptionBlock (uint8_t dir, uint32_t frameCounter, uint8_t blockId, uint8_t firstByte, uint8_t multicastStatus)
{
    uint8_t bufferIndex = 0;

    memset (aesBuffer, 0, sizeof (aesBuffer)); //clear the aesBuffer

    aesBuffer[bufferIndex] = firstByte;

    bufferIndex = bufferIndex + 5;  // 4 bytes of 0x00 (done with memset at the beginning of the function)

    aesBuffer[bufferIndex++] = dir;

    if (DISABLED == multicastStatus)
    {
        memcpy (&aesBuffer[bufferIndex], &loRa.activationParameters.deviceAddress, sizeof (loRa.activationParameters.deviceAddress));
        bufferIndex = bufferIndex + sizeof (loRa.activationParameters.deviceAddress);
    }
    else
    {
        memcpy (&aesBuffer[bufferIndex], &loRa.activationParameters.mcastDeviceAddress, sizeof (loRa.activationParameters.mcastDeviceAddress));
        bufferIndex = bufferIndex + sizeof (loRa.activationParameters.mcastDeviceAddress);
    }

    memcpy (&aesBuffer[bufferIndex], &frameCounter, sizeof (frameCounter));
    bufferIndex = bufferIndex + sizeof (frameCounter) ;

    bufferIndex ++;   // 1 byte of 0x00 (done with memset at the beginning of the function)

    aesBuffer[bufferIndex] = blockId;
}

static uint32_t ExtractMic (uint8_t *buffer, uint8_t bufferLength)
{
     uint32_t mic = 0;
     memcpy (&mic, &buffer[bufferLength - 4], sizeof (mic));
     return mic;
}

static uint32_t ComputeMic ( uint8_t *key, uint8_t* buffer, uint8_t bufferLength)  // micType is 0 for join request and 1 for data packet
{
    uint32_t mic = 0;

    AESCmac(key, aesBuffer, buffer, bufferLength); //if micType is 0, bufferLength the same, but for data messages bufferLength increases with 16 because a block is added

    memcpy(&mic, aesBuffer, sizeof( mic ));

    return mic;
}

static void EncryptFRMPayload (uint8_t* buffer, uint8_t bufferLength, uint8_t dir, uint32_t frameCounter, uint8_t* key, uint8_t macBufferIndex, uint8_t* bufferToBeEncrypted, uint8_t multicastStatus)
{
    uint8_t k = 0, i = 0, j = 0;

    k = bufferLength / AES_BLOCKSIZE;
    bufferToBeEncrypted += macBufferIndex;
    for (i = 1; i <= k; i++)
    {
        AssembleEncryptionBlock (dir, frameCounter, i, 0x01, multicastStatus);
        AESEncodeLoRa(aesBuffer, key);

        for (j = 0; j < AES_BLOCKSIZE; j++)
        {
            *bufferToBeEncrypted++ = aesBuffer[j] ^ buffer[AES_BLOCKSIZE*(i-1) + j];
        }
    }

    if ( (bufferLength % AES_BLOCKSIZE) != 0 )
    {
        AssembleEncryptionBlock (dir, frameCounter, i, 0x01, multicastStatus);
        AESEncodeLoRa (aesBuffer, key);

        for (j = 0; j < (bufferLength % AES_BLOCKSIZE); j++)
        {
            *bufferToBeEncrypted++ = aesBuffer[j] ^ buffer[(AES_BLOCKSIZE*k) + j];
        }
    }
}

uint8_t LORAWAN_GetState(void)
{
    return loRa.macStatus.macState;
}

void LORAWAN_Mainloop (void)
{
    SwTimersExecute();

    //Execute radio interrupts
    localDioStatus = INTERRUPT_GetDioStatus();

    if ((localDioStatus & DIO0) != 0)
    {
        RADIO_DIO0();
    }
    if ((localDioStatus & DIO1) != 0)
    {
        RADIO_DIO1();
    }
    if ((localDioStatus & DIO2) != 0)
    {
        RADIO_DIO2();
    }
    if ((localDioStatus & DIO3) != 0)
    {
        RADIO_DIO3();
    }
    if ((localDioStatus & DIO4) != 0)
    {
        RADIO_DIO4();
    }
    if ((localDioStatus & DIO5) != 0)
    {
        RADIO_DIO5();
    }
}
