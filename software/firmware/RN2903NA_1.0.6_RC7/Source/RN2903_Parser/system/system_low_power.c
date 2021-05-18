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
 *
 *                           system_low_power.c
 *
 * Low power management file
 *
 *
 * Hardware:
 *  USB Carrier with RN2903 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M15723         2014.10.12        0.5
 * M18029		  2016.09.30
 ******************************************************************************/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "../mcc_generated_files/mcc.h"
#include "../system/system_low_power.h"
#include "../mcc_generated_files/interrupt_manager.h"
#include "../mcc_generated_files/LoRaWAN/sw_timer.h"
#include "../system/system.h"
#include "../mcc_generated_files/eusart1.h"
#include "mcc_lora_config.h"

#if defined(DEBUG_SLEEP)
#include "../mcc_generated_files/pin_manager.h"
#define MCU_SLEEP()                \
                        GPIO13_SetLow();    \
                        SLEEP();            \
                        GPIO13_SetHigh()
#else // DEBUG_SLEEP
#define MCU_SLEEP()                \
                        SLEEP()
#endif // DEBUG_SLEEP

#define TICKS_SHIFT_LIMIT   6                       // shift limit before a 32-bit overflow
//#define ABD_TIMEOUT_TICKS   MS_TO_TICKS_SHORT(100)  // UART ABD must complete in less than 100 ms
#define ABD_TIMEOUT_TICKS   MS_TO_TICKS_SHORT(1*60000)  // UART ABD must complete in less than 1 minute

static void OnSleepTimerEvent(uint8_t param);
static void OnABDTimerEvent(uint8_t param);
static void SysCfgLowPower(void);
static void SysCfgRun(void);

static volatile SysSleepState_t goToSleep = SLEEP_OFF;
static volatile SysSleepState_t eusart1WakeUpState = SLEEP_ON;
static uint8_t sleepTimerId;
static uint8_t abdTimerId;
static exitFromSleepCb_t pExitFromSleepCb;
static volatile bool exitFromSleep = false;

static void UARTStartAutobaudTimer();



void SysSleepInit(exitFromSleepCb_t exitFromSleepCb)
{
    // Turn off everything the project won't ever use
    UART2MD = 1;
    TMR6MD = 1;
    TMR5MD = 1;
    TMR4MD = 1;
    TMR3MD = 1;
    TMR2MD = 1;
    CCP5MD = 1;
    CCP4MD = 1;
    CCP3MD = 1;
    CCP2MD = 1;
    CCP1MD = 1;
    CTMUMD = 1;
    CMP2MD = 1;
    CMP1MD = 1;
    ADCMD = 1;

    IDLEN = 0;
    
    sleepTimerId = SwTimerCreate();
    SwTimerSetCallback(sleepTimerId, OnSleepTimerEvent, 0U);
    
    abdTimerId   = SwTimerCreate();
    SwTimerSetCallback(abdTimerId, OnABDTimerEvent, 0U);
    
    pExitFromSleepCb = exitFromSleepCb;
}

    


void SysSleepStart(uint32_t ms)
{
    parse32bit_t    msParts;
    uint16_t        mask;
    uint8_t         shiftCount;
    uint8_t         ticksUpperByte;
    uint32_t        sleepTicks;
    
    
    /*
     * For short sleep times, times less than 0xfffff ms (~1049 seconds), use
     * the most efficient sleepTicks calculation
     */
    if((MS_TO_TICKS_LONG_THRESHOLD > ms) || (0 == ms))
    {
        // set the sleep timer timeout
        SwTimerSetTimeout(sleepTimerId, MS_TO_TICKS_SHORT(ms));
    }
    else
    {
        /*
         * Longer sleep times >= 0xfffff ms (~1049 seconds) require additional
         * effort to calculate a sleepTicks value that could exceed 32-bit in
         * size.
         * 
         * The objective here is to calculate the number of timer ticks required
         * to sleep for the number of milliseconds requested.  The issue is, the
         * number of timer ticks is greater than the number of milliseconds.  The
         * number of allowed milliseconds is the maximum unsigned 32-bit number.
         * Therefore, the number of ticks will be larger than 32-bits.
         * 
         * The compiler can perform 32-bit math, however 40-bit math is required.
         * To achieve the desired result, the number of milliseconds is shifted
         * right to prevent the calculation from overflowing 32-bits.  The upper
         * 32-bits of the 40-bit number are stored.  Then the ticks for the lower
         * bits of the milliseconds are calculated and added to the upper part.
         * 
         * This technique does introduce some rounding error, but the magnitude
         * should be small enough to be acceptable.
         */

        mask            = 0;
        shiftCount      = 0;
        ticksUpperByte  = 0;
        
        
        // save a copy of the ms time passed in
        msParts.value   = ms;



        //  Reduce the magnitude of ms to prevent calculation overflow
        //  Keeping the value as large as possible is required to maintain accuracy
        while(MS_TO_TICKS_LONG_THRESHOLD < ms)
        {
            ms       = ms >> 1;
            mask    |= 0x0001 << shiftCount;    // used to select lower bits later
            shiftCount++;                       // track amount of the shift
        }



        // convert the reduced ms value to ticks
        sleepTicks  = MS_TO_TICKS_SHORT(ms);

        // capture overflow byte before restoring the ticks magnitude
        if(TICKS_SHIFT_LIMIT < shiftCount)
        {
            ticksUpperByte  = (uint8_t)(sleepTicks >> (32 - shiftCount));
        }

        // store the ticks upper byte in the SwTimer call back parameter
        SwTimerSetCallbackParam(sleepTimerId, ticksUpperByte);



        // restore the ticks magnitude to correct for the reduction shift of the ms
        sleepTicks   = sleepTicks << shiftCount;

        // add back in the ms lower bits lost due to the right shift reduction
        sleepTicks   += MS_TO_TICKS_SHORT(msParts.low16bits & mask);

        // set the sleep timer timeout
        SwTimerSetTimeout(sleepTimerId, sleepTicks);
    }
    
    // Tell the system to prepare to sleep
    SysSetGoToSleepState(SLEEP_ON);
    
}

void SysGoToSleep(void)
{
    uint8_t     timerDisabled   = false;
    
    if(goToSleep == SLEEP_ON)
    {
        // wait for sleep to be possible. Make sure no timers are running,
        // no interrupts are pending service.
        INTERRUPT_GlobalInterruptDisable();
        if ( ( INTERRUPT_PeekDioStatus() == 0 ) && ( TXIE == 0 ) && ( TXSTA1bits.TRMT == 1 )  && (SwTimersCanSleep( ) == 1))
        {
            // MCU going to sleep. Start sleep timer.
            SysCfgLowPower();
            INTERRUPT_GlobalInterruptEnable();
            
            if(0 == SwTimerReadValue(sleepTimerId))
            {
                // A sleep timer value of (0) indicates 'sleep forever '
                // therefore the sw timer should not be started 
                // and the hw timer should be stopped
                timerDisabled    = true;
            }
            else
            {
                // Start normal timed sleep
                SwTimerStart(sleepTimerId);
            }

            //Radio default state after init is "sleep". There is no need to change radio state
            
            
            while (goToSleep == SLEEP_ON || goToSleep == SLEEP_TRANSITION_TO_OFF)
            {

                if(SLEEP_ON == goToSleep)
                {
                    EUSART1_PrepareForSleep();
                    if(timerDisabled)
                    {
                        TMR_StopTimer();
                    }
                    MCU_SLEEP();
                }
                if(TMR_RUNNING)
                {
                    SwTimersExecute();
                }
                
                // find out why we woke up  - SwTimersExecute might set SLEEP_OFF
                if(SLEEP_OFF == goToSleep)
                {
                    // sw sleep timer has expired
                    if(UART_SLEEP < EUSART1_GetOperationState())
                    {
                        // UART auto baud was interrupted
                        EUSART1_RestoreBaudrate();
                    }
                    EUSART1_ResetOperationState();  // reset UART to normal operation
                }
                else
                {
                    // sleep timer did not expire - check UART
                    switch(EUSART1_GetOperationState())
                    {
                        case UART_WUE_WAKE:
                            TMR_StartTimer();           // ensure TMR is running
                            UARTStartAutobaudTimer();
                            break;
                        case UART_CHECK_AUTOBAUD:
                            SwTimerStop(abdTimerId);  // stop the ABD timeout timer
                            if(EUSART1_CheckAutobaud())
                            {
                                SysSetGoToSleepState(SLEEP_OFF);    // prepare to wake
                            }
                            else
                            {
                                SysSetGoToSleepState(SLEEP_ON);     // go back to sleep
                            }
                           break;
                        case UART_SLEEP:
                            SysSetGoToSleepState(SLEEP_ON);
                            break;
                        default:
                            // The remaining states the UARTOperationState are
                            // handled in ISRs
                            break;
                    }
                }
                
            } // end while(SLEEP)
            
            SwTimerStop(sleepTimerId);  // ensure the sleep timer in not running
            SwTimerStop(abdTimerId);    // ensure the ABD timer is not running
            SwTimerSetPS(0);            // Remove the TMR sleep prescaler
            
        }
        INTERRUPT_GlobalInterruptEnable();
    }
}

SysSleepState_t SysGetGoToSleepState(void)
{
    return goToSleep;
}

void SysSetGoToSleepState(SysSleepState_t sleepState)
{
    if(sleepState == SLEEP_OFF)
    {
        exitFromSleep = true;
    }
    
    goToSleep = sleepState;
}

void SysExitFromSleep(void)
{   
    
    if(exitFromSleep == true)
    {
        SwTimerStop(sleepTimerId);
        
        exitFromSleep = false;
        
        SysCfgRun();

        if (pExitFromSleepCb)
        {
            pExitFromSleepCb();
        }
    }
}

void SysValidateAutobaud(void)
{
    /* During normal operation (not in sleep) if the EUSART detects a 
     * framing error, it will assume a break-autobaud sequence has been
     * initiated by the host.  This will wipe out the current EUSART
     * baudrate.  This code will start a ABD timeout timer,
     * and restore the baudrate if the ABD operation times out, or results
     * in an invalid baudrate */

    switch(EUSART1_GetOperationState())
    {
// TODO clean this up        
//        case UART_WAIT_FOR_AUTOBAUD:
//            // Upon detecting a framing error, the EUSART will initiate the
//            // ABD, and set the UART_WAIT_FOR_AUTOBAUD state.
//            
//            // start auto baud timeout timer
//            UARTStartAutobaudTimer(false);
//            break;
            
        case UART_CHECK_AUTOBAUD:
            // if the EUSART completes the ABD operation, or an ABD timeout
            // occurs, the state will be set to UART_CHECK_AUTOBAUD
            
//            SwTimerStop(abdTimerId);    // stop the ABD timeout timer
            Nop();
            Nop();
            EUSART1_CheckAutobaud();    // if ABD failed restore baudrate
           break;
           
        default:
            
            // The UARTOperationState is primarily used for break-autobaud
            // operation when the module in in System Sleep mode.
            // Only the above states are of interest in normal operation.
            break;
    }
    
}

static void OnSleepTimerEvent(uint8_t param)
{
    /* sleep timer callback parameter contains upper bits of the sleep timer*/
    if( 0 == param)
    {
        // Tell the system to wake from sleep
        SysSetGoToSleepState(SLEEP_OFF);
    } else
    {
        // decrement the sleep timer upper bits then return to sleep
        SwTimerDecCallbackParam(sleepTimerId);
        
        // set the sleep timer timeout rollover value
        SwTimerSetTimeout(sleepTimerId, 0xFFFFFFFFUL);
        
        // Restart the timer
        SwTimerStart(sleepTimerId);
    }
}

static void OnABDTimerEvent(uint8_t param)
{
    // Auto baud sequence timed out - go back to sleep
    EUSART1_SetOperationState(UART_CHECK_AUTOBAUD);
    if(SLEEP_OFF != SysGetGoToSleepState())
    {
        SysSetGoToSleepState(SLEEP_ON);
    }
}

static void SysCfgLowPower(void)
{    
    SPI2_DeInitialize();
    
    //Disable MSSP2 module
    MSSP2MD = 1;
    
    //Make sure SPI2 pins are not left in floating state during sleep
    //NCS
    RADIO_nCS_ANS = 0;
    RADIO_nCS_TRIS = 0;
    RADIO_nCS_LAT = 1;

    //MISO
	ANSD1 = 0;
	TRISD1 = 0;
	LATD1 = 0;
		
    //MOSI
	ANSD4 = 0;
	TRISD4 = 0;
	LATD4 = 0;
		
    //CLK
	ANSD0 = 0;
	TRISD0 = 0;
	LATD0 = 0;
    
    //TMR
    SwTimerSetPS(TMR_SLEEP_PS);     // slow the clock during sleep

    // prepare glitch timer for UART wake from sleep
    // T0CON TMR0ON off; T08BIT 8-bit; TOCS CLKOUT; T0SE high-to-low; PSA NOT assigned; TOPS 1:256
    T0CON   = 0x5F;
}

static void SysCfgRun(void)
{    
    //Enable MSSP2 module
    MSSP2MD = 0;    
    SPI2_Initialize();

    // stop glitch timer during normal operation
    // T0CON TMR0ON off; T08BIT 8-bit; TOCS T0CKI; T0SE high-to-low; PSA NOT assigned; TOPS 1:256
    T0CON   = 0x7F;
}

static void UARTStartAutobaudTimer()
{
    uint32_t abdTimeout = LORAWAN_GetABDTimeout();
    if(0 != abdTimeout)
    {
        SwTimerSetTimeout(abdTimerId, MS_TO_TICKS(abdTimeout));
        SwTimerStart(abdTimerId);   // start ABD timeout timer
    }
    
    SysSetGoToSleepState(SLEEP_TRANSITION_TO_OFF);  // prepare to wake
}
