/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    interrupt_manager_lora_addons.h

  @Summary:
    This is the Interrupt Manager LoRa Addons header file which serves as an extension for Interrupt Manager
	module files generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides LoRa-specific defines for TMR1 APIs.
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
 *                           interrupt_manager_lora_addons.h
 *
 * Interrupt Manager LoRa addons header file
 *
 ******************************************************************************/

#ifndef INTERRUPT_MANAGER_LORA_ADDONS_H
#define	INTERRUPT_MANAGER_LORA_ADDONS_H

#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif


#define DIO0        0x01
#define DIO1        0x02
#define DIO2        0x04
#define DIO3        0x08
#define DIO4        0x10
#define DIO5        0x20

volatile uint8_t dioStatus;

/**
 * @Summary
    Returns the status of DIO's.
 * @Description
    This function returns a bitfield representing the status of the DIO's used 
    between the radio and microcontroller.
    This also resets the bitfield value to 0. While this happens interrupts are disabled.
    bit x corresponds to DIOx in the bitfield.
 * @Preconditions
 * @Param
    None
 * @Returns
    DIO status.
 * @Example
*/
uint8_t INTERRUPT_GetDioStatus(void);

/**
 * @Summary
    Returns the status of DIO's.
 * @Description
    This function returns a bitfield representing the status of the DIO's used 
    between the radio and microcontroller.
    bit x corresponds to DIOx in the bitfield.
 * @Preconditions
 * @Param
    None
 * @Returns
    DIO status.
 * @Example
*/
uint8_t INTERRUPT_PeekDioStatus(void);

void DIO0_ISR_Lora(void);
void DIO1_ISR_Lora(void);
void DIO2_ISR_Lora(void);
void DIO3_ISR_Lora(void);
void DIO4_ISR_Lora(void);
void DIO5_ISR_Lora(void);


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPT_MANAGER_LORA_ADDONS_H */

/**
 End of File
*/