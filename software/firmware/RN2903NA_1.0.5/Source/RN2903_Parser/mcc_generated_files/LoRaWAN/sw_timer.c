/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    sw_timer.c

  @Summary:
    This is the SW Timer source file which provides SW Timer service to the LoRa

  @Description:
    This source file provides SW Timer service implementation.
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
 *                           sw_timer.c
 *
 * SW Timer source file
 *
 ******************************************************************************/

#include <stddef.h>
#include <xc.h>
#include "sw_timer.h"
#include "tmr_lora_addons.h"
#include "mcc_lora_config.h"
#include "../interrupt_manager.h"
#include "../mcc.h"

#define TIMER_CORRECTION_TICKS          32

typedef struct
{
    uint32_t ticksRemaining;
    uint8_t running;
    uint8_t callbackParameter;
    void (*callback)(uint8_t);
} SwTimer_t;

volatile static SwTimer_t swTimers[MAX_TIMERS];
volatile static uint8_t allocatedTimers;
volatile static uint32_t ticksAccounted;


// This function needs to be called with interrupts stopped, shortly after
// GetDeltaTime (i.e. when ticksPassed ~= 0)
void TMR_OverrideRemaining(uint32_t ticksRemaining)
{
    uint16_t tmrVal;
    uint16_t oldReloadVal;

    // Make sure we don't mess with the timer too soon before it expires
    if (ticksToScheduledInterrupt > 10)
    {
        if (ticksRemaining < ticksToScheduledInterrupt)
        {
            ticksToScheduledInterrupt = ticksRemaining;
            oldReloadVal = reloadVal;
            // Refresh timer value
            reloadVal = HW_MAX_TIMER_VAL - ticksRemaining;
            tmrVal = TMR_SwapTimer(reloadVal);
            ticksAdded += tmrVal - oldReloadVal;

            //PIR1bits.TMR1IF = 0;
        }
    }
}

// This function needs to be called with interrupts stopped
uint32_t TMR_GetDeltaTime(void)
{
    static uint32_t retVal;
    static uint16_t tmrVal;

    tmrVal = TMR_ReadTimer();

    if (timerOverflow == 1)
    {
        timerOverflow = 0;
        retVal = HW_MAX_TIMER_VAL - reloadVal + tmrVal - ticksAccounted + ticksAdded;
        ticksAccounted = tmrVal;
        ticksAdded = 0;
        reloadVal = 0;
    }
    else
    {
        retVal = tmrVal - reloadVal - ticksAccounted + ticksAdded;
        ticksAccounted += retVal;
    }

    return retVal;
}

uint16_t TMR_SwapTimer(uint16_t timerVal)
{
    static uint16_t oldVal;
    static uint16_t retVal;

    oldVal = TMR1_ReadTimer();

    // Block until timer increments. This way during the following ~120
    // instructions we won't run the risk of having the timer change again and
    // we can safely update its value with something new.
    while (oldVal == TMR1_ReadTimer())
		;

    TMR1_WriteTimer(timerVal);

    // Correct output since we've let the timer increment after reading it
    oldVal++;

    retVal = oldVal;

    return retVal;
}

void SystemTimerInit(void)
{
    allocatedTimers = 0;
}

// This function should allow interrupts to happen (unless it was called from
// an interrupt itself) and keep its timing accurate. Ideally it should do the
// waiting with the MCU in sleep.
// Find out how long it takes the MCU to go to and wake up from sleep to see if
// it makes sense to go to sleep at all.
void SystemBlockingWaitMs(uint32_t ms)
{
    while (ms > 0)
    {
        __delay_ms(1);
        ms--;
    }
}

uint8_t SwTimerCreate(void)
{
    uint8_t retVal;
    if (allocatedTimers < MAX_TIMERS)
    {
        INTERRUPT_GlobalInterruptDisable();
        retVal = allocatedTimers;
        swTimers[retVal].running = 0;
        swTimers[retVal].ticksRemaining = 0;
        swTimers[retVal].callbackParameter = 0;
        swTimers[retVal].callback = NULL;
        allocatedTimers++;
        INTERRUPT_GlobalInterruptEnable();
    }
    else
    {
        while(1)
        {
            // If you reach this spot it means the MAX_TIMERS is #defined to a
            // lower value than the number of timers that have been
            // SwTimerCreate()d
        }
    }
    return retVal;
}

void SwTimerSetCallback(uint8_t timerId, void (*callback)(uint8_t), uint8_t callbackParameter)
{
    swTimers[timerId].callback = callback;
    swTimers[timerId].callbackParameter = callbackParameter;
}

void SwTimerSetTimeout(uint8_t timerId, uint32_t timeout)
{
    INTERRUPT_GlobalInterruptDisable();
    swTimers[timerId].ticksRemaining = timeout - TIMER_CORRECTION_TICKS;
    INTERRUPT_GlobalInterruptEnable();
}

uint32_t SwTimerReadValue(uint8_t timerId)
{
    uint32_t ticksCount;

    INTERRUPT_GlobalInterruptDisable();
    // Calling this function synchronizes all timers to the current moment, i.e.
    // updates the elapsed times on all timers
    SwTimersInterrupt();
    ticksCount = swTimers[timerId].ticksRemaining;
    INTERRUPT_GlobalInterruptEnable();

    return ticksCount;
}

uint8_t SwTimerIsRunning(uint8_t timerId)
{
    uint8_t isRunning;

    INTERRUPT_GlobalInterruptDisable();
    isRunning = swTimers[timerId].running;
    INTERRUPT_GlobalInterruptEnable();

    return isRunning;
}

void SwTimerStart(uint8_t timerId)
{
    uint32_t ticksRemaining;
    // Need to synchronize all timers
    INTERRUPT_GlobalInterruptDisable();
    ticksRemaining = SwTimersInterrupt();
    if (swTimers[timerId].ticksRemaining < ticksRemaining)
    {
        ticksRemaining = swTimers[timerId].ticksRemaining;
    }
    TMR_OverrideRemaining(ticksRemaining);
    swTimers[timerId].running = 1;
    INTERRUPT_GlobalInterruptEnable();
}

void SwTimerStop(uint8_t timerId)
{
    INTERRUPT_GlobalInterruptDisable();
    swTimers[timerId].running = 0;
    INTERRUPT_GlobalInterruptEnable();
}

// This function needs to be called with interrupts disabled. Returns 1 if MCU
// may go to sleep and 0 if it may not.
uint8_t SwTimersCanSleep(void)
{
    uint8_t i;

    // Calling this function synchronizes all timers to the current moment, i.e.
    // updates the elapsed times on all timers
    SwTimersInterrupt();

    for (i = 0; i < allocatedTimers; i++)
    {
        if ((swTimers[i].running == 1) && (0 == swTimers[i].ticksRemaining))
        {
            return 0;
        }
    }
    return 1;
}

void SwTimersExecute(void)
{
    uint8_t i;
    
    INTERRUPT_GlobalInterruptDisable();
    for (i = 0; i < allocatedTimers; i++)
    {
        // Subtract time elapsed from all running timers and if any hits 0
        // execute its callback (if not null)
        if (swTimers[i].running == 1)
        {
            if (0 == swTimers[i].ticksRemaining)
            {
                swTimers[i].running = 0;
                if (NULL != swTimers[i].callback)
                {
                    INTERRUPT_GlobalInterruptEnable();
                    swTimers[i].callback(swTimers[i].callbackParameter);
                    INTERRUPT_GlobalInterruptDisable();
                }
            }
        }
    }
    INTERRUPT_GlobalInterruptEnable();
}

// Function called from ISR AND mainline code
uint32_t SwTimersInterrupt(void)
{
    uint8_t i;
    uint32_t timeElapsed;
    uint32_t ticksToNextTimerEvent = 0xFFFFFFFF;

    // Find out how much time has passed since the last execution of this
    // function.
    timeElapsed = TMR_GetDeltaTime();

    for (i = 0; i < allocatedTimers; i++)
    {
        // Subtract time elapsed from all running timers and if any hits 0
        // register its callback for execution (msRemaining = 0)
        if (swTimers[i].running == 1)
        {
            if (swTimers[i].ticksRemaining <= timeElapsed)
            {
                swTimers[i].ticksRemaining = 0;
            }
            else
            {
                swTimers[i].ticksRemaining -= timeElapsed;
                // Tell TMR1 subsystem to schedule the next interrupt at the
                // minimum timeout value of all timers
                if (swTimers[i].ticksRemaining < ticksToNextTimerEvent)
                {
                    ticksToNextTimerEvent = swTimers[i].ticksRemaining;
                }
            }
        }
    }

    return ticksToNextTimerEvent;
}

/**
 End of File
*/