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
 *  USB Carrier with RN2483 module
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

#define TICKS_SHIFT_LIMIT   6       // shift limit before a 32-bit overflow

static void OnSleepTimerEvent(uint8_t param);
static void SysCfgLowPower(void);
static void SysCfgRun(void);

static volatile SysSleepState_t goToSleep = SLEEP_OFF;
static volatile SysSleepState_t eusart1WakeUpState = SLEEP_ON;
static uint8_t sleepTimerId;
static exitFromSleepCb_t pExitFromSleepCb;
static volatile bool exitFromSleep = false;



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
    pExitFromSleepCb = exitFromSleepCb;
}

    


void SysSleepStart(uint32_t ms)
{
    parse32bit_t    msParts;
    uint16_t        mask            = 0;
    uint8_t         shiftCount      = 0;
    uint8_t         ticksUpperByte  = 0;
    uint32_t        sleepTicks;
    
    
    /*
     * For short sleep times, times less than 0xfffff ms (~1049 seconds), use
     * the most efficient sleepTicks calculation
     */
    if(MS_TO_TICKS_LONG_THRESHOLD > ms)
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

#if defined(DEBUG_SLEEP)
        sleepTicks = 32;    // TIMER_CORRECTION_TICKS
        if(0 != ms)
        {
#endif // DEBUG_SLEEP

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


#if defined(DEBUG_SLEEP)
        }
#endif // DEBUG_SLEEP

        // set the sleep timer timeout
        SwTimerSetTimeout(sleepTimerId, sleepTicks);
    }
    
    // Tell the system to prepare to sleep
    SysSetGoToSleepState(SLEEP_ON);
    
}

void SysGoToSleep(void)
{
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
            SwTimerStart(sleepTimerId);

            //Radio default state after init is "sleep". There is no need to change radio state
            while (goToSleep == SLEEP_ON || goToSleep == SLEEP_TRANSITION_TO_OFF)
            {
                //if a wake-up is received eusart interrupt will increment the buffer head and eusart1RxCount
                if(eusart1RxCount > 0)
                {
                    EUSART1WakeUp();
                    //EUSART1_Read is needed in order to not consider the received wake-up sequence as part
                    //of the future command. It will decrease the eusart1RxCount back to 0 and will increment
                    //the buffer tail at the same level as buffer head
                    EUSART1_Read();
                    continue;
                }
                BAUDCON1bits.WUE = 1;
                
#if defined(DEBUG_SLEEP)
#include "../mcc_generated_files/pin_manager.h"
                GPIO13_SetLow();    // set low while sleeping
#endif // defined(DEBUG_SLEEP)
                SLEEP();
#if defined(DEBUG_SLEEP)
                GPIO13_SetHigh();   // set high when awake
#endif // defined(DEBUG_SLEEP)
                SwTimersExecute();
            }
            BAUDCON1bits.WUE = 0;
            //executes only on a break command
            if(eusart1WakeUpState == SLEEP_OFF)
            {
                //a blank read is needed because at some point a receive interrupt will occur during the reply
                //from sleep and will consider the \n from the reply as an input and will be introduced in buffer.
                //In this way we ignore the \n introduced during interrupt.
                EUSART1_Read();
                //prepare for next sleep time;
                eusart1WakeUpState = SLEEP_ON;
            }
        }
        INTERRUPT_GlobalInterruptEnable();
    }
}
void EUSART1WakeUp(void)
{
    if(eusart1WakeUpState == SLEEP_ON)
    {
        eusart1WakeUpState = SLEEP_TRANSITION_TO_OFF;
        SysSetGoToSleepState(SLEEP_TRANSITION_TO_OFF);
        BAUDCON1bits.ABDEN = 1;   
    }
    else
    {
        if (RCSTA1bits.FERR == 1)
        {
            if (RCREG1 == 0x00)
            {
                RCSTA1bits.SPEN = 0; /* Force clear FERR by resetting the EUSART */
                NOP();
                RCSTA1bits.SPEN = 1; /* Enable EUSART */
                NOP();
                BAUDCON1bits.ABDEN = 1;
            }
        }
        else
        {
            if(eusart1WakeUpState == SLEEP_TRANSITION_TO_OFF)
            {     
                //Woke up from sleep using UART and new baud detected, reply can be sent now
                eusart1WakeUpState = SLEEP_OFF;
                SysSetGoToSleepState(SLEEP_OFF);
            }
        }
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
}

static void SysCfgRun(void)
{    
    //Enable MSSP2 module
    MSSP2MD = 0;    
    SPI2_Initialize();
}
