/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    tmr_lora_addons.c

  @Summary:
    This is the TMR1 LoRa Addons source file which serves as an extension for TMR1
	module files generated using MPLAB(c) Code Configurator

  @Description:
    This source file provides LoRa-specific implementations for TMR1 APIs.
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
 *                           tmr_lora_addons.c
 *
 * TMR LoRa Addons source file
 *
 ******************************************************************************/

#include <xc.h>
#include <stddef.h>
#include "../tmr1.h"
#include "../interrupt_manager.h"
#include "tmr_lora_addons.h"
#include "mcc_lora_config.h"
#include "sw_timer.h"

volatile uint8_t timerOverflow = 0;
volatile uint32_t ticksToScheduledInterrupt = 0xFFFFFFFFU;
volatile uint16_t reloadVal = 0;
volatile uint32_t ticksAdded = 0;

void TMR_ISR_Lora_Init(void)
{
    TMR_SetInterruptHandler(TMR_ISR_lora);
}

void TMR_ISR_lora(void)
{
    timerOverflow = 1;
    // This could be the registered interrupt. Call software timers
    ticksToScheduledInterrupt = SwTimersInterrupt();
    
    if (ticksToScheduledInterrupt < HW_MAX_TIMER_VAL)
    {
        reloadVal = HW_MAX_TIMER_VAL - ticksToScheduledInterrupt;
        ticksAdded = TMR_SwapTimer(reloadVal);
    }
}

/**
 End of File
*/