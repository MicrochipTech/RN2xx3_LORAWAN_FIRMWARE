/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    tmr_lora_addons.h

  @Summary:
    This is the TMR1 LoRa Addons header file which serves as an extension for TMR1
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
 *                           tmr_lora_addons.h
 *
 * TMR LoRa addons header file
 *
 ******************************************************************************/

#ifndef TMR1_LORA_ADDONS_H
#define	TMR1_LORA_ADDONS_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

extern volatile uint8_t timerOverflow;
extern volatile uint32_t ticksToScheduledInterrupt;
extern volatile uint16_t reloadVal;
extern volatile uint32_t ticksAdded;
    
void TMR_ISR_lora(void);
void TMR_ISR_Lora_Init(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TMR1_LORA_ADDONS_H */

/**
 End of File
*/
