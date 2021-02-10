/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager_lora_addons.c

  @Summary:
    This is the Interrupt Manager LoRa Addons source file which serves as an extension
    for Interrupt Manager module files generated using MPLAB(c) Code Configurator

  @Description:
    This source file provides LoRa-specific implementations for SPI APIs.
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
 *                           interrupt_manager_lora_addons.c
 *
 * Interrupt Manager LoRa Addons source file
 *
 ******************************************************************************/

#include <stdint.h>
#include "../interrupt_manager.h"
#include "../mcc.h"
#include "interrupt_manager_lora_addons.h"

volatile uint8_t dioStatus = 0;

uint8_t INTERRUPT_GetDioStatus(void)
{
    uint8_t a;
    INTERRUPT_GlobalInterruptDisable();
    a = dioStatus;
    dioStatus = 0;
    INTERRUPT_GlobalInterruptEnable();
    return a;
}

uint8_t INTERRUPT_PeekDioStatus(void)
{
    return dioStatus;
}

void DIO0_ISR_Lora(void)
{
    dioStatus |= DIO0;
}

void DIO1_ISR_Lora(void)
{
    dioStatus |= DIO1;
}

void DIO2_ISR_Lora(void)
{
    dioStatus |= DIO2;
}

void DIO3_ISR_Lora(void)
{
    dioStatus |= DIO3;
}

void DIO4_ISR_Lora(void)
{
    dioStatus |= DIO4;
}

void DIO5_ISR_Lora(void)
{
    dioStatus |= DIO5;
}

/**
 End of File
*/