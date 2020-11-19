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
*                           lorawan_eu.c
*
* LoRaWAN EU file
*
******************************************************************************/

/****************************** INCLUDES **************************************/
#include <math.h>
#include <stdbool.h>
#include "lorawan.h"
#include "lorawan_aes.h"
#include "lorawan_aes_cmac.h"
#include "lorawan_private.h"
#include "lorawan_defs.h"
#include "AES.h"
#include "radio_interface.h"
#include "sw_timer.h"
#include "lorawan_eu.h"


/****************************** VARIABLES *************************************/
const uint8_t rxWindowSize[] =  {8, 10, 14, 11, 13, 18, 28, 15};

// Max Payload Size 
const uint8_t maxPayloadSize[8] = {51, 51, 51, 115, 242, 242, 242, 56}; // for FSK max message size should be 64 bytes

// Channels by ism band
ChannelParams_t Channels[MAX_EU_SINGLE_BAND_CHANNELS];

static const int8_t rxWindowOffset[] = {-33, -50, -58, -6, -6, -6, -6, -6};

// Tx power possibilities by ism band
static const int8_t txPower868[] = {16, 14, 12, 10, 8, 6, 4, 2};

static const int8_t txPower433[] = {10, 7, 4, 1, -2, -5};

// Spreading factor possibilities 
static const uint8_t spreadingFactor[] = {12, 11, 10, 9, 8, 7, 7};

// Bandwidth possibilities 
static const uint8_t bandwidth[] = {BW_125KHZ, BW_125KHZ, BW_125KHZ, BW_125KHZ, BW_125KHZ, BW_125KHZ, BW_250KHZ};

// Modulation possibilities 
static const uint8_t modulation[] = {MODULATION_LORA, MODULATION_LORA, MODULATION_LORA, MODULATION_LORA, MODULATION_LORA, MODULATION_LORA, MODULATION_LORA, MODULATION_FSK};

static const ChannelParams_t DefaultChannels868[] = {
LC0_868,
LC1_868,
LC2_868,
};

static const ChannelParams_t DefaultChannels433[] = {
LC0_433,
LC1_433,
LC2_433,
};

static const uint8_t FskSyncWordBuff[3] = {0xC1, 0x94, 0xC1};

LoRa_t loRa;
bool rxParamSetupReqAnsFlag = false;
bool rxTimingSetupReqAnsFlag = false;
bool dlChannelReqAnsFlag = false;

uint8_t rxParamSetupAns = 0;
uint8_t dlChannelAns = 0;

/************************ PRIVATE FUNCTION PROTOTYPES *************************/

static void CreateAllSoftwareTimers (void);

static void SetCallbackSoftwareTimers (void);

static void StopAllSoftwareTimers (void);

static void InitDefault868Channels (void);

static void InitDefault433Channels (void);

static void UpdateDataRange (uint8_t channelId, uint8_t dataRangeNew);

static void UpdateChannelIdStatus (uint8_t channelId, bool statusNew);

static LorawanError_t ValidateRxOffset (uint8_t rxOffset);

static LorawanError_t ValidateFrequency (uint32_t frequencyNew);

static LorawanError_t ValidateDataRange (uint8_t dataRangeNew);

static LorawanError_t ValidateChannelId (uint8_t channelId, bool allowedForDefaultChannels);

static LorawanError_t ValidateDlChannelId (uint8_t channelId);

static LorawanError_t ValidateChannelMaskCntl (uint8_t channelMaskCntl);

static void EnableChannels (uint16_t channelMask, uint8_t channelMaskCntl);

static void UpdateFrequency (uint8_t channelId, uint32_t frequencyNew );

static void UpdateDutyCycle (uint8_t channelId, uint16_t dutyCycleNew);

static LorawanError_t ValidateChannelMask (uint16_t channelMask);

static void EnableChannels1 (uint16_t channelMask, uint8_t channelMaskCntl, uint8_t channelIndexMin,  uint8_t channelIndexMax);

static void DutyCycleCallback (uint8_t param);

static void OTAAJoinTimerCallback (uint8_t param);

static void ConfigureRadioTx(uint8_t dataRate, uint32_t freq);

/****************************** FUNCTIONS *************************************/

void LORAWAN_Init(RxAppDataCb_t RxPayload, RxJoinResponseCb_t RxJoinResponse) // this function resets everything to the default values
{
    // Allocate software timers and their callbacks
    if (loRa.macInitialized == DISABLED)
    {
        CreateAllSoftwareTimers ();
        SetCallbackSoftwareTimers ();
        loRa.macInitialized = ENABLED;
    }
    else
    {
        StopAllSoftwareTimers ();
    }

    rxPayload.RxAppData = RxPayload;
    rxPayload.RxJoinResponse = RxJoinResponse;

    RADIO_Init(&radioBuffer[16], EU868_CALIBRATION_FREQ);

    srand (RADIO_ReadRandom ());  // for the loRa random function we need a seed that is obtained from the radio

    LORAWAN_Reset (ISM_EU868);
}

void LORAWAN_Reset (IsmBand_t ismBandNew)
{
    if (loRa.macInitialized == ENABLED)
    {
        StopAllSoftwareTimers ();
    }

    loRa.syncWord = 0x34;
    RADIO_SetLoRaSyncWord(loRa.syncWord);

    loRa.macStatus.value = 0;
    loRa.linkCheckMargin = 255; // reserved
    loRa.linkCheckGwCnt = 0;
    loRa.lastTimerValue = 0;
    loRa.lastPacketLength = 0;
    loRa.fCntDown.value = 0;
    loRa.fCntUp.value = 0;
    loRa.devNonce = 0;
    loRa.prescaler = 1;
    loRa.adrAckCnt = 0;
    loRa.counterAdrAckDelay = 0;
    loRa.offset = 0; 
    loRa.lastTimerValue = 0;
    loRa.otaaMinuteCount = 0;
    loRa.otaaHourCount = 0;

    // link check mechanism should be disabled
    loRa.macStatus.linkCheck = DISABLED;

    //flags all 0-es
    loRa.macStatus.value = 0;
    loRa.lorawanMacStatus.value = 0;

    loRa.maxRepetitionsConfirmedUplink = 7; // 7 retransmissions should occur for each confirmed frame sent until ACK is received
    loRa.maxRepetitionsUnconfirmedUplink = 0; // 0 retransmissions should occur for each unconfirmed frame sent until a response is received
    loRa.counterRepetitionsConfirmedUplink = 1;
    loRa.counterRepetitionsUnconfirmedUplink = 1;

    loRa.batteryLevel = BATTERY_LEVEL_INVALID; // the end device was not able to measure the battery level

    loRa.ismBand = ismBandNew;

    loRa.deviceClass = CLASS_A;
    // initialize default channels
    loRa.maxChannels = MAX_EU_SINGLE_BAND_CHANNELS; 
    if(ISM_EU868 == ismBandNew)
    {
        RADIO_Init(&radioBuffer[16], EU868_CALIBRATION_FREQ);

        InitDefault868Channels ();     
        
        loRa.receiveWindow2Parameters.dataRate = EU868_DEFAULT_RX_WINDOW2_DR;
        loRa.receiveWindow2Parameters.frequency = EU868_DEFAULT_RX_WINDOW2_FREQ; 
        SwTimerSetTimeout(loRa.otaaJoinTimerId, MS_TO_TICKS_SHORT(OTAA_MINUTECOUNT_MS));
        SwTimerStart(loRa.otaaJoinTimerId);
    }
    else
    {
        RADIO_Init(&radioBuffer[16], EU433_CALIBRATION_FREQ);

        InitDefault433Channels ();  
        
        loRa.receiveWindow2Parameters.dataRate = EU433_DEFAULT_RX_WINDOW2_DR;
        loRa.receiveWindow2Parameters.frequency = EU433_DEFAULT_RX_WINDOW2_FREQ;          
    } 
    
    loRa.txPower = 1; 

    loRa.currentDataRate = DR0;
	
    UpdateMinMaxChDataRate ();

    //keys will be filled with 0
    loRa.macKeys.value = 0;  //no keys are set
    memset (&loRa.activationParameters, 0, sizeof(loRa.activationParameters));

    //protocol parameters receive the default values
    loRa.protocolParameters.receiveDelay1 = RECEIVE_DELAY1;
    loRa.protocolParameters.receiveDelay2 = RECEIVE_DELAY2;
    loRa.protocolParameters.joinAcceptDelay1 = JOIN_ACCEPT_DELAY1;
    loRa.protocolParameters.joinAcceptDelay2 = JOIN_ACCEPT_DELAY2;
    loRa.protocolParameters.ackTimeout = ACK_TIMEOUT;
    loRa.protocolParameters.adrAckDelay = ADR_ACK_DELAY;
    loRa.protocolParameters.adrAckLimit = ADR_ACK_LIMIT;
    loRa.protocolParameters.maxFcntGap = MAX_FCNT_GAP;
    loRa.protocolParameters.maxMultiFcntGap = MAX_MCAST_FCNT_GAP;
    loRa.rxDnFrame = false;

    LORAWAN_LinkCheckConfigure (DISABLED); // disable the link check mechanism
}

LorawanError_t LORAWAN_SetReceiveWindow2Parameters (uint32_t frequency, uint8_t dataRate)
{
    LorawanError_t result = OK;

    if ( (ValidateFrequency (frequency) == OK) && (ValidateDataRate (dataRate) == OK) )
    {
        UpdateReceiveWindow2Parameters (frequency, dataRate);
    }
    else
    {
        result = INVALID_PARAMETER;
    }
    return result;
}

uint32_t LORAWAN_GetFrequency (uint8_t channelId)
{
   return Channels[channelId].frequency;
}

LorawanError_t LORAWAN_SetDataRange (uint8_t channelId, uint8_t dataRangeNew)
{
    LorawanError_t result = OK;

    if ( (ValidateChannelId (channelId, ALL_CHANNELS) != OK) || (ValidateDataRange (dataRangeNew) != OK) )
    {
        result = INVALID_PARAMETER;
    }
    else
    {
       UpdateDataRange (channelId, dataRangeNew);
    }

    return result;
}

uint8_t LORAWAN_GetDataRange (uint8_t channelId)
{
   uint8_t result = 0xFF;

    if (ValidateChannelId (channelId, ALL_CHANNELS) == OK)
    {
        result = Channels[channelId].dataRange.value;
    }
    return result;
}

LorawanError_t LORAWAN_SetChannelIdStatus (uint8_t channelId, bool statusNew)
{
    LorawanError_t result = OK;


    if (ValidateChannelId (channelId, ALL_CHANNELS) != OK)
    {
        result = INVALID_PARAMETER;
    }

    else
    {
        if ( (Channels[channelId].parametersDefined & (FREQUENCY_DEFINED | DATA_RANGE_DEFINED | DUTY_CYCLE_DEFINED) ) == (FREQUENCY_DEFINED | DATA_RANGE_DEFINED | DUTY_CYCLE_DEFINED) )
        {
            UpdateChannelIdStatus (channelId, statusNew);
        }
        else 
        {
            result = INVALID_PARAMETER;
        }
    }

    return result;
}

bool LORAWAN_GetChannelIdStatus (uint8_t channelId)
{
    bool result = DISABLED;

    if (ValidateChannelId (channelId, ALL_CHANNELS) == OK)
    {
        result = Channels[channelId].status;
    }
    return result;
}

LorawanError_t LORAWAN_SetFrequency (uint8_t channelId, uint32_t frequencyNew)
{
    LorawanError_t result = OK;  

    if ( (ValidateChannelId (channelId, 0) != OK) || (ValidateFrequency (frequencyNew) != OK) )
    {
        return INVALID_PARAMETER;
    }

    UpdateFrequency (channelId, frequencyNew);

    return result;
}

LorawanError_t LORAWAN_SetDutyCycle (uint8_t channelId, uint16_t dutyCycleValue)
{
    LorawanError_t result = OK;

    if (ValidateChannelId (channelId, ALL_CHANNELS) == OK)
    {
        UpdateDutyCycle (channelId, dutyCycleValue);
    }
    else
    {
        result = INVALID_PARAMETER;
    }
    
    return result;
}

uint16_t LORAWAN_GetDutyCycle (uint8_t channelId)
{
    uint16_t result = UINT16_MAX;

    if (ValidateChannelId (channelId, ALL_CHANNELS) == OK)
    {
        result = Channels[channelId].dutyCycle;
    }

    return result;
}

uint8_t LORAWAN_GetIsmBand(void)                        //returns the ISM band
{
    return loRa.ismBand;
}

void LORAWAN_TxDone(uint16_t timeOnAir)
{
    if (loRa.macStatus.macPause == DISABLED)
    {
        bool found = false;
        uint8_t i;
        uint32_t delta = 0, minim = UINT32_MAX, ticks;       

        //This flag is used when the reception in RX1 is overlapping the opening of RX2
        loRa.rx2DelayExpired = 0;

        loRa.macStatus.macState = BEFORE_RX1;

        i = loRa.lastUsedChannelIndex;    

       // the join request should never exceed 0.1%
       if (loRa.lorawanMacStatus.joining == 1)
       {           
            SwTimerSetTimeout(loRa.joinAccept1TimerId, MS_TO_TICKS_SHORT(loRa.protocolParameters.joinAcceptDelay1 + rxWindowOffset[loRa.receiveWindow1Parameters.dataRate]));
            SwTimerSetTimeout(loRa.joinAccept2TimerId, MS_TO_TICKS_SHORT(loRa.protocolParameters.joinAcceptDelay2 + rxWindowOffset[loRa.receiveWindow2Parameters.dataRate]));
            SwTimerStart(loRa.joinAccept1TimerId);
            SwTimerStart(loRa.joinAccept2TimerId);

            if(Channels[i].dutyCycle != 0)
            {
                if(loRa.otaaHourCount == 0)
                    Channels[i].channelTimer = ((uint32_t)timeOnAir) * (((uint32_t)DUTY_CYCLE_JOIN_1HOUR + 1) * ((uint32_t)loRa.prescaler) - 1);
                else if(loRa.otaaHourCount < 11)
                    Channels[i].channelTimer = ((uint32_t)timeOnAir) * (((uint32_t)DUTY_CYCLE_JOIN_11HOUR + 1) * ((uint32_t)loRa.prescaler) - 1);
                else
                    Channels[i].channelTimer = ((uint32_t)timeOnAir) * (((uint32_t)DUTY_CYCLE_JOIN_REQUEST + 1) * ((uint32_t)loRa.prescaler) - 1); 
            }
            else
            {
                Channels[i].channelTimer = ((uint32_t)timeOnAir) * (((uint32_t)Channels[i].dutyCycle + 1) * ((uint32_t)loRa.prescaler) - 1);
            }
       }
       else
       {
            SwTimerSetTimeout(loRa.receiveWindow1TimerId, MS_TO_TICKS_SHORT(loRa.protocolParameters.receiveDelay1 + rxWindowOffset[loRa.receiveWindow1Parameters.dataRate]));
            SwTimerSetTimeout(loRa.receiveWindow2TimerId, MS_TO_TICKS_SHORT(loRa.protocolParameters.receiveDelay2 + rxWindowOffset[loRa.receiveWindow2Parameters.dataRate]));
            SwTimerStart(loRa.receiveWindow1TimerId);
            SwTimerStart(loRa.receiveWindow2TimerId);
            

            Channels[i].channelTimer = ((uint32_t)timeOnAir) * (((uint32_t)Channels[i].dutyCycle + 1) * ((uint32_t)loRa.prescaler) - 1);
        }

       if(SwTimerIsRunning(loRa.dutyCycleTimerId))
       {
           SwTimerStop(loRa.dutyCycleTimerId);

           ticks = SwTimerReadValue (loRa.dutyCycleTimerId);
           delta = loRa.lastTimerValue - TICKS_TO_MS(ticks);
       }

        for (i=0; i < MAX_EU_SINGLE_BAND_CHANNELS; i++)
        {
            if ((Channels[i].status == ENABLED) && ( Channels[i].channelTimer != 0 ))
            {
                if( i != loRa.lastUsedChannelIndex ) {
                    if (Channels[i].channelTimer > delta)
                    {
                        Channels[i].channelTimer = Channels[i].channelTimer - delta;
                    }
                    else
                    {
                        Channels[i].channelTimer = 0;
                    }
                }

                if ( (Channels[i].channelTimer <= minim) && (Channels[i].channelTimer !=0) )
                {
                    minim  = Channels[i].channelTimer;
                    found = true;
                }
            }
        }
       if (found == true)
       {
           loRa.lastTimerValue = minim;
           SwTimerSetTimeout (loRa.dutyCycleTimerId, MS_TO_TICKS (minim));
           SwTimerStart (loRa.dutyCycleTimerId);
       }

       if (CLASS_C == loRa.deviceClass)
       {
           loRa.macStatus.macState = CLASS_C_RX2_1_OPEN;
           LORAWAN_EnterContinuousReceive();
       }
    }
    else
    {
        if ((RADIO_GetStatus() & RADIO_FLAG_TIMEOUT) != 0)
        {
            // Radio transmission ended by watchdog timer
            if ( rxPayload.RxAppData != NULL )
            {
                rxPayload.RxAppData( NULL, 0, RADIO_NOT_OK );
            }
        }
        else
        {
            //Standalone radio transmissions finished OK, using the same callback as in LoRaWAN tx
            if ( rxPayload.RxAppData != NULL )
            {
                rxPayload.RxAppData( NULL, 0, RADIO_OK );
            }
        }
    }
}

// this function is called by the radio when the first or the second receive window expired without receiving any message (either for join accept or for message)
void LORAWAN_RxTimeout(void)
{
    uint8_t i;
    uint32_t minim = UINT32_MAX;

    if (loRa.macStatus.macPause == 0)
    {
        // if the timeout is after the first receive window, we have to wait for the second receive window....
        if ( loRa.macStatus.macState == RX1_OPEN )
        {
                loRa.macStatus.macState = BETWEEN_RX1_RX2;
                if ((loRa.macStatus.networkJoined == 1) && (loRa.deviceClass == CLASS_C))
                {
                    if (loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage != ENABLED)
                    {
                        if (loRa.counterRepetitionsUnconfirmedUplink > loRa.maxRepetitionsUnconfirmedUplink)
                        {
                            if(loRa.lastPacketLength > 0x0D)
                            {
                                ResetParametersForUnconfirmedTransmission ();
                                if (rxPayload.RxAppData != NULL)
                                {
                                    rxPayload.RxAppData(NULL, 0, MAC_OK);  // inform the application layer that no message was received back from the server
                                }
                            }
                        }
                    }
                }
   
        }
        else
        {
        // if last message sent was a join request, the join was not accepted after the second window expired
            if (loRa.lorawanMacStatus.joining == 1)
            {
                SetJoinFailState();
                return;
            }
            // if last message sent was a data message, and there was no reply...
            else if (loRa.macStatus.networkJoined == 1)
            {
                if (loRa.lorawanMacStatus.ackRequiredFromNextDownlinkMessage == ENABLED) // if last uplink packet was confirmed, we have to send this packet by the number indicated by NbRepConfFrames
                {
                    if (loRa.counterRepetitionsConfirmedUplink <= loRa.maxRepetitionsConfirmedUplink)
                    {
                        loRa.macStatus.macState = RETRANSMISSION_DELAY;
                        SwTimerSetTimeout(loRa.ackTimeoutTimerId, MS_TO_TICKS_SHORT(loRa.protocolParameters.ackTimeout));
                        SwTimerStart(loRa.ackTimeoutTimerId);
                    }
                    else
                    {
                        ResetParametersForConfirmedTransmission ();
                        if (rxPayload.RxAppData != NULL)
                        {
                            rxPayload.RxAppData (NULL, 0, MAC_NOT_OK);
                        }
                    }
                }

                else
                {
                    if (loRa.counterRepetitionsUnconfirmedUplink <= loRa.maxRepetitionsUnconfirmedUplink)
                    {
                        loRa.macStatus.macState = RETRANSMISSION_DELAY;
                        if (SelectChannelForTransmission (1) == OK)
                        {
                            //resend the last packet if the radio transmit function succeedes
                            if (RADIO_Transmit (&macBuffer[16], loRa.lastPacketLength) == OK)
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
                        else
                        {
                            // if no channel was found due to duty cycle limitations, start a timer and send the packet when the first channel will be free
                            for (i = 0; i <= loRa.maxChannels; i ++)
                            {
                                 if ( (Channels[i].status == ENABLED) && (Channels[i].channelTimer != 0) && (Channels[i].channelTimer <= minim) && (loRa.currentDataRate >= Channels[i].dataRange.min) && (loRa.currentDataRate <= Channels[i].dataRange.max) )
                                 {
                                     minim = Channels[i].channelTimer;
                                 }
                             }
                             SwTimerSetTimeout (loRa.unconfirmedRetransmisionTimerId, MS_TO_TICKS_SHORT(minim + 50) );
                             SwTimerStart (loRa.unconfirmedRetransmisionTimerId);
                        }
                    }
                    else
                    {
                        if(loRa.deviceClass != CLASS_C)
                        {
                            ResetParametersForUnconfirmedTransmission ();
                            if (rxPayload.RxAppData != NULL)
                            {
                                rxPayload.RxAppData(NULL, 0, MAC_OK);  // inform the application layer that no message was received back from the server
                            }
                        }
                    }
                }
            }
            if (CLASS_C == loRa.deviceClass)
            {
                loRa.macStatus.macState = CLASS_C_RX2_2_OPEN;
                LORAWAN_EnterContinuousReceive();
            }
                
        }
    }
    else
    {
        //Standalone radio reception NOK, using the same callback as in LoRaWAN rx
        if (rxPayload.RxAppData != NULL)
        {
            rxPayload.RxAppData(NULL, 0, RADIO_NOT_OK);
        }
    }
}

LorawanError_t ValidateDataRate (uint8_t dataRate)
{
    LorawanError_t result = OK;

    if ( dataRate > DR7 )
    {
        result = INVALID_PARAMETER;
    }
      
    return result;
}

LorawanError_t ValidateChMaskDataRate (uint16_t channelMask, uint8_t dataRate, uint8_t chMaskCntl)
{
    uint8_t i;
    LorawanError_t result = INVALID_PARAMETER;
    
    //If channelMask control is 6, enable all channels irrespective of channelMask setting
    if(chMaskCntl == 6) 
        channelMask = 0xFFFF;
    
    for (i = 0; i < loRa.maxChannels; i++)
    {
        if(( dataRate >= Channels[i].dataRange.min ) && ( dataRate <= Channels[i].dataRange.max ))
        {
            //Find atleast one channel enabled in the ChannelMask
            if((channelMask & BIT0) == BIT0)
            {
                return OK;
            }
        }
        channelMask = channelMask >> SHIFT1;
    }
      return result;
}

LorawanError_t ValidateTxPower (uint8_t txPowerNew)
{
    LorawanError_t result = OK;

    if ((ISM_EU868 == loRa.ismBand) && (txPowerNew > 7))
    {
        result = INVALID_PARAMETER;
    }

    return result;
}

uint8_t* ExecuteDutyCycle (uint8_t *ptr)
{
    uint8_t maxDCycle;

    maxDCycle = *(ptr++);
    if (maxDCycle < 15)
    {
        loRa.prescaler = 1 << maxDCycle; // Execute the 2^maxDCycle here
        loRa.macStatus.prescalerModified = ENABLED;
    }

    if (maxDCycle == 255)
    {
        loRa.macStatus.silentImmediately = ENABLED;
    }

    return ptr;
}

uint8_t* ExecuteLinkAdr (uint8_t *ptr, uint8_t *commandsLen)
{
    uint8_t txPower, dataRate, nextCommand;
    uint16_t channelMask;
    Redundancy_t *redundancy;
    uint8_t block = 1;
    bool channelAck = true, powerAck = false, drAck = false;

    ptr--;
    while(ptr < commandsLen)
    {
        //5 is the command length of LINK ADR
        if(((ptr+5) < commandsLen) && (*(ptr+5) == LINK_ADR_CID))
        {
            //If next command is LINK ADR, just process the Channel mask commands
            block++;
            redundancy = (Redundancy_t*) (ptr+4);
            if( ValidateChannelMaskCntl(redundancy->chMaskCntl) != OK)
                channelAck = false;
            ptr = ptr + 5;
        }
        
        //last command in the block (or) single link adr command
        else if (*(ptr) == LINK_ADR_CID)
        {
            ptr++;
            txPower = *(ptr) & LAST_NIBBLE;
            dataRate = ( *(ptr) & FIRST_NIBBLE ) >> SHIFT4;
            ptr++;
            channelMask = (*((uint16_t*)ptr));
            ptr = ptr + sizeof (channelMask);
            redundancy = (Redundancy_t*)(ptr++);

            if (ENABLED == loRa.macStatus.adr)
            {
                if(channelAck)
                {
                    if ( (ValidateChannelMaskCntl(redundancy->chMaskCntl) != OK))  // If the ChMask field value is one of values meaning RFU, the end-device should reject the command and unset the Channel mask ACK bit in its response.
                    {
                        channelAck = false;
                    }
                    else
                    {
                      if(redundancy->chMaskCntl != 6)       //CHANNEL MASK 6 CORRESPONDS TO ENABLE ALL DEFINED CHANNELS; ignore channel mask if channelmaskcntl =6 
                      {
                          if(ValidateChannelMask(channelMask) != OK)
                          {
                              channelAck = false;
                          }}
                    }
                }

                if ( (ValidateDataRate (dataRate) == OK) &&  (dataRate >= EU868_TX_MIN_DATARATE) && (dataRate <= EU868_TX_MAX_DATARATE) && (ValidateChMaskDataRate(channelMask, dataRate, redundancy->chMaskCntl) == OK)) 
                {
                    drAck = true;
                }

                if (ValidateTxPower (txPower) == OK)
                {
                    powerAck = true;
                }

                if ( (powerAck == 1) && (drAck == 1) && (channelAck == 1) )
                {
                    EnableChannels (channelMask, redundancy->chMaskCntl);

                    UpdateTxPower (txPower);
                    loRa.macStatus.txPowerModified = ENABLED; // the current tx power was modified, so the user is informed about the change via this flag
                    UpdateCurrentDataRate (dataRate);

                    if (redundancy->nbRep == 0)
                    {
                        loRa.maxRepetitionsUnconfirmedUplink = 0;
                    }
                    else
                    {
                        loRa.maxRepetitionsUnconfirmedUplink = redundancy->nbRep - 1;
                    }
                    loRa.macStatus.nbRepModified = 1;
                }
            }
            else
            {
                //defaults for drAck and powerAck are set to false
                channelAck = false;
            } 

            //Format the command response (for blocks)
            while(block--)
            {
                loRa.macCommands[loRa.crtMacCmdIndex].channelMaskAck = channelAck;
                loRa.macCommands[loRa.crtMacCmdIndex].dataRateAck = drAck;
                loRa.macCommands[loRa.crtMacCmdIndex].powerAck = powerAck;

                if((loRa.crtMacCmdIndex < MAX_NB_CMD_TO_PROCESS) && (block != 0))
                {
                    loRa.crtMacCmdIndex ++;
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

                    //Link ADR command
                    loRa.macCommands[loRa.crtMacCmdIndex].receivedCid = LINK_ADR_CID;
                }

            }
            break;
        }     
        }   
    return ptr;
}

uint8_t* ExecuteDevStatus (uint8_t *ptr)
{
    return ptr;
}

uint8_t* ExecuteNewChannel (uint8_t *ptr)
{
    uint8_t channelIndex;
    DataRange_t drRange;
    uint32_t frequency = 0;

    channelIndex = *(ptr++);

    frequency = (*((uint32_t*)ptr)) & 0x00FFFFFF;
    frequency = frequency * 100;
    ptr = ptr + 3;  // 3 bytes for frequecy

    drRange.value = *(ptr++);

    if (ValidateChannelId (channelIndex, WITHOUT_DEFAULT_CHANNELS) == OK)
    {
        if ( (ValidateFrequency (frequency) == OK) || (frequency == 0) )
        {
            loRa.macCommands[loRa.crtMacCmdIndex].channelFrequencyAck = 1;
        }

        if (ValidateDataRange (drRange.value) == OK)
        {
            loRa.macCommands[loRa.crtMacCmdIndex].dataRateRangeAck = 1;
        }
    }

    if ( (loRa.macCommands[loRa.crtMacCmdIndex].channelFrequencyAck == 1) && (loRa.macCommands[loRa.crtMacCmdIndex].dataRateRangeAck == 1) )
    {
        if (loRa.lastUsedChannelIndex < 16)
        {
            if (frequency != 0)
            {
                UpdateFrequency (channelIndex, frequency);
                UpdateDataRange (channelIndex, drRange.value);
                UpdateDutyCycle (channelIndex, DUTY_CYCLE_DEFAULT);
                UpdateChannelIdStatus (channelIndex, ENABLED);
            }
            else
            {
                LORAWAN_SetChannelIdStatus (channelIndex, DISABLED);  // according to the spec, a frequency value of 0 disables the channel
            }
        }
        else
        {
            if (frequency != 0)
            {
                UpdateFrequency (channelIndex + 16, frequency);
                UpdateDataRange (channelIndex + 16, drRange.value);
                UpdateDutyCycle (channelIndex + 16, DUTY_CYCLE_DEFAULT);
                UpdateChannelIdStatus (channelIndex + 16, ENABLED);
            }
            else
            {
                LORAWAN_SetChannelIdStatus (channelIndex + 16, DISABLED);  // according to the spec, a frequency value of 0 disables the channel
            }
        }

        loRa.macStatus.channelsModified = 1; // a new channel was added, so the flag is set to inform the user
    }
    return ptr;
}

uint8_t* ExecuteRxParamSetupReq (uint8_t *ptr)
{
    DlSettings_t dlSettings;
    uint32_t frequency = 0;

    //In the status field (response) we have to include the following: channle ACK, RX2 data rate ACK, RX1DoffsetACK

    dlSettings.value = *(ptr++);

    frequency = (*((uint32_t*)ptr)) & 0x00FFFFFF;
    frequency = frequency * 100;
    ptr = ptr + 3; //3 bytes for frequency

    if (ValidateFrequency (frequency) == OK)
    {
        loRa.macCommands[loRa.crtMacCmdIndex].channelAck = 1;
    }

    if (ValidateDataRate (dlSettings.bits.rx2DataRate) == OK)
    {
        loRa.macCommands[loRa.crtMacCmdIndex].dataRateReceiveWindowAck = 1;
    }

    if (ValidateRxOffset (dlSettings.bits.rx1DROffset) == OK)
    {
        loRa.macCommands[loRa.crtMacCmdIndex].rx1DROffestAck = 1;
    }

    if ( (loRa.macCommands[loRa.crtMacCmdIndex].dataRateReceiveWindowAck == 1) && (loRa.macCommands[loRa.crtMacCmdIndex].channelAck == 1) && (loRa.macCommands[loRa.crtMacCmdIndex].rx1DROffestAck == 1))
    {
        loRa.offset = dlSettings.bits.rx1DROffset;
        UpdateReceiveWindow2Parameters (frequency, dlSettings.bits.rx2DataRate);
        loRa.macStatus.secondReceiveWindowModified = 1;
    }
    
    rxParamSetupReqAnsFlag = true;

    return ptr;
}

uint8_t* ExecuteDlChannelReq (uint8_t *ptr)
{
    uint8_t channelIndex;
    uint32_t frequency = 0;
    
    channelIndex = *(ptr++);

    frequency = (*((uint32_t*)ptr)) & 0x00FFFFFF;
    frequency = frequency * 100;
    ptr = ptr + 3;  // 3 bytes for frequency

    if (ValidateDlChannelId (channelIndex) == OK)
    {
        if (ValidateFrequency (frequency) == OK)
        {
            loRa.macCommands[loRa.crtMacCmdIndex].dlChannelFreqOkAck = 1;
        }  
        if ( ((Channels[channelIndex].parametersDefined & FREQUENCY_DEFINED) == FREQUENCY_DEFINED) && (Channels[channelIndex].status == ENABLED) )
        {
            loRa.macCommands[loRa.crtMacCmdIndex].dlUplinkFreqExistsAck = 1;
        }
    }
        
    if ( (loRa.macCommands[loRa.crtMacCmdIndex].dlChannelFreqOkAck == 1) && (loRa.macCommands[loRa.crtMacCmdIndex].dlUplinkFreqExistsAck == 1) )
    {
        UpdateDlChannelWindow1Parameters (channelIndex, frequency);
    }
    
    dlChannelReqAnsFlag = true;
    
    return ptr;
}

LorawanError_t SearchAvailableChannel (uint8_t maxChannels, bool transmissionType, uint8_t* channelIndex)
{
    uint8_t randomNumberCopy, randomNumber, i;
    LorawanError_t result = OK;

    randomNumber = Random (maxChannels) + 1; //this is a guard so that randomNumber is not 0 and the search will happen
    randomNumberCopy = randomNumber;

    while (randomNumber)
    {
        for (i=0; (i < maxChannels) && (randomNumber != 0) ; i++)
        {
            if ( ( Channels[i].status == ENABLED ) && ( Channels[i].channelTimer == 0 ) && ( loRa.currentDataRate >= Channels[i].dataRange.min ) && ( loRa.currentDataRate <= Channels[i].dataRange.max ) )
            {
                if (transmissionType == 0) // if transmissionType is join request, then check also for join request channels
                {
                    if ( Channels[i].joinRequestChannel == 1 )
                    {
                        randomNumber --;
                    }
                }
                else
                {
                    randomNumber --;
                }
            }
        }
        // if after one search in all the vector no valid channel was found, exit the loop and return an error
        if ( randomNumber == randomNumberCopy )
        {
            result = NO_CHANNELS_FOUND;
            break;
        }
    }

    if ( i != 0)
    {
        *channelIndex = i - 1;
    }
    else
    {
        result = NO_CHANNELS_FOUND;
    }
    return result;
}

void UpdateCfList (uint8_t bufferLength, JoinAccept_t *joinAccept)  
{
    uint8_t i;
    uint32_t frequency;
    uint8_t channelIndex;
    
    if ( (bufferLength == SIZE_JOIN_ACCEPT_WITH_CFLIST) )
    {
        // 3 is the minimum channel index for single band
        channelIndex = 3;

        for (i = 0; i < NUMBER_CFLIST_FREQUENCIES; i++ )
        {
            frequency = 0;
            memcpy (&frequency, joinAccept->members.cfList + 3*i, 3);
            frequency *= 100;
            if (frequency != 0)
            {
                if (ValidateFrequency (frequency) == OK)
                {
                    Channels[i+channelIndex].frequency = frequency;
                    Channels[i+channelIndex].downFrequency = 0;
                    Channels[i+channelIndex].dataRange.max = DR5;
                    Channels[i+channelIndex].dataRange.min = DR0;
                    Channels[i+channelIndex].dutyCycle = DUTY_CYCLE_DEFAULT_NEW_CHANNEL;
                    Channels[i+channelIndex].parametersDefined = 0xFF; //all parameters defined
                    LORAWAN_SetChannelIdStatus(i+channelIndex, ENABLED);
                    loRa.macStatus.channelsModified = ENABLED; // a new channel was added, so the flag is set to inform the user
                }
            }
            else
            {
                LORAWAN_SetChannelIdStatus(i+channelIndex, DISABLED);
            }
         }
    
         loRa.macStatus.channelsModified = ENABLED;
    }
}

void ConfigureRadio(uint8_t dataRate, uint32_t freq)
{
    RADIO_SetModulation (modulation[dataRate]);
    RADIO_SetChannelFrequency (freq);
    RADIO_SetFrequencyHopPeriod (DISABLED);

    if (dataRate <= DR6)
    {
        //LoRa modulation
        RADIO_SetSpreadingFactor (spreadingFactor[dataRate]);        
        RADIO_SetBandwidth (bandwidth[dataRate]);
        RADIO_SetLoRaSyncWord(loRa.syncWord);        
    }
    else
    {
        //FSK modulation
        RADIO_SetFSKSyncWord(sizeof(FskSyncWordBuff) / sizeof(FskSyncWordBuff[0]), (uint8_t*)FskSyncWordBuff);
    }   
}

uint32_t GetRx1Freq (void)
{
    return loRa.receiveWindow1Parameters.frequency;
}

void UpdateDLSettings(uint8_t dlRx2Dr, uint8_t dlRx1DrOffset)
{
    if (dlRx2Dr <= DR7)
    {
        loRa.receiveWindow2Parameters.dataRate = dlRx2Dr;
    }

    if (dlRx1DrOffset <= 5)
    {
        // update the offset between the uplink data rate and the downlink data rate used to communicate with the end device on the first reception slot
        loRa.offset = dlRx1DrOffset;
    }   
}

void StartReTxTimer(void)
{
    uint8_t i;
    uint32_t minim = UINT32_MAX;
    
    for (i = 0; i <= loRa.maxChannels; i++)
    {
        if ( (Channels[i].status == ENABLED) && (Channels[i].channelTimer != 0) && (Channels[i].channelTimer <= minim) && (loRa.currentDataRate >= Channels[i].dataRange.min) && (loRa.currentDataRate <= Channels[i].dataRange.max) )
        {
            minim = Channels[i].channelTimer;
        }
    }
    loRa.macStatus.macState = RETRANSMISSION_DELAY;
    SwTimerSetTimeout (loRa.automaticReplyTimerId, MS_TO_TICKS_SHORT(minim) );
    SwTimerStart (loRa.automaticReplyTimerId);    
}

LorawanError_t SelectChannelForTransmission (bool transmissionType)  // transmission type is 0 means join request, transmission type is 1 means data message mode
{
    LorawanError_t result = OK;
    uint8_t channelIndex;

    result = SearchAvailableChannel (MAX_EU_SINGLE_BAND_CHANNELS, transmissionType, &channelIndex);

    if (result == OK)
    {
        loRa.lastUsedChannelIndex = channelIndex;
        if(Channels[channelIndex].downFrequency == 0)
        {
            loRa.receiveWindow1Parameters.frequency = Channels[channelIndex].frequency;
        }
        else
        {
            loRa.receiveWindow1Parameters.frequency = Channels[channelIndex].downFrequency;
        }
        loRa.receiveWindow1Parameters.dataRate = loRa.currentDataRate;        

        ConfigureRadioTx(loRa.receiveWindow1Parameters.dataRate, Channels[channelIndex].frequency);
    }
    return result;
}

static void CreateAllSoftwareTimers (void)
{
    loRa.joinAccept1TimerId = SwTimerCreate();
    loRa.joinAccept2TimerId = SwTimerCreate();
    loRa.receiveWindow1TimerId = SwTimerCreate();
    loRa.receiveWindow2TimerId = SwTimerCreate();
    loRa.linkCheckTimerId = SwTimerCreate();
    loRa.ackTimeoutTimerId = SwTimerCreate();
    loRa.automaticReplyTimerId = SwTimerCreate();
    loRa.unconfirmedRetransmisionTimerId = SwTimerCreate();
    loRa.abpJoinTimerId = SwTimerCreate();
    loRa.dutyCycleTimerId = SwTimerCreate();
    loRa.otaaJoinTimerId = SwTimerCreate();
}

static void SetCallbackSoftwareTimers (void)
{        
    SwTimerSetCallback(loRa.joinAccept1TimerId, LORAWAN_ReceiveWindow1Callback, 0);
    SwTimerSetCallback(loRa.joinAccept2TimerId, LORAWAN_ReceiveWindow2Callback, 0);
    SwTimerSetCallback(loRa.linkCheckTimerId, LORAWAN_LinkCheckCallback, 0);
    SwTimerSetCallback(loRa.receiveWindow1TimerId, LORAWAN_ReceiveWindow1Callback, 0);
    SwTimerSetCallback(loRa.receiveWindow2TimerId, LORAWAN_ReceiveWindow2Callback, 0);
    SwTimerSetCallback(loRa.ackTimeoutTimerId, AckRetransmissionCallback, 0);
    SwTimerSetCallback(loRa.automaticReplyTimerId, AutomaticReplyCallback, 0);
    SwTimerSetCallback(loRa.unconfirmedRetransmisionTimerId, UnconfirmedTransmissionCallback, 0);
    SwTimerSetCallback(loRa.abpJoinTimerId, UpdateJoinSuccessState, 0);
    SwTimerSetCallback(loRa.dutyCycleTimerId, DutyCycleCallback, 0);
    SwTimerSetCallback(loRa.otaaJoinTimerId, OTAAJoinTimerCallback, 0);
}

static void StopAllSoftwareTimers (void)
{    
    SwTimerStop(loRa.joinAccept1TimerId);
    SwTimerStop(loRa.joinAccept2TimerId);
    SwTimerStop(loRa.linkCheckTimerId);
    SwTimerStop(loRa.receiveWindow1TimerId);
    SwTimerStop(loRa.receiveWindow2TimerId);
    SwTimerStop(loRa.ackTimeoutTimerId);
    SwTimerStop(loRa.automaticReplyTimerId);
    SwTimerStop(loRa.unconfirmedRetransmisionTimerId);
    SwTimerStop(loRa.abpJoinTimerId);
	SwTimerStop(loRa.dutyCycleTimerId);
    SwTimerStop(loRa.otaaJoinTimerId);
}

static void InitDefault868Channels (void)
{
    uint8_t i;

    memset (Channels, 0, sizeof(Channels) );
    memcpy (Channels, DefaultChannels868, sizeof(DefaultChannels868) );
    for (i = 3; i < MAX_EU_SINGLE_BAND_CHANNELS; i++)
    {
        // for undefined channels the duty cycle should be a very big value, and the data range a not-valid value
        //duty cycle 0 means no duty cycle limitation, the bigger the duty cycle value, the greater the limitation
        Channels[i].dutyCycle = UINT16_MAX; 
        Channels[i].dataRange.value = UINT8_MAX;
    }
}

static void InitDefault433Channels (void)
{
    uint8_t i;

    memset (Channels, 0, sizeof(Channels) );
    memcpy (Channels, DefaultChannels433, sizeof(DefaultChannels433) );
    for (i = 3; i < MAX_EU_SINGLE_BAND_CHANNELS; i++)
    {
        // for undefined channels the duty cycle should be a very big value, and the data range a not-valid value
        //duty cycle 0 means no duty cycle limitation, the bigger the duty cycle value, the greater the limitation
        Channels[i].dutyCycle = UINT16_MAX; 
        Channels[i].dataRange.value = UINT8_MAX; 
    }
}

static void UpdateDataRange (uint8_t channelId, uint8_t dataRangeNew)
{
    uint8_t i;
    // after updating the data range of a channel we need to check if the minimum dataRange has changed or not.
    // The user cannot set the current data rate outside the range of the data range
    loRa.minDataRate = DR7;
    loRa.maxDataRate = DR0;

    Channels[channelId].dataRange.value = dataRangeNew;
    Channels[channelId].parametersDefined |= DATA_RANGE_DEFINED;
    for (i=0; i < loRa.maxChannels; i++)
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

    if (loRa.currentDataRate > loRa.maxDataRate)
    {
        loRa.currentDataRate = loRa.maxDataRate;
    }

    if (loRa.currentDataRate < loRa.minDataRate)
    {
        loRa.currentDataRate = loRa.minDataRate;
    }
}

static void UpdateChannelIdStatus (uint8_t channelId, bool statusNew)
{
    uint8_t i;

    Channels[channelId].status = statusNew;
    if(Channels[channelId].status == DISABLED)
    {
        //Clear the dutycycle timer of the channel
        Channels[channelId].channelTimer = 0;
    }

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
    
    if (loRa.currentDataRate > loRa.maxDataRate)
    {
        loRa.currentDataRate = loRa.maxDataRate;
    }

    if (loRa.currentDataRate < loRa.minDataRate)
    {
        loRa.currentDataRate = loRa.minDataRate;
    }
}

static LorawanError_t ValidateRxOffset (uint8_t rxOffset)
{
    LorawanError_t result =  OK;

    if (rxOffset > 5)
    {
        result = INVALID_PARAMETER;
    }

    return result;
}

static LorawanError_t ValidateFrequency (uint32_t frequencyNew)
{
    LorawanError_t result = OK;

    if(ISM_EU868 == loRa.ismBand)
    {
        if ( (frequencyNew > FREQ_870000KHZ) || (frequencyNew < FREQ_863000KHZ) )
        {
            result = INVALID_PARAMETER ;
        }        
    }
    else
    {
        if ( (frequencyNew > FREQ_434790KHZ) || (frequencyNew < FREQ_433050KHZ) )
        {
            result = INVALID_PARAMETER ;
        }        
    }

    return result;
}

static LorawanError_t ValidateDataRange (uint8_t dataRangeNew)
{
    LorawanError_t result = OK;
    uint8_t dataRateMax, dataRateMin;

    dataRateMin = dataRangeNew & LAST_NIBBLE;
    dataRateMax = (dataRangeNew & FIRST_NIBBLE) >> SHIFT4;

    if ( (ValidateDataRate (dataRateMax) != OK) || (ValidateDataRate (dataRateMin) != OK ) || (dataRateMax < dataRateMin) )
    {
        result = INVALID_PARAMETER;
    }
    return result;
}

static LorawanError_t ValidateChannelId (uint8_t channelId, bool allowedForDefaultChannels)  //if allowedForDefaultChannels is 1, all the channels can be modified, if it is 0 channels 0, 1, 2 and 16, 17, and 18 (dual band) cannot be modified
{
    LorawanError_t result = OK;

    if ( (channelId >= MAX_EU_SINGLE_BAND_CHANNELS) ||  ( (allowedForDefaultChannels == WITHOUT_DEFAULT_CHANNELS) && (channelId < 3) ) )
    {
        result = INVALID_PARAMETER ;
    }

    return result;
}

static LorawanError_t ValidateDlChannelId (uint8_t channelId)
{
    LorawanError_t result = OK;
        
    if ( channelId >= MAX_EU_SINGLE_BAND_CHANNELS )
    {
        result = INVALID_PARAMETER ;
    }

    return result;
}

static LorawanError_t ValidateChannelMaskCntl (uint8_t channelMaskCntl)
{
   LorawanError_t result = OK;

   if ( (channelMaskCntl != 0) && (channelMaskCntl != 6) )
   {
       result = INVALID_PARAMETER;
   }

  return result;
}

static void EnableChannels (uint16_t channelMask, uint8_t channelMaskCntl)
{
    EnableChannels1 (channelMask, channelMaskCntl, 0, MAX_EU_SINGLE_BAND_CHANNELS);
}

static void UpdateFrequency (uint8_t channelId, uint32_t frequencyNew )
{
    Channels[channelId].frequency = frequencyNew;
    Channels[channelId].parametersDefined |= FREQUENCY_DEFINED;
}

static void UpdateDutyCycle (uint8_t channelId, uint16_t dutyCycleNew)
{
    Channels[channelId].dutyCycle = dutyCycleNew;
    Channels[channelId].parametersDefined |= DUTY_CYCLE_DEFINED;
}

static LorawanError_t ValidateChannelMask (uint16_t channelMask)
{
   uint8_t i = 0;

   if(channelMask != 0x0000U)
   {
        for (i = 0; i < MAX_EU_SINGLE_BAND_CHANNELS; i++)
        {
            if ( ( (channelMask & BIT0) == BIT0) && ( (Channels[i].parametersDefined & (FREQUENCY_DEFINED | DATA_RANGE_DEFINED | DUTY_CYCLE_DEFINED) ) != (FREQUENCY_DEFINED | DATA_RANGE_DEFINED | DUTY_CYCLE_DEFINED) ) )  // if the channel mask sent enables a yet undefined channel, the command is discarded and the device state is not changed
            {
                return INVALID_PARAMETER;
            }
            else
            {
                channelMask = channelMask >> SHIFT1;
            }
        }

      return OK;
   }
   else
   {
       //ChMask set to 0x0000 in ADR may be used as a DoS attack so receiving this results in an error
       return INVALID_PARAMETER;
   }
}

static void EnableChannels1 (uint16_t channelMask, uint8_t channelMaskCntl, uint8_t channelIndexMin,  uint8_t channelIndexMax)
{
   uint8_t i;

   if (channelMaskCntl == 6)
   {
       for ( i = channelIndexMin; i < channelIndexMax; i++ )
       {
           if((Channels[i].parametersDefined & FREQUENCY_DEFINED) == FREQUENCY_DEFINED)
               UpdateChannelIdStatus (i, ENABLED);
       }
   }
   else if (channelMaskCntl == 0)
   {
       for ( i = channelIndexMin; i < channelIndexMax; i++ )
       {
           if ( channelMask & BIT0 == BIT0)
           {
               UpdateChannelIdStatus (i, ENABLED);
           }
           else
           {
               UpdateChannelIdStatus (i, DISABLED);
           }
           channelMask = channelMask >> SHIFT1;
       }
   }
}

static void DutyCycleCallback (uint8_t param)
{
    uint32_t minim = UINT32_MAX;
    bool found = false;
    uint8_t i;

    for (i=0; i < MAX_EU_SINGLE_BAND_CHANNELS; i++)
    {
        //Validate this only for enabled channels
        if ((Channels[i].status == ENABLED) && ( Channels[i].channelTimer != 0 ))
        {
            if ( Channels[i].channelTimer > loRa.lastTimerValue )
            {
                Channels[i].channelTimer = Channels[i].channelTimer - loRa.lastTimerValue;
            }
            else
            {
                Channels[i].channelTimer = 0;
            }
            if ( (Channels[i].channelTimer <= minim) && (Channels[i].channelTimer != 0) )
            {
                minim  = Channels[i].channelTimer;
                found = true;
            }
        }
    }
    if ( found == true )
    {
        loRa.lastTimerValue = minim;
        SwTimerSetTimeout (loRa.dutyCycleTimerId, MS_TO_TICKS(minim));
        SwTimerStart (loRa.dutyCycleTimerId);
    }
}

static void OTAAJoinTimerCallback(uint8_t param)
{
    loRa.otaaMinuteCount++;
    if(loRa.otaaMinuteCount >= 60)
    {
        loRa.otaaHourCount++;
        loRa.otaaMinuteCount = 0;
    }
    SwTimerSetTimeout (loRa.otaaJoinTimerId, MS_TO_TICKS(OTAA_MINUTECOUNT_MS));
    SwTimerStart (loRa.otaaJoinTimerId);
}

static void ConfigureRadioTx(uint8_t dataRate, uint32_t freq)
{
    int8_t txPower;
    
    ConfigureRadio(dataRate, freq);
    
    if (ISM_EU868 == loRa.ismBand)
    {
        txPower = txPower868[loRa.txPower];        
    }
    else
    {
        txPower = txPower868[loRa.txPower];
    }
    
    RADIO_SetOutputPower (txPower);
    
    RADIO_SetCRC(ENABLED);
    RADIO_SetIQInverted(DISABLED);     
}

