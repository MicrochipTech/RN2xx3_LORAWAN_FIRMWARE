/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    radio_driver_SX1276.c

  @Summary:
    This is the Radio Driver SX1276 source file which contains LoRa-specific
    Radio Driver functions declarations and defines for SX1276

  @Description:
    This source file provides LoRa-specific implementations for Radio Driver for SX1276.
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
 *                           radio_driver_SX1276.c
 *
 * Radio Driver SX1276 source file
 *
 ******************************************************************************/

#include "radio_interface.h"
#include "radio_registers_SX1276.h"
#include "lorawan_defs.h"
#include "radio_driver_hal.h"
#include "lorawan_radio.h"
#include "sw_timer.h"
#if defined(DEBUGRX2)
// debug bct
#include "../../parser/parser.h"
#include "../../parser/parser_tsp.h"
#include "../../parser/parser_utils.h"
#endif // DEBUGRX2

#define TIME_ON_AIR_LOAD_VALUE              ((uint32_t)20000)
#define WATCHDOG_DEFAULT_TIME               ((uint32_t)15000)


// These enums need to be set according to the definition in the radio
// datasheet
typedef enum
{
    MODE_SLEEP = 0,
    MODE_STANDBY,
    MODE_FSTX,
    MODE_TX,
    MODE_FSRX,
    MODE_RXCONT,
    MODE_RXSINGLE,
    MODE_CAD,
} RadioMode_t;

typedef struct
{
    uint32_t frequency;
    uint32_t frequencyDeviation;
    uint32_t bitRate;
    uint16_t preambleLen;
    uint8_t syncWordLoRa;
    uint8_t syncWord[8];
    uint8_t syncWordLen;
    RadioModulation_t modulation;
    RadioDataRate_t dataRate;
    RadioLoRaBandWidth_t bandWidth;
    int8_t outputPower;
    uint8_t crcOn;
    uint8_t paBoost;
    uint16_t frequencyHopPeriod;
    uint8_t iqInverted;
    RadioErrorCodingRate_t errorCodingRate;
    uint8_t implicitHeaderMode;
    uint8_t flags;
    uint8_t dataBufferLen;
    uint8_t *dataBuffer;
    uint8_t timeOnAirTimerId;
    uint8_t fskRxWindowTimerId;
    uint8_t watchdogTimerId;
    uint32_t watchdogTimerTimeout;
    uint8_t initialized;
    uint32_t (*fhssNextFrequency)(void);
    uint8_t regVersion;
    int8_t packetSNR;
    int16_t packetRSSI;
    RadioFSKShaping_t fskDataShaping;
    RadioFSKBandWidth_t rxBw;
    RadioFSKBandWidth_t afcBw;
} RadioConfiguration_t;



static RadioConfiguration_t RadioConfiguration;



static void RADIO_RxFSKTimeout(uint8_t param);
static void RADIO_WatchdogTimeout(uint8_t param);
static void RADIO_WriteMode(RadioMode_t newMode, RadioModulation_t newModulation, uint8_t blocking);
static void RADIO_Reset(void);
static void RADIO_WriteFrequency(uint32_t frequency);
static void RADIO_WriteFSKFrequencyDeviation(uint32_t frequencyDeviation);
static void RADIO_WriteFSKBitRate(uint32_t bitRate);
static void RADIO_WritePower(int8_t power);
static void RADIO_WriteConfiguration(uint16_t symbolTimeout);
static void RADIO_RxDone(void);
static void RADIO_FSKPayloadReady(void);
static void RADIO_RxTimeout(void);
static void RADIO_TxDone(void);
static void RADIO_FSKPacketSent(void);
static void RADIO_UnhandledInterrupt(RadioModulation_t modulation);
static void RADIO_FHSSChangeChannel(void);



void RADIO_RegisterWrite(uint8_t reg, uint8_t value)
{
    HALSPICSAssert();
    HALSPISend(REG_WRITE | reg);
    HALSPISend(value);
    HALSPICSDeassert();
}

uint8_t RADIO_RegisterRead(uint8_t reg)
{
    uint8_t readValue;
    reg &= 0x7F;    // Make sure write bit is not set
    HALSPICSAssert();
    HALSPISend(reg);
    readValue = HALSPISend(0xFF);
    HALSPICSDeassert();
    return readValue;
}


// This function repurposes DIO5 for ModeReady functionality
static void RADIO_WriteMode(RadioMode_t newMode, RadioModulation_t newModulation, uint8_t blocking)
{
    uint8_t opMode;
    uint8_t dioMapping;
    RadioModulation_t currentModulation;
    RadioMode_t currentMode;

    if ((MODULATION_FSK == newModulation) &&
        ((MODE_RXSINGLE == newMode) || (MODE_CAD == newMode)))
    {
        // Unavaliable modes for FSK. Just return.
        return;
    }

    // Sanity enforcement on parameters
    newMode &= 0x07;
    newModulation &= 0x01;

    opMode = RADIO_RegisterRead(REG_OPMODE);

    if ((opMode & 0x80) != 0)
    {
        currentModulation = MODULATION_LORA;
    }
    else
    {
        currentModulation = MODULATION_FSK;
    }

    currentMode = opMode & 0x07;

    // If we need to change modulation, we need to do this in sleep mode.
    // Otherwise, we can go straight to changing the current mode to newMode.
    if (newModulation != currentModulation)
    {
        // Go to sleep
        if (MODE_SLEEP != currentMode)
        {
            // Clear mode bits, effectively going to sleep
            RADIO_RegisterWrite(REG_OPMODE, opMode & (~0x07));
            currentMode = MODE_SLEEP;
        }
        // Change modulation
        if (MODULATION_FSK == newModulation)
        {
            // Clear MSB and sleep bits to make it stay in sleep
            opMode = opMode & (~0x87);
        }
        else
        {
            // LoRa mode. Set MSB and clear sleep bits to make it stay in sleep
            opMode = 0x80 | (opMode & (~0x87));
        }
        RADIO_RegisterWrite(REG_OPMODE, opMode);
    }

    // From here on currentModulation is no longer current, we will use
    // newModulation instead as it reflects the chip configuration.
    // opMode reflects the actual configuration of the chip.

    if (newMode != currentMode)
    {
        // If we need to block until the mode switch is ready, configure the
        // DIO5 pin to relay this information.
        if ((MODE_SLEEP != newMode) && (1 == blocking))
        {
            dioMapping = RADIO_RegisterRead(REG_DIOMAPPING2);
            if (MODULATION_FSK == newModulation)
            {
                // FSK mode
                dioMapping |= 0x30;     // DIO5 = 11 means ModeReady in FSK mode
            }
            else
            {
                // LoRa mode
                dioMapping &= ~0x30;    // DIO5 = 00 means ModeReady in LoRa mode
            }
            RADIO_RegisterWrite(REG_DIOMAPPING2, dioMapping);
        }

        // Do the actual mode switch.
        opMode &= ~0x07;                // Clear old mode bits
        opMode |= newMode;              // Set new mode bits
        RADIO_RegisterWrite(REG_OPMODE, opMode);

        // If required and possible, wait for switch to complete
        if (1 == blocking)
        {
            if (MODE_SLEEP != newMode)
            {
                while (HALDIO5PinValue() == 0)
                    ;
            }
            else
            {
                SystemBlockingWaitMs(1);
            }
        }
    }
}


static void RADIO_Reset(void)
{
    HALResetPinMakeOutput();
    HALResetPinOutputValue(0);
    SystemBlockingWaitMs(1);
    HALResetPinMakeInput();
    SystemBlockingWaitMs(10);
    //Added these two lines to make sure this pin is not left in floating state during sleep
    HALResetPinOutputValue(1);
    HALResetPinMakeOutput();
}

// The math in this function needs adjusting for FXOSC != 32MHz
static void RADIO_WriteFrequency(uint32_t frequency)
{
    uint32_t num, num_mod;

    // Frf = (Fxosc * num) / 2^19
    // We take advantage of the fact that 32MHz = 15625Hz * 2^11
    // This simplifies our formula to Frf = (15625Hz * num) / 2^8
    // Thus, num = (Frf * 2^8) / 15625Hz

    // First, do the division, since Frf * 2^8 does not fit in 32 bits
    num = frequency / 15625;
    num_mod = frequency % 15625;

    // Now do multiplication as well, both for the quotient as well as for
    // the remainder
    num <<= SHIFT8;
    num_mod <<= SHIFT8;

    // Try to correct for the remainder. After the multiplication we can still
    // recover some accuracy
    num_mod = num_mod / 15625;
    num += num_mod;

    // Now variable num holds the representation of the frequency that needs to
    // be loaded into the radio chip
    RADIO_RegisterWrite(REG_FRFMSB, (num >> SHIFT16) & 0xFF);
    RADIO_RegisterWrite(REG_FRFMID, (num >> SHIFT8) & 0xFF);
    RADIO_RegisterWrite(REG_FRFLSB, num & 0xFF);
}

// The math in this function needs adjusting for FXOSC != 32MHz
// This function needs to be called with the radio configured in FSK mode
static void RADIO_WriteFSKFrequencyDeviation(uint32_t frequencyDeviation)
{
    uint32_t num;

    // Fdev = (Fxosc * num) / 2^19
    // We take advantage of the fact that 32MHz = 15625Hz * 2^11
    // This simplifies our formula to Fdev = (15625Hz * num) / 2^8
    // Thus, num = (Fdev * 2^8) / 15625Hz

    num = frequencyDeviation;
    num <<= SHIFT8;     // Multiply by 2^8
    num /= 15625;       // divide by 15625

    // Now variable num holds the representation of the frequency deviation that
    // needs to be loaded into the radio chip
    RADIO_RegisterWrite(REG_FSK_FDEVMSB, (num >> SHIFT8) & 0xFF);
    RADIO_RegisterWrite(REG_FSK_FDEVLSB, num & 0xFF);
}

// The math in this function needs adjusting for FXOSC != 32MHz
// This function needs to be called with the radio configured in FSK mode.
// BitrateFrac is always 0
static void RADIO_WriteFSKBitRate(uint32_t bitRate)
{
    uint32_t num;

    num = 32000000;
    num /= bitRate;

    // Now variable num holds the representation of the bitrate that
    // needs to be loaded into the radio chip
    RADIO_RegisterWrite(REG_FSK_BITRATEMSB, (num >> SHIFT8) & 0xFF);
    RADIO_RegisterWrite(REG_FSK_BITRATELSB, num & 0xFF);
    RADIO_RegisterWrite(REG_FSK_BITRATEFRAC, 0x00);
}

int8_t RADIO_GetMaxPower(void)
{
    if (RadioConfiguration.paBoost == 0)
    {
        return 15;
    }
    else
    {
        return 20;
    }
}

static void RADIO_WritePower(int8_t power)
{
    uint8_t paDac;
    uint8_t ocp;

    if (RadioConfiguration.paBoost == 0)
    {
        // RFO pin used for RF output
        if (power < -3)
        {
            power = -3;
        }
        if (power > 15)
        {
            power = 15;
        }

        paDac = RADIO_RegisterRead(REG_PADAC);
        paDac &= ~(0x07);
        paDac |= 0x04;
        RADIO_RegisterWrite(REG_PADAC, paDac);

        if (power < 0)
        {
            // MaxPower = 2
            // Pout = 10.8 + MaxPower*0.6 - 15 + OutPower
            // Pout = -3 + OutPower
            power += 3;
            RADIO_RegisterWrite(REG_PACONFIG, 0x20 | power);
        }
        else
        {
            // MaxPower = 7
            // Pout = 10.8 + MaxPower*0.6 - 15 + OutPower
            // Pout = OutPower
            RADIO_RegisterWrite(REG_PACONFIG, 0x70 | power);
        }
    }
    else
    {
        // PA_BOOST pin used for RF output

        // Lower limit
        if (power < 2)
        {
            power = 2;
        }

        // Upper limit
        if (power >= 20)
        {
            power = 20;
        }
        // In the RN2903 datasheet (DS50002390E) Table 2-5 shows an actual
        // output power jumps from 17dBm to 18.5 dBm, skipping over 18.
        // Therefore the value 18 is replaced with 17.
        else if (power > 17)
        {
            power = 17;
        }

        ocp = RADIO_RegisterRead(REG_OCP);
        paDac = RADIO_RegisterRead(REG_PADAC);
        paDac &= ~(0x07);
        if (power == 20)
        {
            paDac |= 0x07;
            power = 15;
            ocp &= ~(0x20);
        }
        else
        {
            paDac |= 0x04;
            power -= 2;
            ocp |= 0x20;
        }

        RADIO_RegisterWrite(REG_PADAC, paDac);
        RADIO_RegisterWrite(REG_PACONFIG, 0x80 | power);
        RADIO_RegisterWrite(REG_OCP, ocp);
    }
}

void RADIO_Init(uint8_t *radioBuffer, uint32_t frequency)
{
    char cnt=0;
    
    RadioConfiguration.frequency = frequency;
    RadioConfiguration.frequencyDeviation = 25000;
    RadioConfiguration.bitRate = 50000;
    RadioConfiguration.modulation = MODULATION_LORA;
    RadioConfiguration.bandWidth = BW_125KHZ;
    RadioConfiguration.outputPower = 2;
    RadioConfiguration.errorCodingRate = CR_4_5;
    RadioConfiguration.implicitHeaderMode = 0;
    RadioConfiguration.preambleLen = 8;
    RadioConfiguration.dataRate = SF_12;
    RadioConfiguration.crcOn = 1;
    RadioConfiguration.paBoost = 0;
    RadioConfiguration.iqInverted = 0;
    RadioConfiguration.syncWord[0] = 0xC1;
    RadioConfiguration.syncWord[1] = 0x94;
    RadioConfiguration.syncWord[2] = 0xC1;
    RadioConfiguration.syncWordLen = 3;
    RadioConfiguration.syncWordLoRa = 0x34;
    RadioConfiguration.flags = 0;
    RadioConfiguration.dataBufferLen = 0;
    RadioConfiguration.dataBuffer = radioBuffer;
    RadioConfiguration.frequencyHopPeriod = 0;
    RadioConfiguration.packetSNR = -128;
    RadioConfiguration.packetRSSI = -128;
    RadioConfiguration.watchdogTimerTimeout = WATCHDOG_DEFAULT_TIME;
    RadioConfiguration.fskDataShaping = FSK_SHAPING_GAUSS_BT_0_5;
    RadioConfiguration.rxBw = FSKBW_50_0KHZ;
    RadioConfiguration.afcBw = FSKBW_83_3KHZ;
    RadioConfiguration.fhssNextFrequency = NULL;


    // Make sure we do not allocate multiple software timers just because the
    // radio's initialization function is called multiple times.
    if (0 == RadioConfiguration.initialized)
    {
        // This behaviour depends on the compiler's behaviour regarding
        // uninitialized variables. It should be configured to set them to 0.
        RadioConfiguration.timeOnAirTimerId = SwTimerCreate();
        RadioConfiguration.fskRxWindowTimerId = SwTimerCreate();
        RadioConfiguration.watchdogTimerId = SwTimerCreate();
        SwTimerSetCallback(RadioConfiguration.fskRxWindowTimerId, RADIO_RxFSKTimeout, 0);
        SwTimerSetCallback(RadioConfiguration.watchdogTimerId, RADIO_WatchdogTimeout, 0);
        RadioConfiguration.initialized = 1;
    }
    else
    {
        SwTimerStop(RadioConfiguration.timeOnAirTimerId);
        SwTimerStop(RadioConfiguration.fskRxWindowTimerId);
        SwTimerStop(RadioConfiguration.watchdogTimerId);
    }

    RADIO_Reset();

    // Perform image and RSSI calibration. This also puts the radio in FSK mode.
    // In order to perform image and RSSI calibration, we need the radio in
    // FSK mode. To do this, we first put it in sleep mode.
    RADIO_WriteMode(MODE_STANDBY, MODULATION_FSK, 1);

    // Set frequency to do calibration at the configured frequency
    RADIO_WriteFrequency(RadioConfiguration.frequency);

    // Do not do autocalibration at runtime, start calibration now, Temp
    // threshold for monitoring 10 deg. C, Temperature monitoring enabled
    RADIO_RegisterWrite(REG_FSK_IMAGECAL, 0x42);

    // Wait for calibration to complete
    while ((RADIO_RegisterRead(REG_FSK_IMAGECAL) & 0x20) != 0)
        ;

    // High frequency LNA current adjustment, 150% LNA current (Boost on)
    RADIO_RegisterWrite(REG_LNA, 0x23);

    // Triggering event: PreambleDetect does AfcAutoOn, AgcAutoOn
    RADIO_RegisterWrite(REG_FSK_RXCONFIG, 0x1E);

    // Preamble detector on, 2 bytes trigger an interrupt, Chip errors tolerated
    // over the preamble size
    RADIO_RegisterWrite(REG_FSK_PREAMBLEDETECT, 0xAA);
    
    // Transmission starts as soon as there is a byte in the FIFO. FifoLevel
    // interrupt is generated whenever there are at least 16 bytes in FIFO.
    RADIO_RegisterWrite(REG_FSK_FIFOTHRESH, 0x8F);

    // Set FSK max payload length to 255 bytes
    RADIO_RegisterWrite(REG_FSK_PAYLOADLENGTH, 0xFF);

    // Packet mode
    RADIO_RegisterWrite(REG_FSK_PACKETCONFIG2, 1 << SHIFT6);

    // Go to LoRa mode for this register to be set
    RADIO_WriteMode(MODE_SLEEP, MODULATION_LORA, 1);
    
    // Detect RN2903A and above modules:
    //
    // Enable pull-up resistor on RB3
    // RB3 is connected to the RF switch on revH and higher
    // RB3 is connected to the SX1276 DIO3 line on revF and lower
    //
    // revH modules will read RB3 high when pull-up enabled,
    // older modules will read RB3 low when pull-up enabled
    //
    // Note:
    // minimum pull-up current supplied by PIC is 25uA
    // maximum supply current required for PE4259 (RF Switch) is 20uA
    // This provides a minimum of 5uA margin 
    RADIO_SW_POW_SetDigitalInput();
    RADIO_SW_POW_SetPullup();
    INTCON2bits.nRBPU = 0;
    for (cnt=0; !(cnt&0x80); cnt++)
    if (RADIO_SW_POW_GetValue())
    {
        RADIO_SW_POW_SetDigitalOutput();
        break;
    }

    RADIO_SW_POW_ResetPullup();
    INTCON2bits.nRBPU = 1;

    // Set LoRa max payload length
    RADIO_RegisterWrite(REG_LORA_PAYLOADMAXLENGTH, 0xFF);

    RadioConfiguration.regVersion = RADIO_RegisterRead(REG_VERSION);
}

void RADIO_SetLoRaSyncWord(uint8_t syncWord)
{
    // Change LoRa syncword
    RadioConfiguration.syncWordLoRa = syncWord;
}

uint8_t RADIO_GetLoRaSyncWord(void)
{
    return RadioConfiguration.syncWordLoRa;
}

static void RADIO_WriteConfiguration(uint16_t symbolTimeout)
{
    uint32_t tempValue;
    uint8_t regValue;
    uint8_t i;

    // Load configuration from RadioConfiguration_t structure into radio
    RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
    RADIO_WriteFrequency(RadioConfiguration.frequency);
    RADIO_WritePower(RadioConfiguration.outputPower);

    if (MODULATION_LORA == RadioConfiguration.modulation)
    {
        RADIO_RegisterWrite(REG_LORA_SYNCWORD, RadioConfiguration.syncWordLoRa);

        RADIO_RegisterWrite(REG_LORA_MODEMCONFIG1,
                (RadioConfiguration.bandWidth << SHIFT4) |
                (RadioConfiguration.errorCodingRate << SHIFT1) |
                (RadioConfiguration.implicitHeaderMode & 0x01));

        RADIO_RegisterWrite(REG_LORA_MODEMCONFIG2,
                (RadioConfiguration.dataRate << SHIFT4) |
                ((RadioConfiguration.crcOn & 0x01) << SHIFT2) |
                ((symbolTimeout & 0x0300) >> SHIFT8));


        // Handle frequency hopping, if necessary
        if (0 != RadioConfiguration.frequencyHopPeriod)
        {
            tempValue = RadioConfiguration.frequencyHopPeriod;
            // Multiply by BW/1000 (since period is in ms)
            switch (RadioConfiguration.bandWidth)
            {
                case BW_125KHZ:
                    tempValue *= 125;
                    break;
                case BW_250KHZ:
                    tempValue *= 250;
                    break;
                case BW_500KHZ:
                    tempValue *= 500;
                    break;
                default:
                    // Disable frequency hopping
                    tempValue = 0;
                    break;
            }
            // Divide by 2^SF
            tempValue >>= RadioConfiguration.dataRate;
        }
        else
        {
            tempValue = 0;
        }
        RADIO_RegisterWrite(REG_LORA_HOPPERIOD, (uint8_t)tempValue);

        RADIO_RegisterWrite(REG_LORA_SYMBTIMEOUTLSB, (symbolTimeout & 0xFF));

        // If the symbol time is > 16ms, LowDataRateOptimize needs to be set
        // This long symbol time only happens for SF12&BW125, SF12&BW250
        // and SF11&BW125 and the following if statement checks for these
        // conditions
        regValue = RADIO_RegisterRead(REG_LORA_MODEMCONFIG3);
        if (
            (
             (SF_12 == RadioConfiguration.dataRate) &&
             ((BW_125KHZ == RadioConfiguration.bandWidth) || (BW_250KHZ == RadioConfiguration.bandWidth))
            ) ||
            (
             (SF_11 == RadioConfiguration.dataRate) &&
             (BW_125KHZ == RadioConfiguration.bandWidth)
            )
           )
        {
            regValue |= 1 << SHIFT3;     // Set LowDataRateOptimize
        }
        else
        {
            regValue &= ~(1 << SHIFT3);    // Clear LowDataRateOptimize
        }
        regValue |= 1 << SHIFT2;         // LNA gain set by internal AGC loop
        RADIO_RegisterWrite(REG_LORA_MODEMCONFIG3, regValue);

        regValue = RADIO_RegisterRead(REG_LORA_DETECTOPTIMIZE);
        regValue &= ~(0x07);        // Clear DetectOptimize bits
        regValue |= 0x03;           // Set value for SF7 - SF12
        RADIO_RegisterWrite(REG_LORA_DETECTOPTIMIZE, regValue);

        // Also set DetectionThreshold value for SF7 - SF12
        RADIO_RegisterWrite(REG_LORA_DETECTIONTHRESHOLD, 0x0A);
        
        // Errata settings to mitigate spurious reception of a LoRa Signal
        if (0x12 == RadioConfiguration.regVersion)
        {
            // Chip already is in sleep mode. For these BWs we don't need to
            // offset Frf
            if ( (BW_125KHZ == RadioConfiguration.bandWidth) ||
                 (BW_250KHZ == RadioConfiguration.bandWidth) )
            {
                regValue = RADIO_RegisterRead(0x31);
                regValue &= ~0x80;                                  // Clear bit 7
                RADIO_RegisterWrite(0x31, regValue);
                RADIO_RegisterWrite(0x2F, 0x40);
                RADIO_RegisterWrite(0x30, 0x00);
            }
            
            if (BW_500KHZ == RadioConfiguration.bandWidth)
            {
                regValue = RADIO_RegisterRead(0x31);
                regValue |= 0x80;                                   // Set bit 7
                RADIO_RegisterWrite(0x31, regValue);
            }
        }

        regValue = RADIO_RegisterRead(REG_LORA_INVERTIQ);
        regValue &= ~(1 << SHIFT6);                                        // Clear InvertIQ bit
        regValue |= (1 << SHIFT0);
        regValue |= (RadioConfiguration.iqInverted & 0x01) << SHIFT6;    // Set InvertIQ bit if needed   
        RADIO_RegisterWrite(REG_LORA_INVERTIQ, regValue);
        
        regValue = REG_LORA_INVERTIQ2_VALUE_OFF & (~((RadioConfiguration.iqInverted & 0x01) << SHIFT2));
        RADIO_RegisterWrite(REG_LORA_INVERTIQ2, regValue);

        RADIO_RegisterWrite(REG_LORA_PREAMBLEMSB, RadioConfiguration.preambleLen >> SHIFT8);
        RADIO_RegisterWrite(REG_LORA_PREAMBLELSB, RadioConfiguration.preambleLen & 0xFF);

        RADIO_RegisterWrite(REG_LORA_FIFOADDRPTR, 0x00);
        RADIO_RegisterWrite(REG_LORA_FIFOTXBASEADDR, 0x00);
        RADIO_RegisterWrite(REG_LORA_FIFORXBASEADDR, 0x00);

        // Errata sensitivity increase for 500kHz BW
        if (0x12 == RadioConfiguration.regVersion)
        {
            if ( (BW_500KHZ == RadioConfiguration.bandWidth) &&
                 (RadioConfiguration.frequency >= FREQ_862000KHZ) &&
                 (RadioConfiguration.frequency <= FREQ_1020000KHZ)
               )
            {
                RADIO_RegisterWrite(0x36, 0x02);
                RADIO_RegisterWrite(0x3a, 0x64);
            }
            else if ( (BW_500KHZ == RadioConfiguration.bandWidth) &&
                      (RadioConfiguration.frequency >= FREQ_410000KHZ) &&
                      (RadioConfiguration.frequency <= FREQ_525000KHZ)
                    )
            {
                RADIO_RegisterWrite(0x36, 0x02);
                RADIO_RegisterWrite(0x3a, 0x7F);
            }
            else
            {
                RADIO_RegisterWrite(0x36, 0x03);
            }

            // LoRa Inverted Polarity 500kHz fix (May 26, 2015 document)
            if ((BW_500KHZ == RadioConfiguration.bandWidth) && (1 == RadioConfiguration.iqInverted))
            {
                RADIO_RegisterWrite(0x3A, 0x65);     // Freq to time drift
                RADIO_RegisterWrite(REG_LORA_INVERTIQ2, 25);       // Freq to time invert = 0d25
            }
            else
            {
                RADIO_RegisterWrite(0x3A, 0x65);     // Freq to time drift
                RADIO_RegisterWrite(REG_LORA_INVERTIQ2, 29);       // Freq to time invert = 0d29 (default)
            }
        }

        // Clear all interrupts (just in case)
        RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 0xFF);
    }
    else
    {
        // FSK modulation
        RADIO_WriteFSKFrequencyDeviation(RadioConfiguration.frequencyDeviation);
        RADIO_WriteFSKBitRate(RadioConfiguration.bitRate);

        RADIO_RegisterWrite(REG_FSK_PREAMBLEMSB, RadioConfiguration.preambleLen >> SHIFT8);
        RADIO_RegisterWrite(REG_FSK_PREAMBLELSB, RadioConfiguration.preambleLen & 0xFF);

        // Configure PaRamp
        regValue = RADIO_RegisterRead(REG_PARAMP);
        regValue &= ~0x60;    // Clear shaping bits
        regValue |= RadioConfiguration.fskDataShaping << SHIFT5;
        RADIO_RegisterWrite(REG_PARAMP, regValue);

        // Variable length packets, whitening, keep FIFO when CRC fails
        // no address filtering, CCITT CRC and whitening
        regValue = 0xC8;
        if (RadioConfiguration.crcOn)
        {
            regValue |= 0x10;   // Enable CRC
        }
        RADIO_RegisterWrite(REG_FSK_PACKETCONFIG1, regValue);

        // Syncword value
        for (i = 0; i < RadioConfiguration.syncWordLen; i++)
        {
            // Take advantage of the fact that the SYNCVALUE registers are
            // placed at sequential addresses
            RADIO_RegisterWrite(REG_FSK_SYNCVALUE1 + i, RadioConfiguration.syncWord[i]);
        }

        // Enable sync word generation/detection if needed, Syncword size = syncWordLen + 1 bytes
        if (RadioConfiguration.syncWordLen != 0)
        {
            RADIO_RegisterWrite(REG_FSK_SYNCCONFIG, 0x10 | (RadioConfiguration.syncWordLen - 1));
        }
        else
        {
            RADIO_RegisterWrite(REG_FSK_SYNCCONFIG, 0x00);
        }

        // Clear all FSK interrupts (just in case)
        RADIO_RegisterWrite(REG_FSK_IRQFLAGS1, 0xFF);
        RADIO_RegisterWrite(REG_FSK_IRQFLAGS2, 0xFF);

    }
}

RadioError_t RADIO_TransmitCW(void)
{
    if ((RadioConfiguration.flags & (RADIO_FLAG_TRANSMITTING | RADIO_FLAG_RECEIVING)) != 0)
    {
        return ERR_RADIO_BUSY;
    }

    // The RadioTransmitCW function is guaranteed to turn on the radio after this
    // point. The RF switch needs to be turned on by ~10us before the transmission
    // begins. The SPI runs at 4 MHz so 10 us is the time it takes to move 8
    // bytes between the MCU and the radio. The RadioTransmitCW function still has
    // a lot of configuration to do from this point on, so it will certainly
    // take longer than 10us. Turning on the RF switch now.
    RADIO_SW_POW_SetHigh();

    RadioConfiguration.modulation = MODULATION_LORA;

    // Since we're interested in a transmission, rxWindowSize is irrelevant.
    // Setting it to 4 is a valid option.
    RADIO_WriteConfiguration(4);

    RadioConfiguration.flags |= RADIO_FLAG_TRANSMITTING;
    RadioConfiguration.flags &= ~RADIO_FLAG_TIMEOUT;

    RADIO_RegisterWrite(0x3D, 0xA1);
    RADIO_RegisterWrite(0x36, 0x01);
    RADIO_RegisterWrite(0x1E, 0x08);
    RADIO_RegisterWrite(0x01, 0x8B);

    return ERR_NONE;
}

RadioError_t RADIO_StopCW(void)
{

    RADIO_WriteMode(MODE_STANDBY, RadioConfiguration.modulation, 0);
    SystemBlockingWaitMs(100);
    RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
    SystemBlockingWaitMs(100);

    // Turning off the RF switch now.
    RADIO_SW_POW_SetLow();

    return ERR_NONE;
}

RadioError_t RADIO_Transmit(uint8_t *buffer, uint8_t bufferLen)
{
    uint8_t regValue;
    uint8_t i;

    if ((RadioConfiguration.flags & RADIO_FLAG_RXDATA) != 0)
    {
        return ERR_BUFFER_LOCKED;
    }

    if ((RadioConfiguration.flags & (RADIO_FLAG_TRANSMITTING | RADIO_FLAG_RECEIVING)) != 0)
    {
        return ERR_RADIO_BUSY;
    }

    if ((MODULATION_FSK == RadioConfiguration.modulation) && (bufferLen > 64))
    {
        return ERR_DATA_SIZE;
    }

    // The RadioTransmit function is guaranteed to turn on the radio after this
    // point. The RF switch needs to be turned on by ~10us before the transmission
    // begins. The SPI runs at 4 MHz so 10 us is the time it takes to move 8
    // bytes between the MCU and the radio. The RadioTransmit function still has
    // a lot of configuration to do from this point on, so it will certainly
    // take longer than 10us. Turning on the RF switch now.
    RADIO_SW_POW_SetHigh();

    SwTimerStop(RadioConfiguration.timeOnAirTimerId);

    // Since we're interested in a transmission, rxWindowSize is irrelevant.
    // Setting it to 4 is a valid option.
    RADIO_WriteConfiguration(4);

    if (MODULATION_LORA == RadioConfiguration.modulation)
    {
        RADIO_RegisterWrite(REG_LORA_PAYLOADLENGTH, bufferLen);

        // Configure PaRamp
        regValue = RADIO_RegisterRead(REG_PARAMP);
        regValue &= ~0x0F;    // Clear lower 4 bits
        regValue |= 0x08;     // 50us PA Ramp-up time
        RADIO_RegisterWrite(REG_PARAMP, regValue);

        // DIO0 = 01 means TxDone in LoRa mode.
        // DIO2 = 00 means FHSSChangeChannel
        RADIO_RegisterWrite(REG_DIOMAPPING1, 0x40);
        RADIO_RegisterWrite(REG_DIOMAPPING2, 0x00);

        RADIO_WriteMode(MODE_STANDBY, RadioConfiguration.modulation, 1);
    }
    else
    {
        // DIO0 = 00 means PacketSent in FSK Tx mode
        RADIO_RegisterWrite(REG_DIOMAPPING1, 0x00);
        RADIO_RegisterWrite(REG_DIOMAPPING2, 0x00);
    }

    if (MODULATION_FSK == RadioConfiguration.modulation)
    {
        // FSK requires the length to be sent to the FIFO
        RADIO_RegisterWrite(REG_FIFO, bufferLen);
    }

    HALSPICSAssert();
    HALSPISend(REG_WRITE | REG_FIFO);

    for (i = 0; i < bufferLen; i++)
    {
        HALSPISend(buffer[i]);
    }
    HALSPICSDeassert();

    RadioConfiguration.flags |= RADIO_FLAG_TRANSMITTING;
    RadioConfiguration.flags &= ~(RADIO_FLAG_TIMEOUT | RADIO_FLAG_RXERROR);

    // Non blocking switch. We don't really care when it starts transmitting.
    // If accurate timing of the time on air is required, the simplest way to
    // achieve it is to change this to a blocking mode switch.
    RADIO_WriteMode(MODE_TX, RadioConfiguration.modulation, 0);

    // Set timeout to some very large value since the timer counts down.
    // Leaving the callback uninitialized it will assume the default value of
    // NULL which in turns means no callback.
    SwTimerSetTimeout(RadioConfiguration.timeOnAirTimerId, MS_TO_TICKS(TIME_ON_AIR_LOAD_VALUE));
    SwTimerStart(RadioConfiguration.timeOnAirTimerId);

    if (0 != RadioConfiguration.watchdogTimerTimeout)
    {
        SwTimerSetTimeout(RadioConfiguration.watchdogTimerId, MS_TO_TICKS(RadioConfiguration.watchdogTimerTimeout));
        SwTimerStart(RadioConfiguration.watchdogTimerId);
    }

    return ERR_NONE;
}

// rxWindowSize parameter is in symbols for LoRa and ms for FSK
RadioError_t RADIO_ReceiveStart(uint16_t rxWindowSize)
{
#if defined(DEBUGRX2)
   // debug bct
    {
    uint8_t debug_buf[20];
    strcpy(debug_buf, "### Rx: start");
    Parser_TxAddReply(debug_buf, strlen(debug_buf));
    }
#endif // DEBUGRX2
    if ((RadioConfiguration.flags & RADIO_FLAG_RXDATA) != 0)
    {
        return ERR_BUFFER_LOCKED;
    }

    if ((RadioConfiguration.flags & (RADIO_FLAG_TRANSMITTING | RADIO_FLAG_RECEIVING)) != 0)
    {
        return ERR_RADIO_BUSY;
    }

    // The RadioReceiveStart function is guaranteed to turn on the radio after this
    // point. The RF switch needs to be turned on by ~10us before the transmission
    // begins. The SPI runs at 4 MHz so 10 us is the time it takes to move 8
    // bytes between the MCU and the radio. The RadioReceiveStart function still has
    // a lot of configuration to do from this point on, so it will certainly
    // take longer than 10us. Turning on the RF switch now.
    RADIO_SW_POW_SetHigh();

    if (0 == rxWindowSize)
    {
        RADIO_WriteConfiguration(4);
    }
    else
    {
        RADIO_WriteConfiguration(rxWindowSize);
    }

    if (MODULATION_LORA == RadioConfiguration.modulation)
    {
        // All LoRa packets are received with explicit header, so this register
        // is not used. However, a value of 0 is not allowed.
        RADIO_RegisterWrite(REG_LORA_PAYLOADLENGTH, 0x01);

        // DIO0 = 00 means RxDone in LoRa mode
        // DIO1 = 00 means RxTimeout in LoRa mode
        // DIO2 = 00 means FHSSChangeChannel
        // Other DIOs are unused.
        RADIO_RegisterWrite(REG_DIOMAPPING1, 0x00);
        RADIO_RegisterWrite(REG_DIOMAPPING2, 0x00);
    }
    else
    {
        RADIO_RegisterWrite(REG_FSK_RXBW, RadioConfiguration.rxBw);
        RADIO_RegisterWrite(REG_FSK_AFCBW, RadioConfiguration.afcBw);

        // DIO0 = 00 means PayloadReady in FSK Rx mode
        RADIO_RegisterWrite(REG_DIOMAPPING1, 0x00);
        RADIO_RegisterWrite(REG_DIOMAPPING2, 0x00);
    }

    RadioConfiguration.flags |= RADIO_FLAG_RECEIVING;
    RadioConfiguration.flags &= ~(RADIO_FLAG_TIMEOUT | RADIO_FLAG_RXERROR);

    // Will use non blocking switches to RadioSetMode. We don't really care
    // when it starts receiving.
    if (0 == rxWindowSize)
    {
        RADIO_WriteMode(MODE_RXCONT, RadioConfiguration.modulation, 0);
    }
    else
    {
        if (MODULATION_LORA == RadioConfiguration.modulation)
        {
            RADIO_WriteMode(MODE_RXSINGLE, MODULATION_LORA, 0);
        }
        else
        {
            RADIO_WriteMode(MODE_RXCONT, MODULATION_FSK, 0);
            SwTimerSetTimeout(RadioConfiguration.fskRxWindowTimerId, MS_TO_TICKS_SHORT(rxWindowSize));
            SwTimerStart(RadioConfiguration.fskRxWindowTimerId);
        }
    }

    if (0 != RadioConfiguration.watchdogTimerTimeout)
    {
        SwTimerSetTimeout(RadioConfiguration.watchdogTimerId, MS_TO_TICKS(RadioConfiguration.watchdogTimerTimeout));
        SwTimerStart(RadioConfiguration.watchdogTimerId);
    }
    return ERR_NONE;
}


void RADIO_ReceiveStop(void)
{
#if defined(DEBUGRX2)
   // debug bct
    {
    uint8_t debug_buf[20];
    strcpy(debug_buf, "### Rx: stop");
    Parser_TxAddReply(debug_buf, strlen(debug_buf));
    }
#endif // DEBUGRX2
    if (RADIO_FLAG_RECEIVING == RadioConfiguration.flags)
    {
        RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
        RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;
        
		// Turning off the RF switch now.
        RADIO_SW_POW_SetLow();
    }
}


static void RADIO_RxDone(void)
{
    uint8_t i, irqFlags;
    uint8_t rssi;
    
#if defined(DEBUGRX2)
   // debug bct
    {
    uint8_t debug_buf[20];
    strcpy(debug_buf, "### Rx: done");
    Parser_TxAddReply(debug_buf, strlen(debug_buf));
    }
#endif // DEBUGRX2
    irqFlags = RADIO_RegisterRead(REG_LORA_IRQFLAGS);
    // Clear RxDone interrupt (also CRC error and ValidHeader interrupts, if
    // they exist)
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, (1<<SHIFT6) | (1<<SHIFT5) | (1<<SHIFT4));
    if (((1<<SHIFT6) | (1<<SHIFT4)) == (irqFlags & ((1<<SHIFT6) | (1<<SHIFT4))))
    {
        // Make sure the watchdog won't trigger MAC functions erroneously.
        SwTimerStop(RadioConfiguration.watchdogTimerId);

        // Turning off the RF switch now.
        RADIO_SW_POW_SetLow();
        
        // Read CRC info from received packet header
        i = RADIO_RegisterRead(REG_LORA_HOPCHANNEL);
        if ((0 == RadioConfiguration.crcOn) || ((0 == (irqFlags & (1<<SHIFT5))) && (0 != (i & (1<<SHIFT6)))))
        {
            // ValidHeader and RxDone are set from the initial if condition.
            // We get here either if CRC doesn't need to be set (crcOn == 0) OR
            // if it is present in the header and it checked out.

            // Radio did not go to standby automatically. Will need to be set
            // later on.
            
            RadioConfiguration.dataBufferLen = RADIO_RegisterRead(REG_LORA_RXNBBYTES);
            RADIO_RegisterWrite(REG_LORA_FIFOADDRPTR, 0x00);

            HALSPICSAssert();
            HALSPISend(REG_FIFO);
            for (i = 0; i < RadioConfiguration.dataBufferLen; i++)
            {
                RadioConfiguration.dataBuffer[i] = HALSPISend(0xFF);
            }
            HALSPICSDeassert();
            RadioConfiguration.flags |= RADIO_FLAG_RXDATA;

            RadioConfiguration.packetSNR = RADIO_RegisterRead(REG_LORA_PKTSNRVALUE);
            RadioConfiguration.packetSNR /= (int8_t)4;
            rssi = RADIO_RegisterRead(REG_LORA_PKTRSSIVALUE);
            RadioConfiguration.packetRSSI = rssi - 157;
            if (RadioConfiguration.packetSNR < 0)
                RadioConfiguration.packetRSSI += RadioConfiguration.packetSNR;
            else
                RadioConfiguration.packetRSSI += rssi >> 4;
        }
        else
        {
            // CRC required and CRC error found.
            RadioConfiguration.flags |= RADIO_FLAG_RXERROR;
        }
        RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
        RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;
        LORAWAN_RxDone(RadioConfiguration.dataBuffer, RadioConfiguration.dataBufferLen);
    }
}

static void RADIO_FSKPayloadReady(void)
{
    uint8_t irqFlags;
    uint8_t i;
    int16_t instRSSI = -(RADIO_RegisterRead(REG_FSK_RSSIVALUE)>>1) ;
    RadioConfiguration.packetRSSI = instRSSI ;
    
    irqFlags = RADIO_RegisterRead(REG_FSK_IRQFLAGS2);
    if ((1<<SHIFT2) == (irqFlags & (1<<SHIFT2)))
    {
        // Clearing of the PayloadReady (and CrcOk) interrupt is done when the
        // FIFO is empty

        // Make sure the watchdog won't trigger MAC functions erroneously.
        SwTimerStop(RadioConfiguration.watchdogTimerId);
        SwTimerStop(RadioConfiguration.fskRxWindowTimerId);

        // Turning off the RF switch now.
        RADIO_SW_POW_SetLow();

        HALSPICSAssert();
        HALSPISend(REG_FIFO);
        RadioConfiguration.dataBufferLen = HALSPISend(0xFF);
        for (i = 0; i < RadioConfiguration.dataBufferLen; i++)
        {
            RadioConfiguration.dataBuffer[i] = HALSPISend(0xFF);
        }
        HALSPICSDeassert();

        RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
        RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;

        RadioConfiguration.packetSNR = -128;

        if (1 == RadioConfiguration.crcOn)
        {
            if ((1<<SHIFT1) == (irqFlags & (1<<SHIFT1)))
            {
                RadioConfiguration.flags |= RADIO_FLAG_RXDATA;
            }
            else
            {
                RadioConfiguration.flags &= ~RADIO_FLAG_RXDATA;
            }
        }
        else
        {
            RadioConfiguration.flags |= RADIO_FLAG_RXDATA;
        }

        if ((RadioConfiguration.flags & RADIO_FLAG_RXDATA) != 0)
        {
            LORAWAN_RxDone(RadioConfiguration.dataBuffer, RadioConfiguration.dataBufferLen);
        }
        else
        {
            LORAWAN_RxTimeout();
        }
    }
}

static void RADIO_RxTimeout(void)
{
#if defined(DEBUGRX2)
   // debug bct
    {
    uint8_t debug_buf[20];
    strcpy(debug_buf, "### Rx: time");
    Parser_TxAddReply(debug_buf, strlen(debug_buf));
    }
#endif // DEBUGRX2
    // Make sure the watchdog won't trigger MAC functions erroneously.
    SwTimerStop(RadioConfiguration.watchdogTimerId);

    // Turning off the RF switch now.
    RADIO_SW_POW_SetLow();
	
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 1<<SHIFT7);
    // Radio went to STANDBY. Set sleep.
    RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
    RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;

    LORAWAN_RxTimeout ();
}

static void RADIO_TxDone(void)
{
    uint32_t timeOnAir;
    // Make sure the watchdog won't trigger MAC functions erroneously.
    SwTimerStop(RadioConfiguration.watchdogTimerId);

    // Turning off the RF switch now.
    RADIO_SW_POW_SetLow();
	
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 1<<SHIFT3);
    RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
    RadioConfiguration.flags &= ~RADIO_FLAG_TRANSMITTING;
    if ((RadioConfiguration.flags & RADIO_FLAG_TIMEOUT) == 0)
    {
        timeOnAir = TIME_ON_AIR_LOAD_VALUE - TICKS_TO_MS(SwTimerReadValue(RadioConfiguration.timeOnAirTimerId));
        LORAWAN_TxDone((uint16_t)timeOnAir);
    }
}

static void RADIO_FSKPacketSent(void)
{
    uint8_t irqFlags;
    uint32_t timeOnAir;

    irqFlags = RADIO_RegisterRead(REG_FSK_IRQFLAGS2);
    if ((1<<SHIFT3) == (irqFlags & (1<<SHIFT3)))
    {
        RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
        RadioConfiguration.flags &= ~RADIO_FLAG_TRANSMITTING;
        // Make sure the watchdog won't trigger MAC functions erroneously.
        SwTimerStop(RadioConfiguration.watchdogTimerId);

        // Turning off the RF switch now.
        RADIO_SW_POW_SetLow();
	
        // Clearing of the PacketSent interrupt is done on exiting Tx
        if ((RadioConfiguration.flags & RADIO_FLAG_TIMEOUT) == 0)
        {
            timeOnAir = TIME_ON_AIR_LOAD_VALUE - TICKS_TO_MS(SwTimerReadValue(RadioConfiguration.timeOnAirTimerId));
            LORAWAN_TxDone((uint16_t)timeOnAir);
        }
    }
}


uint8_t RADIO_GetStatus(void)
{
#if !defined(DISABLE_BUSY_WORKAROUND)
    /* TODO: FIXME:
     * This if statement in its entirety was added as a hack to prevent an
     * early call to LORAWAN_ReceiveWindow2Callback from locking the
     * loRa.macStatus.macState in a non IDLE state.  The root issue is that
     * the LORAWAN_ReceiveWindow2Callback is sometimes called about 1 second
     * earlier than it should. This workaround, while logically valid, masks
     * issue early call issue.
     * REG_FSK_RES18 is also Semtech RegModemStat
     *      BIT0 = Signal Detected (preamble started)
     *      BIT1 = Signal Synchronized (preamble detected modem in lock)
     *      BIT3 = Header Info Valid (Header CRC is correct) */
    if (RADIO_RegisterRead(REG_FSK_RES18) & (BIT0 | BIT1 | BIT3))
    {
        RadioConfiguration.flags |= RADIO_FLAG_RECEIVING;
    }
    else
    {
        RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;
    }
#endif // BUSY_WORKAROUND
    return RadioConfiguration.flags;
}

RadioError_t RADIO_GetData(uint8_t *data, uint16_t *dataLen)
{
    if ((RadioConfiguration.flags & RADIO_FLAG_RXDATA) == 0)
    {
        return ERR_NO_DATA;
    }

    memcpy(data, RadioConfiguration.dataBuffer, RadioConfiguration.dataBufferLen);
    *dataLen = RadioConfiguration.dataBufferLen;

    return ERR_NONE;
}

void RADIO_ReleaseData(void)
{
    RadioConfiguration.flags &= ~RADIO_FLAG_RXDATA;
}

static void RADIO_UnhandledInterrupt(RadioModulation_t modulation)
{
    // Clear all IRQ flags to recover gracefully.
    // Since we already know the radio modulation settings we can go access
    // those registers directly.
    if (MODULATION_LORA == modulation)
    {
        RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 0xFF);
    }
    else
    {
        // Although just some of the bits can be cleared, try to clear
        // everything
        RADIO_RegisterWrite(REG_FSK_IRQFLAGS1, 0xFF);
        RADIO_RegisterWrite(REG_FSK_IRQFLAGS2, 0xFF);
    }
}


static void RADIO_FHSSChangeChannel(void)
{
    uint8_t irqFlags;
    irqFlags = RADIO_RegisterRead(REG_LORA_IRQFLAGS);
   
    if (NULL != RadioConfiguration.frequencyHopPeriod)
    {
        if (NULL != RadioConfiguration.fhssNextFrequency)
        {
//            RADIO_WriteFrequency(RadioConfiguration.fhssNextFrequency());
        }
    }

    // Clear FHSSChangeChannel interrupt
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 1<<SHIFT1);
}


void RADIO_DIO0(void)
{
    // Check radio configuration (modulation and DIO0 settings).
    uint8_t dioMapping;
    uint8_t opMode;
    dioMapping = (RADIO_RegisterRead(REG_DIOMAPPING1) & 0xC0) >> SHIFT6;
    opMode = RADIO_RegisterRead(REG_OPMODE);

    if ((opMode & 0x80) != 0)
    {
        // LoRa modulation
        switch (dioMapping)
        {
            case 0x00:
                RADIO_RxDone();
                break;
            case 0x01:
                RADIO_TxDone();
                break;
            default:
                RADIO_UnhandledInterrupt(MODULATION_LORA);
                break;
        }
    }
    else
    {
        // FSK modulation
        switch (dioMapping)
        {
            case 0x00:
                // Check if the radio state is Tx or Rx
                opMode &= 0x07;
                if (MODE_TX == opMode)
                {
                    // PacketSent
                    RADIO_FSKPacketSent();
                }
                else if (MODE_RXCONT == opMode)
                {
                    // PayloadReady
                    RADIO_FSKPayloadReady();
                }
                else
                {
                    RADIO_UnhandledInterrupt(MODULATION_FSK);
                }
                break;
            default:
                RADIO_UnhandledInterrupt(MODULATION_FSK);
                break;
        }
    }
}


void RADIO_DIO1(void)
{
    // Check radio configuration (modulation and DIO1 settings).
    uint8_t dioMapping;
    dioMapping = (RADIO_RegisterRead(REG_DIOMAPPING1) & 0x30) >> SHIFT4;

    if ((RADIO_RegisterRead(REG_OPMODE) & 0x80) != 0)
    {
        // LoRa modulation
        switch (dioMapping)
        {
            case 0x00:
                RADIO_RxTimeout();
                break;
            case 0x01:
                RADIO_FHSSChangeChannel();
                break;
            default:
                RADIO_UnhandledInterrupt(MODULATION_LORA);
                break;
        }
    }
    else
    {
        // FSK modulation
        switch (dioMapping)
        {
            case 0x00:
                // RADIO_FSKFifoLevel();
                break;
            default:
                // RADIO_UnhandledInterrupt(MODULATION_FSK);
                break;
        }
    }
}

void RADIO_DIO2(void)
{
    // Check radio configuration (modulation and DIO2 settings).
    uint8_t dioMapping;
    uint8_t opMode;
    dioMapping = (RADIO_RegisterRead(REG_DIOMAPPING1) & 0x0C) >> SHIFT2;
    opMode = RADIO_RegisterRead(REG_OPMODE);

    if ((RADIO_RegisterRead(REG_OPMODE) & 0x80) != 0)
    {
        // LoRa modulation
        switch (dioMapping)
        {
            // Intentional fall-through
            case 0x00:
            case 0x01:
            case 0x02:
                RADIO_FHSSChangeChannel();
                break;
            default:
                RADIO_UnhandledInterrupt(MODULATION_LORA);
                break;
        }
    }
    else
    {
        // FSK modulation
    }
}

void RADIO_DIO3(void)
{
    // Check radio configuration (modulation and DIO3 settings).
    uint8_t dioMapping;
    dioMapping = RADIO_RegisterRead(REG_DIOMAPPING1) & 0x03;

    if ((RADIO_RegisterRead(REG_OPMODE) & 0x80) != 0)
    {
        // LoRa modulation
        switch (dioMapping)
        {
            default:
                RADIO_UnhandledInterrupt(MODULATION_LORA);
                break;
        }
    }
    else
    {
        // FSK modulation
        // RADIO_UnhandledInterrupt(MODULATION_FSK);
    }
}

void RADIO_DIO4(void)
{
    // Check radio configuration (modulation and DIO4 settings).
    uint8_t dioMapping;
    dioMapping = (RADIO_RegisterRead(REG_DIOMAPPING2) & 0xC0) >> SHIFT6;

    if ((RADIO_RegisterRead(REG_OPMODE) & 0x80) != 0)
    {
        // LoRa modulation
        switch (dioMapping)
        {
            default:
                RADIO_UnhandledInterrupt(MODULATION_LORA);
                break;
        }
    }
    else
    {
        // FSK modulation
        // RADIO_UnhandledInterrupt(MODULATION_FSK);
    }
}

void RADIO_DIO5(void)
{
    // Check radio configuration (modulation and DIO5 settings).
    uint8_t dioMapping;
    dioMapping = (RADIO_RegisterRead(REG_DIOMAPPING2) & 0x30) >> SHIFT4;

    if ((RADIO_RegisterRead(REG_OPMODE) & 0x80) != 0)
    {
        // LoRa modulation
        switch (dioMapping)
        {
            default:
                RADIO_UnhandledInterrupt(MODULATION_LORA);
                break;
        }
    }
    else
    {
        // FSK modulation
        // RADIO_UnhandledInterrupt(MODULATION_FSK);
    }
}

uint16_t RADIO_ReadRandom(void)
{
    uint8_t i;
    uint16_t retVal;
    retVal = 0;

    // Turn on the RF switch.
    RADIO_SW_POW_SetHigh();
	
    // Mask all interrupts, do many measurements of RSSI
    RADIO_WriteMode(MODE_SLEEP, MODULATION_LORA, 1);
    RADIO_RegisterWrite(REG_LORA_IRQFLAGSMASK, 0xFF);
    RADIO_WriteMode(MODE_RXCONT, MODULATION_LORA, 1);
    for (i = 0; i < 16; i++)
    {
        SystemBlockingWaitMs(1);
        retVal <<= SHIFT1;
        retVal |= RADIO_RegisterRead(REG_LORA_RSSIWIDEBAND) & 0x01;
    }

    // Turn off the RF switch.
    RADIO_SW_POW_SetLow();
	

    // Return radio to sleep
    RADIO_WriteMode(MODE_SLEEP, MODULATION_LORA, 1);
    // Clear interrupts in case any have been generated
    RADIO_RegisterWrite(REG_LORA_IRQFLAGS, 0xFF);
    // Unmask all interrupts
    RADIO_RegisterWrite(REG_LORA_IRQFLAGSMASK, 0x00);
    return retVal;
}

static void RADIO_RxFSKTimeout(uint8_t param)
{
    uint8_t irqFlags;
    irqFlags = RADIO_RegisterRead(REG_FSK_IRQFLAGS1);
    if (0 == (irqFlags & (1<<SHIFT0)))
    {
        // SyncAddressMatch is not set. Set radio to sleep.
        RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);
        // Make sure the watchdog won't trigger MAC functions erroneously.
        SwTimerStop(RadioConfiguration.watchdogTimerId);

        // Turning off the RF switch now.
        RADIO_SW_POW_SetLow();
	
        RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;
        LORAWAN_RxTimeout();
    }
}


static void RADIO_WatchdogTimeout(uint8_t param)
{
#if defined(DEBUGRX2)
   // debug bct
    {
    uint8_t debug_buf[20];
    strcpy(debug_buf, "### Rx: wdt");
    Parser_TxAddReply(debug_buf, strlen(debug_buf));
    }
#endif // DEBUGRX2
    RADIO_WriteMode(MODE_STANDBY, RadioConfiguration.modulation, 1);
    RADIO_WriteMode(MODE_SLEEP, RadioConfiguration.modulation, 0);

    // Turning off the RF switch now.
    RADIO_SW_POW_SetLow();
	
    RadioConfiguration.flags |= RADIO_FLAG_TIMEOUT;
    if ((RadioConfiguration.flags & RADIO_FLAG_RECEIVING) != 0)
    {
        RadioConfiguration.flags &= ~RADIO_FLAG_RECEIVING;
        LORAWAN_RxTimeout();
    }
    else if ((RadioConfiguration.flags & RADIO_FLAG_TRANSMITTING) != 0)
    {
        RadioConfiguration.flags &= ~RADIO_FLAG_TRANSMITTING;
        // This will tell the MAC that the channel has been used a lot. Since
        // this time-out occured we cannot know for sure that the radio did not
        // transmit this whole time, so this is the safest way to go (block the
        // channel for a really long time from now on).
        LORAWAN_TxDone(RadioConfiguration.watchdogTimerTimeout);
    }
}

int8_t RADIO_GetPacketSnr(void)
{
    return RadioConfiguration.packetSNR;
}

int16_t RADIO_GetPacketRSSI(void)
{
    return RadioConfiguration.packetRSSI;
}

void RADIO_SetSpreadingFactor(RadioDataRate_t spreadingFactor)
{
    RadioConfiguration.dataRate = spreadingFactor;
}

RadioDataRate_t RADIO_GetSpreadingFactor(void)
{
    return RadioConfiguration.dataRate;
}

RadioError_t RADIO_SetChannelFrequency(uint32_t frequency)
{
    if ( (frequency >= FREQ_137000KHZ) && (frequency <= FREQ_175000KHZ) ||
         (frequency >= FREQ_410000KHZ) && (frequency <= FREQ_525000KHZ) ||
         (frequency >= FREQ_862000KHZ) && (frequency <= FREQ_1020000KHZ) )
    {
        RadioConfiguration.frequency = frequency;
        return ERR_NONE;
    }
    else
    {
        return ERR_OUT_OF_RANGE;
    }
}

uint32_t RADIO_GetChannelFrequency(void)
{
    return RadioConfiguration.frequency;
}

void RADIO_SetOutputPower(int8_t power)
{
    RadioConfiguration.outputPower = power;
}

uint8_t RADIO_GetOutputPower(void)
{
    return RadioConfiguration.outputPower;
}

void RADIO_SetCRC(uint8_t crc)
{
    RadioConfiguration.crcOn = crc;
}

uint8_t RADIO_GetCRC(void)
{
    return RadioConfiguration.crcOn;
}

void RADIO_SetIQInverted(uint8_t iqInverted)
{
    RadioConfiguration.iqInverted = iqInverted;
}

uint8_t RADIO_GetIQInverted(void)
{
    return RadioConfiguration.iqInverted;
}


void RADIO_SetBandwidth(RadioLoRaBandWidth_t bandwidth)
{
    RadioConfiguration.bandWidth = bandwidth;
}

RadioLoRaBandWidth_t RADIO_GetBandwidth(void)
{
    return RadioConfiguration.bandWidth;
}

void RADIO_SetPABoost(uint8_t paBoost)
{
    RadioConfiguration.paBoost = paBoost;
}

uint8_t RADIO_GetPABoost(void)
{
    return RadioConfiguration.paBoost;
}

void RADIO_SetModulation(RadioModulation_t modulation)
{
    RadioConfiguration.modulation = modulation;
}

RadioModulation_t RADIO_GetModulation(void)
{
    return RadioConfiguration.modulation;
}

void RADIO_SetChannelFrequencyDeviation(uint32_t frequencyDeviation)
{
    RadioConfiguration.frequencyDeviation = frequencyDeviation;
}

uint32_t RADIO_GetChannelFrequencyDeviation(void)
{
    return RadioConfiguration.frequencyDeviation;
}

void RADIO_SetPreambleLen(uint16_t preambleLen)
{
    RadioConfiguration.preambleLen = preambleLen;
}

uint16_t RADIO_GetPreambleLen(void)
{
    return RadioConfiguration.preambleLen;
}

void RADIO_SetFHSSChangeCallback(uint32_t (*fhssNextFrequency)(void))
{
    RadioConfiguration.fhssNextFrequency = fhssNextFrequency;
}

void RADIO_SetFrequencyHopPeriod(uint16_t frequencyHopPeriod)
{
    RadioConfiguration.frequencyHopPeriod = frequencyHopPeriod;
}

uint8_t RADIO_GetFrequencyHopPeriod(void)
{
    return RadioConfiguration.frequencyHopPeriod;
}


void RADIO_SetErrorCodingRate(RadioErrorCodingRate_t errorCodingRate)
{
    RadioConfiguration.errorCodingRate = errorCodingRate;
}

RadioErrorCodingRate_t RADIO_GetErrorCodingRate(void)
{
    return RadioConfiguration.errorCodingRate;
}


void RADIO_SetWatchdogTimeout(uint32_t timeout)
{
    RadioConfiguration.watchdogTimerTimeout = timeout;
}

uint32_t RADIO_GetWatchdogTimeout(void)
{
    return RadioConfiguration.watchdogTimerTimeout;
}


void RADIO_SetFSKBitRate(uint32_t bitRate)
{
    RadioConfiguration.bitRate = bitRate;
}

uint32_t RADIO_GetFSKBitRate(void)
{
    return RadioConfiguration.bitRate;
}


void RADIO_SetFSKDataShaping(RadioFSKShaping_t fskDataShaping)
{
    RadioConfiguration.fskDataShaping = fskDataShaping;
}

RadioFSKShaping_t RADIO_GetFSKDataShaping(void)
{
    return RadioConfiguration.fskDataShaping;
}

void RADIO_SetFSKRxBw(RadioFSKBandWidth_t bw)
{
    RadioConfiguration.rxBw = bw;
}

RadioFSKBandWidth_t RADIO_GetFSKRxBw(void)
{
    return RadioConfiguration.rxBw;
}

void RADIO_SetFSKAFCBw(RadioFSKBandWidth_t bw)
{
    RadioConfiguration.afcBw = bw;
}

RadioFSKBandWidth_t RADIO_GetFSKAFCBw(void)
{
    return RadioConfiguration.afcBw;
}

void RADIO_SetFSKSyncWord(uint8_t syncWordLen, uint8_t* syncWord)
{
    if (syncWordLen > 8)
    {
        syncWordLen = 8;
    }
    memcpy(RadioConfiguration.syncWord, syncWord, syncWordLen);
    RadioConfiguration.syncWordLen = syncWordLen;
}

uint8_t RADIO_GetFSKSyncWord(uint8_t* syncWord)
{
    memcpy(syncWord, RadioConfiguration.syncWord, RadioConfiguration.syncWordLen);
    return RadioConfiguration.syncWordLen;
}


/**
 End of File
*/
