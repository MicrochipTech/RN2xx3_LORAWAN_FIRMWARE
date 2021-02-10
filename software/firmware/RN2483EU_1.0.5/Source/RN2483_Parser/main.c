/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.16
        Device            :  PIC18LF46K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.35
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

#include "mcc_generated_files/mcc.h"

#include "system/system.h"
#include "sw_timer.h"
#include "system/system_low_power.h"

#include "radio_interface.h"
#include "radio_driver_hal.h"

#include "lorawan.h"
#include "lorawan_private.h"

#include "parser/parser.h"
#include "parser/parser_tsp.h"
#include "parser/parser_system.h"
#include "parser/parser_utils.h"
/*
                         Main application
 */

static uint16_t AppRestoreMacFromEep(void);

void main(void)
{
    uint16_t tempXor;
    uint16_t savedXor;
    uint16_t iCtr;
    uint8_t tempBuff[16];
    uint8_t temp;

    // Initialize the device
	SYSTEM_Initialize();
    FVR_DeInitialize();
    // If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
    // If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global and Peripheral Interrupts
	SysSleepInit(Parser_ExitFromSleep);
    
    //initialize parser	
	Parser_Init();

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
	//Check to see if anything was written to EEPROM
    savedXor = DATAEE_ReadByte(MAX_EEPROM_PARAM_INDEX - 1);
    temp = DATAEE_ReadByte(MAX_EEPROM_PARAM_INDEX);
    savedXor += (((uint16_t)temp) << 8);
    if(savedXor != 0xFFFF)
    {
        tempXor = AppRestoreMacFromEep();
		
        if(savedXor != tempXor)
        {
            //Reset LoRaWAN
            LORAWAN_Reset(ISM_EU868);            
			
			// Set default EUI to unique value
			System_GetExternalEui(tempBuff);
            LORAWAN_SetDeviceEui(tempBuff);
            Parser_SetConfiguredJoinParameters(0x01);
	
            //Delete parameters from EEPROM
            for(iCtr = 0; iCtr <= MAX_EEPROM_PARAM_INDEX ; iCtr++)
            {
                DATAEE_WriteByte(iCtr, 0xFF);
            }
        }        
    }
    else
    {
        //Unconfigured EEPROM        
		
        //Reset LoRaWAN
        LORAWAN_Reset(ISM_EU868);
		
        // Set default EUI to unique value
        System_GetExternalEui(tempBuff);
        LORAWAN_SetDeviceEui(tempBuff);
        Parser_SetConfiguredJoinParameters(0x01);        
    }
    
	//Display firmware version once the module is started
    Parser_GetSwVersion(aParserData);
    Parser_TxAddReply(aParserData, strlen(aParserData));

    
    while (1)
    {
        LORAWAN_Mainloop();
       
        //Exit sleep mode (if possible)
        SysExitFromSleep();        
        
        //Execute parser main function
        Parser_Main();
        
        //Enter sleep mode (if possible)
        SysGoToSleep();
    }
}
//Returns the checksum of the used EEPROM memory
static uint16_t AppRestoreMacFromEep(void)
{
    auint32_t temp32;
    uint16_t temp16;
    auint16_t itemp16;
    uint16_t tempXor;
    uint16_t startIdx;
    uint8_t tempBuff[16];
    uint8_t iCtr;
    uint8_t temp;
    uint8_t jCtr;

    parserConfiguredJoinParameters_t configuredJoinParameters;
    
    //Read the LoRaWAN configuration parameters from internal EEPROM
    startIdx = 0;

    //ISM frequency band
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor = temp;
    LORAWAN_Reset(temp);
    
    // Restore RX2 window parameters
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;            
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += temp32.buffer[iCtr];
    }  
    LORAWAN_SetReceiveWindow2Parameters(temp32.value, temp);

    // Restore configured join parameters
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;
    temp16 = temp;
    temp16 <<= 8U;
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;
    temp16 += temp;
    Parser_SetConfiguredJoinParameters(temp16);
    configuredJoinParameters.value = temp16;
    
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;
    LORAWAN_SetClass(temp);
    
    // Restore configured uplink counter
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += temp32.buffer[iCtr];
    }
    LORAWAN_SetUplinkCounter(temp32.value);

    // Restore configured downlink counter
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += temp32.buffer[iCtr];
    }
    LORAWAN_SetDownlinkCounter(temp32.value);

    //EEPROM[0..7] device EUI
    for(iCtr = 0U; iCtr < 8U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }

    if (1 == configuredJoinParameters.flags.deveui)
    {
        LORAWAN_SetDeviceEui(tempBuff);
    }

    //EEPROM[8..15] application EUI
    for(iCtr = 0U; iCtr < 8U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }

    if (1 == configuredJoinParameters.flags.appeui)
    {
        LORAWAN_SetApplicationEui(tempBuff);
    }

    //EEPROM[16..31] application key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }

    if (1 == configuredJoinParameters.flags.appkey)
    {
        LORAWAN_SetApplicationKey(tempBuff);
    }

    //EEPROM[32..47] network session key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }

    if (1 == configuredJoinParameters.flags.nwkskey)
    {
        LORAWAN_SetNetworkSessionKey(tempBuff);
    }

    //EEPROM[48..63] application session key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }

    if (1 == configuredJoinParameters.flags.appskey)
    {
        LORAWAN_SetApplicationSessionKey(tempBuff);
    }

    //EEPROM[64..68] device address
    temp32.value = 0;
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += temp32.buffer[iCtr];
    }

    if (1 == configuredJoinParameters.flags.devaddr)
    {
        LORAWAN_SetDeviceAddress(temp32.value);
    }
    
    //EEPROM[69..85] app multicast session key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }
    
    if (1 == configuredJoinParameters.flags.appmultiskey)
    {
        LORAWAN_SetMcastApplicationSessionKey(tempBuff);
    }
    
    //EEPROM[86..102] network multicast session key
    for(iCtr = 0U; iCtr < 16U; iCtr ++)
    {
        tempBuff[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += tempBuff[iCtr];
    }
    
    if (1 == configuredJoinParameters.flags.nwkmultiskey)
    {
        LORAWAN_SetMcastNetworkSessionKey(tempBuff);
    }
    
    //EEPROM[103..107] device multicast address
    temp32.value = 0;
    for(iCtr = 0U; iCtr < 4U; iCtr ++)
    {
        temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
        tempXor += temp32.buffer[iCtr];
    }
    
    if (1 == configuredJoinParameters.flags.devmultiaddr)
    {
        LORAWAN_SetMcastDeviceAddress(temp32.value);
    }
    
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;
    LORAWAN_SetMcast(temp);
    
    //EU specific parser code - channel list update
    //For the first 3 channels only the status and the duty cycle can be configured
    for(jCtr = 0U; jCtr < 3U; jCtr ++)
    {
        //Channel status
        //TODO: check if memory == 0 then status = disable
        temp = DATAEE_ReadByte(startIdx ++);
        tempXor += temp;
        LORAWAN_SetChannelIdStatus(jCtr, (temp > 0) ? 1: 0);

        //Channel DRRange
        temp = DATAEE_ReadByte(startIdx ++);
        tempXor += temp;
        LORAWAN_SetDataRange(jCtr, temp);

        //Channel dutycycle
        temp32.value = 0;
        for(iCtr = 0U; iCtr < 4U; iCtr ++)
        {
            temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
            tempXor += temp32.buffer[iCtr];
        }
        LORAWAN_SetDutyCycle(jCtr, temp32.value);
    }

    //Restore from EEPROM the parameters for the rest of the channels
    for(jCtr = 0U; jCtr < 29U; jCtr ++)
    {
        //Frequency
        temp32.value = 0;
        for(iCtr = 0U; iCtr < 4U; iCtr ++)
        {
            temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
            tempXor += temp32.buffer[iCtr];
        }
        LORAWAN_SetFrequency(jCtr + 3, temp32.value);

        //Dutycycle
        temp32.value = 0;
        for(iCtr = 0U; iCtr < 4U; iCtr ++)
        {
            temp32.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
            tempXor += temp32.buffer[iCtr];
        }
        LORAWAN_SetDutyCycle(jCtr + 3, temp32.value);

        //DR Range
        temp = DATAEE_ReadByte(startIdx ++);
        tempXor += temp;
        LORAWAN_SetDataRange(jCtr + 3, temp);

        //Status
        temp = DATAEE_ReadByte(startIdx ++);
        tempXor += temp;
        LORAWAN_SetChannelIdStatus(jCtr + 3, temp);
    }
    
    //Current data rate
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;    
    LORAWAN_SetCurrentDataRate(temp);
    
    //Current ADR state
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;    
    LORAWAN_SetAdr(temp);

    //Current Receive Offset
    temp = DATAEE_ReadByte(startIdx ++);
    tempXor += temp;    
    LORAWAN_SetReceiveOffset(temp);
    
    //Current Rx1Delay
    itemp16.value = 0;
        for(iCtr = 0U; iCtr < 2U; iCtr ++)
        {
            itemp16.buffer[iCtr] = DATAEE_ReadByte(startIdx ++);
            tempXor += itemp16.buffer[iCtr];
        }
        LORAWAN_SetReceiveDelay1(itemp16.value);
    
    
    if(tempXor == 0xFFFF)
    {
        tempXor = 0;
    }

    return tempXor;
}
/**
 End of File
 */
