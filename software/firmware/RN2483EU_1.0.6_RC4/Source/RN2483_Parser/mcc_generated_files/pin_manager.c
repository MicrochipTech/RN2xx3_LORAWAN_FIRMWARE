/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC18LF46K22
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.

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

*/

#include <xc.h>
#include "pin_manager.h"
#include "stdbool.h"

void (*IOCB4_InterruptHandler)(void);
void (*IOCB5_InterruptHandler)(void);

void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */   
    LATE = 0x00;    
    LATD = 0x08;    
    LATA = 0x00;    
    LATB = 0x00;    
    LATC = 0x18;    

    /**
    TRISx registers
    */    
    TRISE = 0x00;
    TRISA = 0x00;
    TRISB = 0x3F;
    TRISC = 0xDF;
    TRISD = 0x02;

    /**
    ANSELx registers
    */   
    ANSELC = 0x04;
    ANSELB = 0x00;
    ANSELD = 0x00;
    ANSELE = 0x00;
    ANSELA = 0x00;

    /**
    WPUx registers
    */ 
    WPUB = 0x00;
    INTCON2bits.nRBPU = 1;

    

    /**
    IOCx registers
    */
    // interrupt on change for group IOCB - any
    IOCBbits.IOCB4 = 1;
    IOCBbits.IOCB5 = 1;

    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCB4_SetInterruptHandler(IOCB4_DefaultInterruptHandler);
    IOCB5_SetInterruptHandler(IOCB5_DefaultInterruptHandler);
   
    // Enable RBI interrupt 
    INTCONbits.RBIE = 1; 
    
}       

void PIN_MANAGER_IOC(void)
{   
    // interrupt on change for pin IOCB4
    if(IOCBbits.IOCB4 == 1)
    {
        IOCB4_ISR();  
    } 
    // interrupt on change for pin IOCB5
    if(IOCBbits.IOCB5 == 1)
    {
        IOCB5_ISR();  
    } 
    
    // Clear global Interrupt-On-Change flag
    INTCONbits.RBIF = 0;

}

/**
   IOCB4 Interrupt Service Routine
*/
void IOCB4_ISR(void) {

    // Add custom IOCB4 code

    // Call the interrupt handler for the callback registered at runtime
    if(IOCB4_InterruptHandler)
    {
        IOCB4_InterruptHandler();
    }
}

/**
  Allows selecting an interrupt handler for IOCB4 at application runtime
*/
void IOCB4_SetInterruptHandler(void* InterruptHandler){
    IOCB4_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCB4
*/
void IOCB4_DefaultInterruptHandler(void){
    // add your IOCB4 interrupt custom code
    // or set custom function using IOCB4_SetInterruptHandler()
}

/**
   IOCB5 Interrupt Service Routine
*/
void IOCB5_ISR(void) {

    // Add custom IOCB5 code

    // Call the interrupt handler for the callback registered at runtime
    if(IOCB5_InterruptHandler)
    {
        IOCB5_InterruptHandler();
    }
}

/**
  Allows selecting an interrupt handler for IOCB5 at application runtime
*/
void IOCB5_SetInterruptHandler(void* InterruptHandler){
    IOCB5_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCB5
*/
void IOCB5_DefaultInterruptHandler(void){
    // add your IOCB5 interrupt custom code
    // or set custom function using IOCB5_SetInterruptHandler()
}

/**
 End of File
*/