;/*****************************************************************************
;* CODE OWNERSHIP AND DISCLAIMER OF LIABILITY
;*
;* Microchip Technology Incorporated ("Microchip") retains all ownership and
;* intellectual property rights in the code accompanying this message and in
;* all derivatives hereto.  You may use this code, and any derivatives created
;* by any person or entity by or on your behalf, exclusively with Microchip?s
;* proprietary products.  Your acceptance and/or use of this code constitutes
;* agreement to the terms and conditions of this notice.
;*
;* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
;* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT
;* LIMITED TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND
;* FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH
;* MICROCHIP?S PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY
;* APPLICATION.
;*
;* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
;* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
;* STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
;* FOR ANY INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
;* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
;* CODE, HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
;* POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE
;* BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
;* CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY
;* TO HAVE THIS CODE DEVELOPED.
;*
;* You agree that you are solely responsible for testing the code and
;* determining its suitability.  Microchip has no obligation to modify, test,
;* certify, or support the code.
;*
;* Author                Date        Comment
;*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
;* name                  09/11/08    ...
;******************************************************************************//*
; * File:   Hardware defs.h
; * Author: C08156
; *
; * Created on September 14, 2012, 4:25 PM
; */

#ifndef HARDWARE_DEFS_H
#define	HARDWARE_DEFS_H

    
#define  INPUT_PIN   1
#define  OUTPUT_PIN  0

#define  ANALOG_PIN  1
#define  DIGITAL_PIN 0

#define  OPEN_DRAIN_ENABLED     1  // Enable Open Drain operation
#define  OPEN_DRAIN_DISABLE     0   // Regular operation

#define  OSCCON_VALUE   0x73


#define  TRIS_BOOTLOADER_INDICATOR   TRISC,TRISC0
#define  BOOTLOADER_INDICATOR        LATC,LATC0


#define  UART_RX       PORTC,7
#define  LAT_UART_TX   LATC,6
#define  TRIS_UART_RX  TRISC,TRISC7
#define  TRIS_UART_TX  TRISC,TRISC6
#define  ANSEL_UART_RX ANSELC,ANSC7
#define  ANSEL_UART_TX ANSELC,ANSC6

CONFIGURE_IO  macro
    bsf     LAT_UART_TX          ; this fixes a phantom 0x80 character during the autobaud process
    bsf     TRIS_UART_TX         ; Setup tx pin Data sheet (end of chapter 25.0)
    bsf     TRIS_UART_RX         ; Setup rx pin specifies both should be set to 1.
    BANKSEL ANSELC   ; if the UART TX & RX pins have analog,
    bcf     ANSEL_UART_TX          ; uncomment these lines.
    bcf     ANSEL_UART_RX
    endm




#endif

