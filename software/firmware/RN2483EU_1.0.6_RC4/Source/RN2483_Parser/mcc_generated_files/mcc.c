/**
  @Generated MPLAB(c) Code Configurator Source File

  @Company:
    Microchip Technology Inc.

  @File Name:
    mcc.c

  @Summary:
    This is the mcc.c file generated using MPLAB(c) Code Configurator

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC18LF46K22
        Driver Version    :  1.02
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

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
*/

// Configuration bits: selected in the GUI

// CONFIG1H
#pragma config FOSC = INTIO67    // Oscillator Selection bits->Internal oscillator block
#pragma config PLLCFG = OFF    // 4X PLL Enable->Oscillator used directly
#pragma config PRICLKEN = OFF    // Primary clock enable bit->Primary clock can be disabled by software
#pragma config FCMEN = OFF    // Fail-Safe Clock Monitor Enable bit->Fail-Safe Clock Monitor disabled
#pragma config IESO = OFF    // Internal/External Oscillator Switchover bit->Oscillator Switchover mode disabled

// CONFIG2L
#pragma config PWRTEN = OFF    // Power-up Timer Enable bit->Power up timer disabled
#pragma config BOREN = NOSLP    // Brown-out Reset Enable bits->Brown-out Reset enabled in hardware only and disabled in Sleep mode (SBOREN is disabled)
#pragma config BORV = 190    // Brown Out Reset Voltage bits->VBOR set to 1.90 V nominal

// CONFIG2H
#pragma config WDTEN = OFF    // Watchdog Timer Enable bits->Watch dog timer is always disabled. SWDTEN has no effect.
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits->1:32768

// CONFIG3H
#pragma config CCP2MX = PORTC1    // CCP2 MUX bit->CCP2 input/output is multiplexed with RC1
#pragma config PBADEN = ON    // PORTB A/D Enable bit->PORTB<5:0> pins are configured as analog input channels on Reset
#pragma config CCP3MX = PORTB5    // P3A/CCP3 Mux bit->P3A/CCP3 input/output is multiplexed with RB5
#pragma config HFOFST = ON    // HFINTOSC Fast Start-up->HFINTOSC output and ready status are not delayed by the oscillator stable status
#pragma config T3CMX = PORTC0    // Timer3 Clock input mux bit->T3CKI is on RC0
#pragma config P2BMX = PORTD2    // ECCP2 B output mux bit->P2B is on RD2
#pragma config MCLRE = EXTMCLR    // MCLR Pin Enable bit->MCLR pin enabled, RE3 input pin disabled

// CONFIG4L
#pragma config STVREN = ON    // Stack Full/Underflow Reset Enable bit->Stack full/underflow will cause Reset
#pragma config LVP = ON    // Single-Supply ICSP Enable bit->Single-Supply ICSP enabled if MCLRE is also 1
#pragma config XINST = OFF    // Extended Instruction Set Enable bit->Instruction set extension and Indexed Addressing mode disabled (Legacy mode)
#pragma config DEBUG = OFF    // Background Debug->Disabled

// CONFIG5L
#pragma config CP0 = ON    // Code Protection Block 0->Block 0 (000800-003FFFh) code-protected
#pragma config CP1 = ON    // Code Protection Block 1->Block 1 (004000-007FFFh) code-protected
#pragma config CP2 = ON    // Code Protection Block 2->Block 2 (008000-00BFFFh) code-protected
#pragma config CP3 = ON    // Code Protection Block 3->Block 3 (00C000-00FFFFh) code-protected

// CONFIG5H
#pragma config CPB = ON    // Boot Block Code Protection bit->Boot block (000000-0007FFh) code-protected
#pragma config CPD = ON    // Data EEPROM Code Protection bit->Data EEPROM code-protected

// CONFIG6L
#pragma config WRT0 = OFF    // Write Protection Block 0->Block 0 (000800-003FFFh) not write-protected
#pragma config WRT1 = OFF    // Write Protection Block 1->Block 1 (004000-007FFFh) not write-protected
#pragma config WRT2 = OFF    // Write Protection Block 2->Block 2 (008000-00BFFFh) not write-protected
#pragma config WRT3 = OFF    // Write Protection Block 3->Block 3 (00C000-00FFFFh) not write-protected

// CONFIG6H
#pragma config WRTC = OFF    // Configuration Register Write Protection bit->Configuration registers (300000-3000FFh) not write-protected
#pragma config WRTB = OFF    // Boot Block Write Protection bit->Boot Block (000000-0007FFh) not write-protected
#pragma config WRTD = OFF    // Data EEPROM Write Protection bit->Data EEPROM not write-protected

// CONFIG7L
#pragma config EBTR0 = OFF    // Table Read Protection Block 0->Block 0 (000800-003FFFh) not protected from table reads executed in other blocks
#pragma config EBTR1 = OFF    // Table Read Protection Block 1->Block 1 (004000-007FFFh) not protected from table reads executed in other blocks
#pragma config EBTR2 = OFF    // Table Read Protection Block 2->Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks
#pragma config EBTR3 = OFF    // Table Read Protection Block 3->Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks

// CONFIG7H
#pragma config EBTRB = OFF    // Boot Block Table Read Protection bit->Boot Block (000000-0007FFh) not protected from table reads executed in other blocks

#include "mcc.h"

void SYSTEM_Initialize(void)
{
    
    INTERRUPT_Initialize();
    PIN_MANAGER_Initialize();
    OSCILLATOR_Initialize();
    I2C1_Initialize();
    SPI2_Initialize();
    FVR_Initialize();
    ADC_Initialize();
    EXT_INT_Initialize();
    TMR1_Initialize();
    EUSART1_Initialize();
    LORAWAN_PlatformInit();
}

void OSCILLATOR_Initialize(void)
{
    // SCS FOSC; IRCF 16MHz_HFINTOSC/4; IDLEN disabled; 
    OSCCON = 0x70;
    // PRISD enabled; SOSCGO disabled; MFIOSEL disabled; 
    OSCCON2 = 0x04;
    // INTSRC disabled; PLLEN disabled; TUN 0; 
    OSCTUNE = 0x00;
    // Set the secondary oscillator
    
}


/**
 End of File
*/
