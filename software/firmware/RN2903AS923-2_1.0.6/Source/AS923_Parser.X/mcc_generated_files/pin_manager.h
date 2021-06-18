/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC18LF46K22
        Version           :  1.01
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


#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set GPIO0 aliases
#define GPIO0_TRIS               TRISAbits.TRISA0
#define GPIO0_LAT                LATAbits.LATA0
#define GPIO0_PORT               PORTAbits.RA0
#define GPIO0_ANS                ANSELAbits.ANSA0
#define GPIO0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define GPIO0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define GPIO0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define GPIO0_GetValue()           PORTAbits.RA0
#define GPIO0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define GPIO0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define GPIO0_SetAnalogMode()  do { ANSELAbits.ANSA0 = 1; } while(0)
#define GPIO0_SetDigitalMode() do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set GPIO1 aliases
#define GPIO1_TRIS               TRISAbits.TRISA1
#define GPIO1_LAT                LATAbits.LATA1
#define GPIO1_PORT               PORTAbits.RA1
#define GPIO1_ANS                ANSELAbits.ANSA1
#define GPIO1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define GPIO1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define GPIO1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define GPIO1_GetValue()           PORTAbits.RA1
#define GPIO1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define GPIO1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define GPIO1_SetAnalogMode()  do { ANSELAbits.ANSA1 = 1; } while(0)
#define GPIO1_SetDigitalMode() do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set GPIO2 aliases
#define GPIO2_TRIS               TRISAbits.TRISA2
#define GPIO2_LAT                LATAbits.LATA2
#define GPIO2_PORT               PORTAbits.RA2
#define GPIO2_ANS                ANSELAbits.ANSA2
#define GPIO2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define GPIO2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define GPIO2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define GPIO2_GetValue()           PORTAbits.RA2
#define GPIO2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define GPIO2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define GPIO2_SetAnalogMode()  do { ANSELAbits.ANSA2 = 1; } while(0)
#define GPIO2_SetDigitalMode() do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set GPIO3 aliases
#define GPIO3_TRIS               TRISAbits.TRISA3
#define GPIO3_LAT                LATAbits.LATA3
#define GPIO3_PORT               PORTAbits.RA3
#define GPIO3_ANS                ANSELAbits.ANSA3
#define GPIO3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define GPIO3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define GPIO3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define GPIO3_GetValue()           PORTAbits.RA3
#define GPIO3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define GPIO3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define GPIO3_SetAnalogMode()  do { ANSELAbits.ANSA3 = 1; } while(0)
#define GPIO3_SetDigitalMode() do { ANSELAbits.ANSA3 = 0; } while(0)

// get/set GPIO4 aliases
#define GPIO4_TRIS               TRISAbits.TRISA4
#define GPIO4_LAT                LATAbits.LATA4
#define GPIO4_PORT               PORTAbits.RA4
#define GPIO4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define GPIO4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define GPIO4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define GPIO4_GetValue()           PORTAbits.RA4
#define GPIO4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define GPIO4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)

// get/set GPIO5 aliases
#define GPIO5_TRIS               TRISAbits.TRISA5
#define GPIO5_LAT                LATAbits.LATA5
#define GPIO5_PORT               PORTAbits.RA5
#define GPIO5_ANS                ANSELAbits.ANSA5
#define GPIO5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define GPIO5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define GPIO5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define GPIO5_GetValue()           PORTAbits.RA5
#define GPIO5_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define GPIO5_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define GPIO5_SetAnalogMode()  do { ANSELAbits.ANSA5 = 1; } while(0)
#define GPIO5_SetDigitalMode() do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set UART_CTS aliases
#define UART_CTS_TRIS               TRISAbits.TRISA6
#define UART_CTS_LAT                LATAbits.LATA6
#define UART_CTS_PORT               PORTAbits.RA6
#define UART_CTS_SetHigh()            do { LATAbits.LATA6 = 1; } while(0)
#define UART_CTS_SetLow()             do { LATAbits.LATA6 = 0; } while(0)
#define UART_CTS_Toggle()             do { LATAbits.LATA6 = ~LATAbits.LATA6; } while(0)
#define UART_CTS_GetValue()           PORTAbits.RA6
#define UART_CTS_SetDigitalInput()    do { TRISAbits.TRISA6 = 1; } while(0)
#define UART_CTS_SetDigitalOutput()   do { TRISAbits.TRISA6 = 0; } while(0)

// get/set UART_RTS aliases
#define UART_RTS_TRIS               TRISAbits.TRISA7
#define UART_RTS_LAT                LATAbits.LATA7
#define UART_RTS_PORT               PORTAbits.RA7
#define UART_RTS_SetHigh()            do { LATAbits.LATA7 = 1; } while(0)
#define UART_RTS_SetLow()             do { LATAbits.LATA7 = 0; } while(0)
#define UART_RTS_Toggle()             do { LATAbits.LATA7 = ~LATAbits.LATA7; } while(0)
#define UART_RTS_GetValue()           PORTAbits.RA7
#define UART_RTS_SetDigitalInput()    do { TRISAbits.TRISA7 = 1; } while(0)
#define UART_RTS_SetDigitalOutput()   do { TRISAbits.TRISA7 = 0; } while(0)

// get/set RB0 procedures
#define RB0_SetHigh()    do { LATBbits.LATB0 = 1; } while(0)
#define RB0_SetLow()   do { LATBbits.LATB0 = 0; } while(0)
#define RB0_Toggle()   do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define RB0_GetValue()         PORTBbits.RB0
#define RB0_SetDigitalInput()   do { TRISBbits.TRISB0 = 1; } while(0)
#define RB0_SetDigitalOutput()  do { TRISBbits.TRISB0 = 0; } while(0)
#define RB0_SetPullup()     do { WPUBbits.WPUB0 = 1; } while(0)
#define RB0_ResetPullup()   do { WPUBbits.WPUB0 = 0; } while(0)
#define RB0_SetAnalogMode() do { ANSELBbits.ANSB0 = 1; } while(0)
#define RB0_SetDigitalMode()do { ANSELBbits.ANSB0 = 0; } while(0)

// get/set RB1 procedures
#define RB1_SetHigh()    do { LATBbits.LATB1 = 1; } while(0)
#define RB1_SetLow()   do { LATBbits.LATB1 = 0; } while(0)
#define RB1_Toggle()   do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define RB1_GetValue()         PORTBbits.RB1
#define RB1_SetDigitalInput()   do { TRISBbits.TRISB1 = 1; } while(0)
#define RB1_SetDigitalOutput()  do { TRISBbits.TRISB1 = 0; } while(0)
#define RB1_SetPullup()     do { WPUBbits.WPUB1 = 1; } while(0)
#define RB1_ResetPullup()   do { WPUBbits.WPUB1 = 0; } while(0)
#define RB1_SetAnalogMode() do { ANSELBbits.ANSB1 = 1; } while(0)
#define RB1_SetDigitalMode()do { ANSELBbits.ANSB1 = 0; } while(0)

// get/set RB2 procedures
#define RB2_SetHigh()    do { LATBbits.LATB2 = 1; } while(0)
#define RB2_SetLow()   do { LATBbits.LATB2 = 0; } while(0)
#define RB2_Toggle()   do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RB2_GetValue()         PORTBbits.RB2
#define RB2_SetDigitalInput()   do { TRISBbits.TRISB2 = 1; } while(0)
#define RB2_SetDigitalOutput()  do { TRISBbits.TRISB2 = 0; } while(0)
#define RB2_SetPullup()     do { WPUBbits.WPUB2 = 1; } while(0)
#define RB2_ResetPullup()   do { WPUBbits.WPUB2 = 0; } while(0)
#define RB2_SetAnalogMode() do { ANSELBbits.ANSB2 = 1; } while(0)
#define RB2_SetDigitalMode()do { ANSELBbits.ANSB2 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()    do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()   do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()   do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()         PORTBbits.RB4
#define RB4_SetDigitalInput()   do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()  do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()     do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()   do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode() do { ANSELBbits.ANSB4 = 1; } while(0)
#define RB4_SetDigitalMode()do { ANSELBbits.ANSB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()    do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()   do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()   do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()         PORTBbits.RB5
#define RB5_SetDigitalInput()   do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()  do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()     do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()   do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode() do { ANSELBbits.ANSB5 = 1; } while(0)
#define RB5_SetDigitalMode()do { ANSELBbits.ANSB5 = 0; } while(0)

// get/set TEST0 aliases
#define TEST0_TRIS               TRISBbits.TRISB6
#define TEST0_LAT                LATBbits.LATB6
#define TEST0_PORT               PORTBbits.RB6
#define TEST0_WPU                WPUBbits.WPUB6
#define TEST0_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define TEST0_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define TEST0_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define TEST0_GetValue()           PORTBbits.RB6
#define TEST0_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define TEST0_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define TEST0_SetPullup()      do { WPUBbits.WPUB6 = 1; } while(0)
#define TEST0_ResetPullup()    do { WPUBbits.WPUB6 = 0; } while(0)

// get/set TEST1 aliases
#define TEST1_TRIS               TRISBbits.TRISB7
#define TEST1_LAT                LATBbits.LATB7
#define TEST1_PORT               PORTBbits.RB7
#define TEST1_WPU                WPUBbits.WPUB7
#define TEST1_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define TEST1_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define TEST1_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define TEST1_GetValue()           PORTBbits.RB7
#define TEST1_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define TEST1_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define TEST1_SetPullup()      do { WPUBbits.WPUB7 = 1; } while(0)
#define TEST1_ResetPullup()    do { WPUBbits.WPUB7 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()    do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()   do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()   do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()         PORTCbits.RC0
#define RC0_SetDigitalInput()   do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()  do { TRISCbits.TRISC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()    do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()   do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()   do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()         PORTCbits.RC1
#define RC1_SetDigitalInput()   do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()  do { TRISCbits.TRISC1 = 0; } while(0)

// get/set RC2 procedures
#define RC2_SetHigh()    do { LATCbits.LATC2 = 1; } while(0)
#define RC2_SetLow()   do { LATCbits.LATC2 = 0; } while(0)
#define RC2_Toggle()   do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define RC2_GetValue()         PORTCbits.RC2
#define RC2_SetDigitalInput()   do { TRISCbits.TRISC2 = 1; } while(0)
#define RC2_SetDigitalOutput()  do { TRISCbits.TRISC2 = 0; } while(0)
#define RC2_SetAnalogMode() do { ANSELCbits.ANSC2 = 1; } while(0)
#define RC2_SetDigitalMode()do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()    do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()   do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()   do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()         PORTCbits.RC3
#define RC3_SetDigitalInput()   do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()  do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetAnalogMode() do { ANSELCbits.ANSC3 = 1; } while(0)
#define RC3_SetDigitalMode()do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()    do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()   do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()   do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()         PORTCbits.RC4
#define RC4_SetDigitalInput()   do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()  do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetAnalogMode() do { ANSELCbits.ANSC4 = 1; } while(0)
#define RC4_SetDigitalMode()do { ANSELCbits.ANSC4 = 0; } while(0)

// get/set GPIO10 aliases
#define GPIO10_TRIS               TRISCbits.TRISC5
#define GPIO10_LAT                LATCbits.LATC5
#define GPIO10_PORT               PORTCbits.RC5
#define GPIO10_ANS                ANSELCbits.ANSC5
#define GPIO10_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define GPIO10_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define GPIO10_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define GPIO10_GetValue()           PORTCbits.RC5
#define GPIO10_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define GPIO10_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define GPIO10_SetAnalogMode()  do { ANSELCbits.ANSC5 = 1; } while(0)
#define GPIO10_SetDigitalMode() do { ANSELCbits.ANSC5 = 0; } while(0)

// get/set RC6 procedures
#define RC6_SetHigh()    do { LATCbits.LATC6 = 1; } while(0)
#define RC6_SetLow()   do { LATCbits.LATC6 = 0; } while(0)
#define RC6_Toggle()   do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define RC6_GetValue()         PORTCbits.RC6
#define RC6_SetDigitalInput()   do { TRISCbits.TRISC6 = 1; } while(0)
#define RC6_SetDigitalOutput()  do { TRISCbits.TRISC6 = 0; } while(0)
#define RC6_SetAnalogMode() do { ANSELCbits.ANSC6 = 1; } while(0)
#define RC6_SetDigitalMode()do { ANSELCbits.ANSC6 = 0; } while(0)

// get/set RC7 procedures
#define RC7_SetHigh()    do { LATCbits.LATC7 = 1; } while(0)
#define RC7_SetLow()   do { LATCbits.LATC7 = 0; } while(0)
#define RC7_Toggle()   do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define RC7_GetValue()         PORTCbits.RC7
#define RC7_SetDigitalInput()   do { TRISCbits.TRISC7 = 1; } while(0)
#define RC7_SetDigitalOutput()  do { TRISCbits.TRISC7 = 0; } while(0)
#define RC7_SetAnalogMode() do { ANSELCbits.ANSC7 = 1; } while(0)
#define RC7_SetDigitalMode()do { ANSELCbits.ANSC7 = 0; } while(0)

// get/set RD0 procedures
#define RD0_SetHigh()    do { LATDbits.LATD0 = 1; } while(0)
#define RD0_SetLow()   do { LATDbits.LATD0 = 0; } while(0)
#define RD0_Toggle()   do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define RD0_GetValue()         PORTDbits.RD0
#define RD0_SetDigitalInput()   do { TRISDbits.TRISD0 = 1; } while(0)
#define RD0_SetDigitalOutput()  do { TRISDbits.TRISD0 = 0; } while(0)
#define RD0_SetAnalogMode() do { ANSELDbits.ANSD0 = 1; } while(0)
#define RD0_SetDigitalMode()do { ANSELDbits.ANSD0 = 0; } while(0)

// get/set RD1 procedures
#define RD1_SetHigh()    do { LATDbits.LATD1 = 1; } while(0)
#define RD1_SetLow()   do { LATDbits.LATD1 = 0; } while(0)
#define RD1_Toggle()   do { LATDbits.LATD1 = ~LATDbits.LATD1; } while(0)
#define RD1_GetValue()         PORTDbits.RD1
#define RD1_SetDigitalInput()   do { TRISDbits.TRISD1 = 1; } while(0)
#define RD1_SetDigitalOutput()  do { TRISDbits.TRISD1 = 0; } while(0)
#define RD1_SetAnalogMode() do { ANSELDbits.ANSD1 = 1; } while(0)
#define RD1_SetDigitalMode()do { ANSELDbits.ANSD1 = 0; } while(0)

// get/set GPIO8 aliases
#define GPIO8_TRIS               TRISDbits.TRISD2
#define GPIO8_LAT                LATDbits.LATD2
#define GPIO8_PORT               PORTDbits.RD2
#define GPIO8_ANS                ANSELDbits.ANSD2
#define GPIO8_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define GPIO8_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define GPIO8_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define GPIO8_GetValue()           PORTDbits.RD2
#define GPIO8_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define GPIO8_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define GPIO8_SetAnalogMode()  do { ANSELDbits.ANSD2 = 1; } while(0)
#define GPIO8_SetDigitalMode() do { ANSELDbits.ANSD2 = 0; } while(0)

// get/set RD3 procedures
#define RD3_SetHigh()    do { LATDbits.LATD3 = 1; } while(0)
#define RD3_SetLow()   do { LATDbits.LATD3 = 0; } while(0)
#define RD3_Toggle()   do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define RD3_GetValue()         PORTDbits.RD3
#define RD3_SetDigitalInput()   do { TRISDbits.TRISD3 = 1; } while(0)
#define RD3_SetDigitalOutput()  do { TRISDbits.TRISD3 = 0; } while(0)
#define RD3_SetAnalogMode() do { ANSELDbits.ANSD3 = 1; } while(0)
#define RD3_SetDigitalMode()do { ANSELDbits.ANSD3 = 0; } while(0)

// get/set RD4 procedures
#define RD4_SetHigh()    do { LATDbits.LATD4 = 1; } while(0)
#define RD4_SetLow()   do { LATDbits.LATD4 = 0; } while(0)
#define RD4_Toggle()   do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define RD4_GetValue()         PORTDbits.RD4
#define RD4_SetDigitalInput()   do { TRISDbits.TRISD4 = 1; } while(0)
#define RD4_SetDigitalOutput()  do { TRISDbits.TRISD4 = 0; } while(0)
#define RD4_SetAnalogMode() do { ANSELDbits.ANSD4 = 1; } while(0)
#define RD4_SetDigitalMode()do { ANSELDbits.ANSD4 = 0; } while(0)

// get/set GPIO11 aliases
#define GPIO11_TRIS               TRISDbits.TRISD5
#define GPIO11_LAT                LATDbits.LATD5
#define GPIO11_PORT               PORTDbits.RD5
#define GPIO11_ANS                ANSELDbits.ANSD5
#define GPIO11_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define GPIO11_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define GPIO11_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define GPIO11_GetValue()           PORTDbits.RD5
#define GPIO11_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define GPIO11_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define GPIO11_SetAnalogMode()  do { ANSELDbits.ANSD5 = 1; } while(0)
#define GPIO11_SetDigitalMode() do { ANSELDbits.ANSD5 = 0; } while(0)

// get/set GPIO13 aliases
#define GPIO13_TRIS               TRISDbits.TRISD6
#define GPIO13_LAT                LATDbits.LATD6
#define GPIO13_PORT               PORTDbits.RD6
#define GPIO13_ANS                ANSELDbits.ANSD6
#define GPIO13_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define GPIO13_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define GPIO13_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define GPIO13_GetValue()           PORTDbits.RD6
#define GPIO13_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define GPIO13_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define GPIO13_SetAnalogMode()  do { ANSELDbits.ANSD6 = 1; } while(0)
#define GPIO13_SetDigitalMode() do { ANSELDbits.ANSD6 = 0; } while(0)

// get/set GPIO12 aliases
#define GPIO12_TRIS               TRISDbits.TRISD7
#define GPIO12_LAT                LATDbits.LATD7
#define GPIO12_PORT               PORTDbits.RD7
#define GPIO12_ANS                ANSELDbits.ANSD7
#define GPIO12_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define GPIO12_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define GPIO12_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define GPIO12_GetValue()           PORTDbits.RD7
#define GPIO12_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define GPIO12_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define GPIO12_SetAnalogMode()  do { ANSELDbits.ANSD7 = 1; } while(0)
#define GPIO12_SetDigitalMode() do { ANSELDbits.ANSD7 = 0; } while(0)

// get/set GPIO6 aliases
#define GPIO6_TRIS               TRISEbits.TRISE0
#define GPIO6_LAT                LATEbits.LATE0
#define GPIO6_PORT               PORTEbits.RE0
#define GPIO6_ANS                ANSELEbits.ANSE0
#define GPIO6_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define GPIO6_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define GPIO6_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define GPIO6_GetValue()           PORTEbits.RE0
#define GPIO6_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define GPIO6_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define GPIO6_SetAnalogMode()  do { ANSELEbits.ANSE0 = 1; } while(0)
#define GPIO6_SetDigitalMode() do { ANSELEbits.ANSE0 = 0; } while(0)

// get/set GPIO7 aliases
#define GPIO7_TRIS               TRISEbits.TRISE1
#define GPIO7_LAT                LATEbits.LATE1
#define GPIO7_PORT               PORTEbits.RE1
#define GPIO7_ANS                ANSELEbits.ANSE1
#define GPIO7_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define GPIO7_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define GPIO7_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define GPIO7_GetValue()           PORTEbits.RE1
#define GPIO7_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define GPIO7_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define GPIO7_SetAnalogMode()  do { ANSELEbits.ANSE1 = 1; } while(0)
#define GPIO7_SetDigitalMode() do { ANSELEbits.ANSE1 = 0; } while(0)

// get/set GPIO9 aliases
#define GPIO9_TRIS               TRISEbits.TRISE2
#define GPIO9_LAT                LATEbits.LATE2
#define GPIO9_PORT               PORTEbits.RE2
#define GPIO9_ANS                ANSELEbits.ANSE2
#define GPIO9_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define GPIO9_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define GPIO9_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define GPIO9_GetValue()           PORTEbits.RE2
#define GPIO9_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define GPIO9_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define GPIO9_SetAnalogMode()  do { ANSELEbits.ANSE2 = 1; } while(0)
#define GPIO9_SetDigitalMode() do { ANSELEbits.ANSE2 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCB4 pin functionality
 * @Example
    IOCB4_ISR();
 */
void IOCB4_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCB4 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCB4 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCB4_SetInterruptHandler(MyInterruptHandler);

*/
void IOCB4_SetInterruptHandler(void* InterruptHandler);

/**
  @Summary
    Dynamic Interrupt Handler for IOCB4 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCB4_SetInterruptHandler() method.
    This handler is called every time the IOCB4 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB4_SetInterruptHandler(IOCB4_InterruptHandler);

*/
extern void (*IOCB4_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCB4 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCB4_SetInterruptHandler() method.
    This handler is called every time the IOCB4 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB4_SetInterruptHandler(IOCB4_DefaultInterruptHandler);

*/
void IOCB4_DefaultInterruptHandler(void);


/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCB5 pin functionality
 * @Example
    IOCB5_ISR();
 */
void IOCB5_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCB5 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCB5 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCB5_SetInterruptHandler(MyInterruptHandler);

*/
void IOCB5_SetInterruptHandler(void* InterruptHandler);

/**
  @Summary
    Dynamic Interrupt Handler for IOCB5 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCB5_SetInterruptHandler() method.
    This handler is called every time the IOCB5 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB5_SetInterruptHandler(IOCB5_InterruptHandler);

*/
extern void (*IOCB5_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCB5 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCB5_SetInterruptHandler() method.
    This handler is called every time the IOCB5 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCB5_SetInterruptHandler(IOCB5_DefaultInterruptHandler);

*/
void IOCB5_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/