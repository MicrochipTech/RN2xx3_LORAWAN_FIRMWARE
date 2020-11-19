/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    radio_driver_hal.h

  @Summary:
    This is the Radio Driver HAL header file which contains LoRa-specific Radio Driver
    Hardware Abstract Layer

  @Description:
    This header file provides LoRa-specific implementations for Radio Driver HAL APIs.
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
 *                           radio_driver_hal.h
 *
 * Radio Driver HAL header file
 *
 ******************************************************************************/

#ifndef RADIO_DRIVER_HAL_H
#define	RADIO_DRIVER_HAL_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdint.h>
#include "../spi2.h"
#include "../pin_manager.h"
#include "pin_manager_lora_addons.h"

    
void HALResetPinMakeOutput(void);
void HALResetPinMakeInput(void);
void HALResetPinOutputValue(uint8_t value);

void HALSPICSAssert(void);
void HALSPICSDeassert(void);
uint8_t HALSPISend(uint8_t data);
uint8_t HALDIO0PinValue(void);
uint8_t HALDIO1PinValue(void);
uint8_t HALDIO2PinValue(void);
uint8_t HALDIO4PinValue(void);
uint8_t HALDIO5PinValue(void);
void HALEnableRFSwitch(uint8_t ifc);
void HALDisableRFSwitch(uint8_t ifc);
#ifdef	__cplusplus
}
#endif

#endif	/* RADIO_DRIVER_HAL_H */

/**
 End of File
*/