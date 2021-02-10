/******************************************************************************
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
*
*************************************************************************
 *                           system_timer.h
 *
 * System timer header file
 *
 *
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M15723          2014.12.04        0.5
 * M18029		   2016.09.30		
 ******************************************************************************/


#ifndef _SYSTEM_H
#define	_SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
    
#define MAX_EEPROM_PARAM_INDEX                      0x18AU 
#define USER_MEM_BASE       0x0300
#define USER_MEM_SIZE       0x0100
#define EEDATA_MAX_ADDR     0x4000
#define EXTERN_MAX_ADDR     0x8000
#define UNIQUE_ID_OFFSET    0x80F8
    
void System_GetExternalEui(uint8_t *id);
uint16_t System_GetAnalogReading(uint8_t channel);

void i2cWrite( uint8_t addr, uint8_t data );
uint8_t i2cRead( uint8_t addr );

uint8_t nvm_read(uint16_t addr);
void nvm_write(uint16_t addr, uint8_t data);

void FVR_DeInitialize();

void SPI2_DeInitialize();

#ifdef	__cplusplus
}
#endif

#endif	/* _SYSTEM_H */

