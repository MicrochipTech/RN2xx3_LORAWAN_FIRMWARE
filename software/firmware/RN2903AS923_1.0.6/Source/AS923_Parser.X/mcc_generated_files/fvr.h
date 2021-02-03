/**
  FVR Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    fvr.h

  @Summary
    This is the generated header file for the FVR driver using MPLAB(c) Code Configurator

  @Description
    This header file provides APIs for driver for FVR.
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

#ifndef _FVR_H
#define _FVR_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

/**
  Section: FVR APIs
*/

/**
  @Summary
    Initializes the FVR

  @Description
    This routine initializes the FVR.
    This routine must be called before any other FVR routine is called.
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
    FVR_Initialize();
    </code>
*/
 void FVR_Initialize(void);

/**
  @Summary
    Gets the FVR output ready status.

  @Description
    This routine gets the FVR output ready status.

  @Preconditions
    The FVR_Initialize() routine should be called
    prior to use this routine.

  @Param
    None

  @Returns
     true  - FVR module is ready for use.
     false - FVR module is not ready for use.

  @Example
    <code>
    FVR_Initialize();

    if(FVR_IsOutputReady())
    {
          //user code
    }
    else
    {
          //user code
    }
    </code>
*/
bool FVR_IsOutputReady(void);

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif // _FVR_H
/**
 End of File
*/

