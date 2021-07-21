/**
  EUSART1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart1.c

  @Summary
    This is the generated driver implementation file for the EUSART1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides implementations for driver APIs for EUSART1.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.0
        Device            :  PIC18LF46K22
        Driver Version    :  2.00
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

/**
  Section: Included Files
*/
#include "eusart1.h"
#include "../system/system_low_power.h"
#include "../parser/parser_tsp.h"
#include "interrupt_manager.h"
#include "pin_manager.h"
#include "mcc_lora_config.h"

/**
  Section: Macro Declarations
*/
#define EUSART1_TX_BUFFER_SIZE 8
#define EUSART1_RX_BUFFER_SIZE 32


#define EUSART1_WUE     BAUDCON1bits.WUE
#define EUSART1_ABDEN   BAUDCON1bits.ABDEN
#define EUSART1_ABDOVF  BAUDCON1bits.ABDOVF
#define EUSART1_SPBRG16 ((SPBRGH1 << 8) | SPBRG1)
        
#define MIN_BAUD_ERROR_110  0x9120UL    /* 37152U -    110 baud -2.12% */
#define MAX_BAUD_230400     0x10UL      /* 16U    - 230400 baud +2.12% */

#define MIN_BREAK_TIME      0x30       /* (9-bits / (115200 + 2.12%) * FOSC/4) - instruction overhead


/**
  Section: Global Variables
*/

static uint8_t eusart1TxHead = 0;
static uint8_t eusart1TxTail = 0;
static uint8_t eusart1TxBuffer[EUSART1_TX_BUFFER_SIZE];
volatile uint8_t eusart1TxBufferRemaining;

static uint8_t eusart1RxHead = 0;
static uint8_t eusart1RxTail = 0;
static uint8_t eusart1RxBuffer[EUSART1_RX_BUFFER_SIZE];
volatile uint8_t eusart1RxCount;

volatile static UARTOperationState_t    UARTOperationState  = UART_NORMAL;
volatile static uint16_t                lastBaudrate        = 0x44;         // default to 57600 baud


/**
  Section: EUSART1 APIs
*/
void EUSART1_SetOperationState(UARTOperationState_t newState)
{
    uint8_t currentGIE = INTERRUPT_GlobalInterruptStatus();
    
    INTERRUPT_GlobalInterruptDisable();
    UARTOperationState  = newState;
    INTERRUPT_GlobalInterruptSet(currentGIE);   // Never enable GIE inside an ISR
}

UARTOperationState_t EUSART1_GetOperationState(void)
{
    return UARTOperationState;
}

void EUSART1_ResetOperationState(void)
{
    EUSART1_ABDEN   = 0;            // disable auto baud detection
    EUSART1_ABDOVF  = 0;            // clear ABDOVF flag
    EUSART1_WUE     = 0;            // disable UART wake
    
    EUSART1_SetOperationState(UART_NORMAL);
}

void EUSART1_PrepareForSleep(void)
{
    EUSART1_ABDEN   = 0;        // disable auto baud detection
    EUSART1_ABDOVF  = 0;        // clear ABDOVF flag
    EUSART1_WUE     = 1;        // enable UART wake
    
    EUSART1_SetOperationState(UART_SLEEP);
}

void EUSART1_StartABD(void)
{
//    while(EUSART1_WUE){}                // wait for break condition to end
    
    lastBaudrate    = EUSART1_SPBRG16;  // save the current baudrate before enabling ABDEN
    EUSART1_ABDEN   = 1;                // enable UART auto baud detection
    EUSART1_SetOperationState(UART_WAIT_FOR_AUTOBAUD);
}

uint8_t EUSART1_CheckAutobaud(void)
{
    uint8_t     result = false;
    

    if(EUSART1_ABDFailed())
    {
        // An auto baud error or timeout has occurred
        // goToSleep is set to SLEEP_ON in the function OnABDTimerEvent
        EUSART1_RestoreBaudrate();
    }
    else
    {
        // Auto baud completed successfully
        result  = true;
        uint16_t baud   = EUSART1_SPBRG16;
        baud    -= 1;   // correct baud - ABD counter starts at 1 not 0
        SPBRG1  = (baud & 0xff);            // set baudrate
        SPBRGH1 = ((baud >> 8) & 0x00ff);   // set baudrate high byte
    }
    
    EUSART1_ResetOperationState();  // reset UART to normal operation

    return result;
}

uint8_t EUSART1_ABDFailed(void)
{
    uint16_t baud   = EUSART1_SPBRG16;
    return ((EUSART1_ABDOVF      == 1)
        || (MIN_BAUD_ERROR_110 < baud)
        || (MAX_BAUD_230400    > baud));
}

void EUSART1_RestoreBaudrate(void)
{
    EUSART1_ABDEN   = 0;                        // disable auto baud detection
    SPBRG1  = (lastBaudrate & 0xff);            // restore baudrate
    SPBRGH1 = ((lastBaudrate >> 8) & 0x00ff);   // restore baudrate high byte
}

void EUSART1_Initialize(void)
{
    // disable interrupts before changing states
    PIE1bits.RC1IE = 0;
    PIE1bits.TX1IE = 0;

    // Set the EUSART1 module to the options selected in the user interface.

    // ABDOVF no_overflow; CKTXP async_noninverted_sync_fallingedge; BRG16 16bit_generator; WUE disabled; ABDEN disabled; DTRXP not_inverted; 
    BAUDCON1 = 0x08;

    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    RCSTA1 = 0x90;

    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
    TXSTA1 = 0x24;

    // Baud Rate = 57600; 
    SPBRG1 = 0x44;

    // Baud Rate = 57600; 
    SPBRGH1 = 0x00;


    // initializing the driver state
    eusart1TxHead = 0;
    eusart1TxTail = 0;
    eusart1TxBufferRemaining = sizeof(eusart1TxBuffer);

    eusart1RxHead = 0;
    eusart1RxTail = 0;
    eusart1RxCount = 0;
    
    EUSART1_SetOperationState(UART_NORMAL);

    // enable receive interrupt
    PIE1bits.RC1IE = 1;
}

uint8_t EUSART1_Read(void)
{
    uint8_t readValue  = 0;
    
    while(0 == eusart1RxCount)
    {
    }

    readValue = eusart1RxBuffer[eusart1RxTail++];
    if(sizeof(eusart1RxBuffer) <= eusart1RxTail)
    {
        eusart1RxTail = 0;
    }
    PIE1bits.RC1IE = 0;
    eusart1RxCount--;
    PIE1bits.RC1IE = 1;

    return readValue;
}

void EUSART1_Write(uint8_t txData)
{
    while(0 == eusart1TxBufferRemaining)
    {
    }

    if(0 == PIE1bits.TX1IE)
    {
        TXREG1 = txData;
    }
    else
    {
        PIE1bits.TX1IE = 0;
        eusart1TxBuffer[eusart1TxHead++] = txData;
        if(sizeof(eusart1TxBuffer) <= eusart1TxHead)
        {
            eusart1TxHead = 0;
        }
        eusart1TxBufferRemaining--;
    }
    PIE1bits.TX1IE = 1;
}

void EUSART1_Transmit_ISR(void)
{

    // add your EUSART1 interrupt custom code
    if(sizeof(eusart1TxBuffer) > eusart1TxBufferRemaining)
    {
        TXREG1 = eusart1TxBuffer[eusart1TxTail++];
        if(sizeof(eusart1TxBuffer) <= eusart1TxTail)
        {
            eusart1TxTail = 0;
        }
        eusart1TxBufferRemaining++;
    }
    else
    {
        PIE1bits.TX1IE = 0;
    }
}

void EUSART1_Receive_ISR(void)
{
    uint8_t rxRead;
    
#if defined(DEBUG_SLEEP)
    GPIO13_Pulse();
    GPIO13_Pulse();
#endif // DEBUG_SLEEP

    if(1 == RCSTA1bits.OERR)
    {
        // EUSART1 error - restart
        // buffer overruns are ignored

        RCSTA1bits.CREN = 0;
        RCSTA1bits.CREN = 1;
    }
    
    
    if (RCSTAbits.FERR == 1)
    {
        if (RCREG1 == 0x00)
        {
            RCSTAbits.SPEN = 0; /* Force clear FERR by resetting the EUSART */
            NOP();
            RCSTAbits.SPEN = 1; /* Enable EUSART */
            NOP();
            EUSART1_StartABD();
        }
    }
    else
    {
        
        rxRead = RCREG1;    // get Rx byte and clear RCxIF flag

        switch(UARTOperationState)
        {
            case UART_NORMAL:
                /* Add another character in the parser receive buffer (if possible) */
                eusart1RxBuffer[eusart1RxHead++] = rxRead;
                if(sizeof(eusart1RxBuffer) <= eusart1RxHead)
                {
                    eusart1RxHead = 0;
                }
                eusart1RxCount++;
                break;


            case UART_SLEEP:
                // Woke from sleep due to falling edge detected on Rx line.
                
                T0CONbits.TMR0ON    = 0;        // stop timer
                TMR0                = 0x00;     // clear TMR0
                INTCONbits.TMR0IF   = 0;        // clear TMR0IF
                T0CONbits.TMR0ON    = 1;        // start timer
                while(EUSART1_WUE){}            // wait for break condition to end
                T0CONbits.TMR0ON    = 0;        // stop timer
                
                if(INTCONbits.TMR0IF || (MIN_BREAK_TIME < TMR0))
                {
                    EUSART1_StartABD();
                    EUSART1_SetOperationState(UART_WUE_WAKE);
                }
                else
                {
                    // Rx line not low long enough, ignore this Rx activity.
                    // Allow the mainline code to return to sleep.
                }
                break;
            case UART_WUE_WAKE:
                // The first interrupt in the UART_WUE_WAKE state should
                // indicate that the autobaud sync character has been
                // received.  The work of the EUSART ISR is complete.  Signal
                // the mainline code to check the new baudrate.
                EUSART1_SetOperationState(UART_CHECK_AUTOBAUD);
                break;
            case UART_WAIT_FOR_AUTOBAUD:
                // system_low_power initiated ABD and the auto baud hw
                // requirements have been met (5 rising edges on Rx line).
                EUSART1_SetOperationState(UART_CHECK_AUTOBAUD);
                break;
            case UART_CHECK_AUTOBAUD:
                // this state is handled in the mainline code
                break;
            default:
                // The remaining states the UARTOperationState are
                // handled in the mainline code
                break;
        }
    }
}
/**
  End of File
*/
