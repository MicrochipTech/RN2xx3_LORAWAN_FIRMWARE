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
 *
 *                           system.c
 *
 * System management file
 *
 *
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M15723         2014.10.12        0.5
 ******************************************************************************/

#include "system.h"
#include <xc.h>
#include "../mcc_generated_files/mcc.h"
#include "../mcc_generated_files/i2c1.h"
#include "../mcc_generated_files/LoRaWAN/sw_timer.h"


void System_GetExternalEui(uint8_t *id)
{
    I2C1_MESSAGE_STATUS status;
    //uint8_t buffer[8];

    *id = 0xF8;
    I2C1_MasterWrite(id, 1, 0x50, &status);
    __delay_ms(1);
    
    I2C1_MasterRead(id, 8, 0x50, &status);
    __delay_ms(3);
}


uint16_t System_GetAnalogReading(uint8_t channel)
{
    uint16_t adcVal;
    ADCMD = 0;
    ADC_Initialize();
    // Is this really necessary?
    SystemBlockingWaitMs(2);
    
    adcVal = ADC_GetConversion((adc_channel_t)channel);

    ADCMD = 1;
    return adcVal;
}

void i2cWrite( uint8_t addr, uint8_t data )
{
    I2C1_MESSAGE_STATUS status;
    static uint8_t wrBuffer[2];

    wrBuffer[0] = addr;
    wrBuffer[1] = data;

    I2C1_MasterWrite(wrBuffer, 2, 0x50, &status);
    __delay_ms(5);
}


uint8_t i2cRead( uint8_t addr )
{
    I2C1_MESSAGE_STATUS status;
    static uint8_t data;

    I2C1_MasterWrite(&addr, 1, 0x50, &status);
    __delay_ms(1);

    I2C1_MasterRead(&data, 1, 0x50, &status);
    __delay_ms(3);
    return data;
} 

uint8_t nvm_read(uint16_t addr)
{
    if (addr < EEDATA_MAX_ADDR)
    {
        return DATAEE_ReadByte(addr);
    }

    addr -= EEDATA_MAX_ADDR;
    if (addr < EXTERN_MAX_ADDR)
    {
        return i2cRead(addr);
    }

    return 0x00;
}

void nvm_write(uint16_t addr, uint8_t data)
{
    if (addr < EEDATA_MAX_ADDR)
    {
        DATAEE_WriteByte(addr, data);
        return;
    }

    addr -= EEDATA_MAX_ADDR;
    if (addr < EXTERN_MAX_ADDR)
    {
        i2cWrite(addr, data);
        return;
    }
}

void FVR_DeInitialize()
{
    // FVRS off; FVREN disabled; FVRST disabled; 
    VREFCON0 = 0x00;
}

void SPI2_DeInitialize(void)
{   
    // SSPEN disabled; WCOL no_collision; SSPOV no_overflow; CKP Idle:Low, Active:High; SSPM FOSC/4; 
    SSP2CON1 = 0x00;
}
