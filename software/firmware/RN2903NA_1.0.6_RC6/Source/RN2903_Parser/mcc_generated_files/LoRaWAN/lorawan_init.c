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
*                           lorawan_init.c
*
* LoRaWAN Initialization file
*
******************************************************************************/

/****************************** INCLUDES **************************************/
#include <xc.h>
#include "../ext_int.h"
#include "../pin_manager.h"
#include "../interrupt_manager.h"
#include "interrupt_manager_lora_addons.h"
#include "tmr_lora_addons.h"
#include "sw_timer.h"
#include "lorawan_defs.h"

void DIO0_ISR_Lora_Init(void);
void DIO1_ISR_Lora_Init(void);
void DI02_ISR_Lora_Init(void);
void DIO4_ISR_Lora_Init(void);
void DIO5_ISR_Lora_Init(void);

void DIO0_ISR_Lora_Init(void)
{
    INT1_SetInterruptHandler(DIO0_ISR_Lora);
}

void DIO1_ISR_Lora_Init(void)
{
    INT2_SetInterruptHandler(DIO1_ISR_Lora);
}

void DIO2_ISR_Lora_Init(void)
{
    IOCB4_SetInterruptHandler(DIO2_ISR_Lora);
}

void DIO3_ISR_Lora_Init(void)
{
}

void DIO4_ISR_Lora_Init(void)
{
    IOCB5_SetInterruptHandler(DIO4_ISR_Lora);
}

void DIO5_ISR_Lora_Init(void)
{
    INT0_SetInterruptHandler(DIO5_ISR_Lora);
}

void LORAWAN_PlatformInit(void)
{
    TMR_ISR_Lora_Init();
    DIO0_ISR_Lora_Init();
    DIO1_ISR_Lora_Init();
    DIO2_ISR_Lora_Init();
    DIO3_ISR_Lora_Init();
    DIO4_ISR_Lora_Init();
    DIO5_ISR_Lora_Init();
    SystemTimerInit();
}
/**
 End of File
*/