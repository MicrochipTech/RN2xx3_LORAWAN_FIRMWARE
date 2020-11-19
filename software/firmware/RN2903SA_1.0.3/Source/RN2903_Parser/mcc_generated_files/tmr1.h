/**
  TMR1 Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr1.h

  @Summary
    This is the generated header file for the TMR1 driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for TMR1.
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

#ifndef _TMR1_H
#define _TMR1_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

#define TMR1_INTERRUPT_TICKER_FACTOR    1

/**
  Section: TMR1 APIs
*/

/**
  @Summary
    Initializes the TMR1

  @Description
    This routine initializes the TMR1.
    This routine must be called before any other TMR1 routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    

  @Example
    <code>
    TMR1_Initialize();
    </code>
*/
void TMR1_Initialize(void);

/**
  @Summary
    Start TMR1

  @Description
    This routine is used to  Start TMR1.

  @Preconditions
    The TMR1_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    // Initialize TMR1 module

    // Start TMR1
    TMR1_StartTimer();

    while(1)
    {
    }
    </code>
 */
void TMR1_StartTimer(void);

/**
  @Summary
    Stop TMR1

  @Description
    This routine is used to  Stop TMR1.

  @Preconditions
    The TMR1_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    None

  @Example
    </code>
    TMR1_Initialize();

    TMR1_StartTimer();
    if(TMR1_HasOverflowOccured())
    {
        TMR1_StopTimer();
    }
    <code>
 */
void TMR1_StopTimer(void);

/**
  @Summary
    Read TMR1 register.

  @Description
    This routine is used to  Read TMR1 register.

  @Preconditions
    The TMR1_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    TMR1 value at the time of the function call read as a 16-bit value

  @Example
    <code>
    uint16_t timerVal=0;

    TMR1_Initialize();

    TMR1_StartTimer();

    // some delay or code

    TMR1_StopTimer();

    timerVal=TMR1_ReadTimer();
    </code>
 */
uint16_t TMR1_ReadTimer(void);

/**
  @Summary
    Write TMR1 register.

  @Description
    This routine is used to Write TMR1 register.

  @Preconditions
    The TMR1_Initialize() routine should be called
    prior to use this routine.

  @Param
    timerVal : Timer value to be loaded

  @Returns
    None

  @Example
    <code>
    TMR1_Initialize();
    TMR1_WriteTimer(0x055);
    TMR1_StartTimer();
    </code>
 */
void TMR1_WriteTimer(uint16_t timerVal);

/**
  @Summary
    Reload TMR1 register.

  @Description
    This routine is used to reload TMR1 register.

  @Preconditions
    The TMR1_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    TMR1_Initialize();
    TMR1_StartTimer();

    if(TMR1_HasOverflowOccured())
    {
        TMR1_StopTimer();
    }

    TMR1_Reload();}
    </code>
*/
void TMR1_Reload(void);

/**
  @Summary
    Starts the single pulse acquisition in TMR1 gate operation.

  @Description
    This function starts the single pulse acquisition in TMR1 gate operation.
    This function must be used when the TMR1 gate is enabled.

  @Preconditions
    Initialize  the TMR1 with gate enable before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    uint16_t xVal;
    uint16_t yVal;

    //enable TMR1 singlepulse mode
    TMR1_StartSinglePulseAcquistion();

    //check TMR1 gate status
    if(TMR1_CheckGateValueStatus()== 0)
    {
        xVal = TMR1_ReadTimer();
    }

    // wait untill gate interrupt occured
    while(TMR1GIF == 0)
    {
    }

    yVal = TMR1_ReadTimer();
    </code>
*/
void TMR1_StartSinglePulseAcquisition(void);

/**
  @Summary
    Check the current state of Timer1 gate.

  @Description
    This function reads the TMR1 gate value and return it.
    This function must be used when the TMR1 gate is enabled.

  @Preconditions
    Initialize  the TMR1 with gate enable before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    uint16_t xVal;
    uint16_t yVal;

    //enable TMR1 singlepulse mode
    TMR1_StartSinglePulseAcquistion();

    //check TMR1 gate status
    if(TMR1_CheckGateValueStatus()== 0)
    {
        xVal = TMR1_ReadTimer();
    }

    //wait untill gate interrupt occured
    while(TMR1IF == 0)
    {
    }

    yVal = TMR1_ReadTimer();
    </code>
*/
uint8_t TMR1_CheckGateValueStatus(void);

/**
  @Summary
    Implements ISR

  @Description
    This routine is used to implement the ISR for the interrupt-driven
    implementations.

  @Returns
    None

  @Param
    None
*/
void TMR1_ISR(void);

/**
  @Summary
    CallBack function.

  @Description
    This routine is called by the Interrupt Manager.

  @Preconditions
    The TMR1_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
    None
*/
void TMR1_CallBack(void);

/**
  @Summary
    Set Timer Interrupt Handler

  @Description
    This sets the function to be called during the ISR

  @Preconditions
    Initialize  the TMR1 module with interrupt before calling this.

  @Param
    Address of function to be set

  @Returns
    None
*/
 void TMR1_SetInterruptHandler(void *InterruptHandler);

/**
  @Summary
    Timer Interrupt Handler

  @Description
    This is a function pointer to the function that will be called during the ISR

  @Preconditions
    Initialize  the TMR1 module with interrupt before calling this isr.

  @Param
    None

  @Returns
    None
*/
extern void (*TMR1_InterruptHandler)(void);

/**
  @Summary
    Default Timer Interrupt Handler

  @Description
    This is the default Interrupt Handler function

  @Preconditions
    Initialize  the TMR1 module with interrupt before calling this isr.

  @Param
    None

  @Returns
    None
*/
void TMR1_DefaultInterruptHandler(void);


#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // _TMR1_H
/**
 End of File
*/