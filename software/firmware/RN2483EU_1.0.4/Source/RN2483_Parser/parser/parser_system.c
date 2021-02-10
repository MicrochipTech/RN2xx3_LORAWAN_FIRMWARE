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
 *                           parser_system.c
 *
 * This file contains all the commands used for testing the system peripherals
 *
 *
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M15723          2014.07.29        0.5
 * M18029		   2016.09.30	
 ******************************************************************************/

#include <xc.h>
#include <stdlib.h>
#include <string.h>
#include "parser_private.h"
#include "parser.h"
#include "parser_utils.h"
#include "parser_tsp.h"
#include "../system/system_low_power.h"
#include "../mcc_generated_files\mcc.h"
#include "../system/system.h"

static const char* gapParserSysStatus[] =
{
    {"ok"},
    {"invalid_param"},
    {"err"}
};

static const char* gapParserSysPinMode[] =
{
    {"digout"},
    {"digin"},
    {"ana"},
    {"sfun"}
};

enum
{
    PM_DIGOUT = 0x00,
    PM_DIGIN,
    PM_ANAIN,
    PM_SFUN
};

static const uint8_t gaAvailablePinFunctions[] = {
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN) | (1 << PM_ANAIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN),
    (1 << PM_DIGOUT) | (1 << PM_DIGIN)
};


void Parser_ExitFromSleep(void)
{
    Parser_RxClearBuffer();
    Parser_TxAddReply((char*)gapParserSysStatus[OK_STATUS_IDX], strlen(gapParserSysStatus[OK_STATUS_IDX]));
}

void Parser_SystemSleep(parserCmdInfo_t* pParserCmdInfo)
{
    uint32_t param1Value = (uint32_t)strtoul(pParserCmdInfo->pParam1, NULL, 10U);

    //TODO: remove (param1Value > 100) and only leave (param1Value > 0)
    if((Validate_UintDecAsciiValue(pParserCmdInfo->pParam1, 10, UINT32_MAX)) &&
       (param1Value >= 100))
    {
        SysSleepStart(param1Value);
    }
    else
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[INVALID_PARAM_IDX];
    }
}

void Parser_SystemBootload(parserCmdInfo_t* pParserCmdInfo)
{
    // Erase start of Application; this will allow Bootloader to wait for 0x55 (autobaud) command from GUI;
    FLASH_EraseBlock(0x00000300);
    // Allow Time to Erase
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    __delay_ms(6);
    // Issue Reset
    RESET();
}

void Parser_SystemReboot(parserCmdInfo_t* pParserCmdInfo)
{
    // Go for reboot, no reply necessary
    RESET();
}

void Parser_SystemFactReset(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t iCtr;

    //Delete EEPROM
    for(iCtr = 0; iCtr < 1024; iCtr ++)
    {
        DATAEE_WriteByte(iCtr, 0xFF);
    }
    
    // Go for reboot, no reply necessary
    RESET();
}

void Parser_SystemSetNvm(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t replyIdx = INVALID_PARAM_IDX;
    uint16_t param1Value;
    uint16_t param2Value; // the real value type is uint8_t, but strol truncates the value

    //TODO: set NVM restriction to protect the LoRaWAN params

    if(Validate_HexValue(pParserCmdInfo->pParam1) && Validate_HexValue(pParserCmdInfo->pParam2))
    {
        param1Value = strtol(pParserCmdInfo->pParam1, NULL, 16U);
        param2Value = strtol(pParserCmdInfo->pParam2, NULL, 16U);

        //LoRaWAN params are set between [0, USER_MEM_BASE): protected
        //User is allowed to write only between [USER_MEM_BASE, USER_MEM_BASE + USER_MEM_SIZE)
        if (param1Value >= USER_MEM_BASE && param1Value <  (USER_MEM_BASE + USER_MEM_SIZE))
        {
            if((param2Value > 0 && param2Value <= 255)  || strcmp("0", pParserCmdInfo->pParam2) == 0)
            {
                nvm_write(param1Value, param2Value);
                replyIdx = OK_STATUS_IDX;
            }
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[replyIdx];
}


void Parser_SystemGetNvm(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t replyIdx = INVALID_PARAM_IDX;
    uint8_t memLocation;
    uint16_t paramValue;


    if(Validate_HexValue(pParserCmdInfo->pParam1))
    {
        paramValue = strtol(pParserCmdInfo->pParam1, NULL, 16U);
        //LoRaWAN params are set between [0, USER_MEM_BASE): protected
        //User is allowed to write only between [USER_MEM_BASE, USER_MEM_BASE + USER_MEM_SIZE)
        if (paramValue >= USER_MEM_BASE && paramValue <  (USER_MEM_BASE + USER_MEM_SIZE))
        {
            memLocation = nvm_read(paramValue);
            utoa(aParserData, memLocation, 16U);
            pParserCmdInfo->pReplyCmd = aParserData;
            return;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[replyIdx];

}

void Parser_SystemSetPinMode(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t* pParam2 = (uint8_t*)pParserCmdInfo->pParam2;
    uint8_t status = INVALID_PARAM_IDX;
    int8_t pinNumber = -1;
    uint8_t i;
    
    pinNumber = Pin_Index(pParam1);
    
    if (pinNumber >= 0)
    {
        for (i = 0; i < sizeof gapParserSysPinMode; i++)
        {
            if (0 == strcmp(gapParserSysPinMode[i], pParam2))
            {
                // user requested pin function i
                if ((gaAvailablePinFunctions[pinNumber] & (1 << i)) != 0)
                {
                    // pin function i is available
                    status = OK_STATUS_IDX;
                    switch (pinNumber)
                    {
                        case 0:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO0_SetDigitalMode();
                                    GPIO0_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO0_SetDigitalMode();
                                    GPIO0_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO0_SetDigitalInput();
                                    GPIO0_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 1:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO1_SetDigitalMode();
                                    GPIO1_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO1_SetDigitalMode();
                                    GPIO1_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO1_SetDigitalInput();
                                    GPIO1_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 2:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO2_SetDigitalMode();
                                    GPIO2_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO2_SetDigitalMode();
                                    GPIO2_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO2_SetDigitalInput();
                                    GPIO2_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 3:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO3_SetDigitalMode();
                                    GPIO3_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO3_SetDigitalMode();
                                    GPIO3_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO3_SetDigitalInput();
                                    GPIO3_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 4:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO4_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO4_SetDigitalInput();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 5:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO5_SetDigitalMode();
                                    GPIO5_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO5_SetDigitalMode();
                                    GPIO5_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO5_SetDigitalInput();
                                    GPIO5_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 6:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO6_SetDigitalMode();
                                    GPIO6_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO6_SetDigitalMode();
                                    GPIO6_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO6_SetDigitalInput();
                                    GPIO6_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 7:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO7_SetDigitalMode();
                                    GPIO7_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO7_SetDigitalMode();
                                    GPIO7_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO7_SetDigitalInput();
                                    GPIO7_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 8:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO8_SetDigitalMode();
                                    GPIO8_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO8_SetDigitalMode();
                                    GPIO8_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO8_SetDigitalInput();
                                    GPIO8_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 9:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO9_SetDigitalMode();
                                    GPIO9_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO9_SetDigitalMode();
                                    GPIO9_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO9_SetDigitalInput();
                                    GPIO9_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 10:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO10_SetDigitalMode();
                                    GPIO10_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO10_SetDigitalMode();
                                    GPIO10_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO10_SetDigitalInput();
                                    GPIO10_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 11:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO11_SetDigitalMode();
                                    GPIO11_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO11_SetDigitalMode();
                                    GPIO11_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO11_SetDigitalInput();
                                    GPIO11_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 12:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO12_SetDigitalMode();
                                    GPIO12_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO12_SetDigitalMode();
                                    GPIO12_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO12_SetDigitalInput();
                                    GPIO12_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 13:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    GPIO13_SetDigitalMode();
                                    GPIO13_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    GPIO13_SetDigitalMode();
                                    GPIO13_SetDigitalInput();
                                    break;
                                case PM_ANAIN:
                                    GPIO13_SetDigitalInput();
                                    GPIO13_SetAnalogMode();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 14:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    UART_RTS_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    UART_RTS_SetDigitalInput();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 15:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    UART_CTS_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    UART_CTS_SetDigitalInput();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 16:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    TEST0_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    TEST0_SetDigitalInput();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                        case 17:
                            switch (i)
                            {
                                case PM_DIGOUT:
                                    TEST1_SetDigitalOutput();
                                    break;
                                case PM_DIGIN:
                                    TEST1_SetDigitalInput();
                                    break;
                                case PM_SFUN:
                                    
                                    break;
                            }
                            break;
                    }   
                }
            }
        }
    }
    
    pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[status];
}

void Parser_SystemSetPinDig(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t* pParam2 = (uint8_t*)pParserCmdInfo->pParam2;
    uint8_t status = INVALID_PARAM_IDX;
    int8_t pinNumber = -1;
    int8_t pinValue = -1;

    pinNumber = Pin_Index(pParam1);
    
    if(Validate_UintDecAsciiValue(pParam2, 1, 1))
    {
        pinValue = atoi(pParam2);
    }

    if ((pinNumber >= 0) && (pinValue >= 0))
    {
        status = OK_STATUS_IDX;
        switch (pinNumber)
        {
            case 0:
                if (0 == pinValue)
                    GPIO0_SetLow();
                else
                    GPIO0_SetHigh();
                break;
            case 1:
                if (0 == pinValue)
                    GPIO1_SetLow();
                else
                    GPIO1_SetHigh();
                break;
            case 2:
                if (0 == pinValue)
                    GPIO2_SetLow();
                else
                    GPIO2_SetHigh();
                break;
            case 3:
                if (0 == pinValue)
                    GPIO3_SetLow();
                else
                    GPIO3_SetHigh();
                break;
            case 4:
                if (0 == pinValue)
                    GPIO4_SetLow();
                else
                    GPIO4_SetHigh();
                break;
            case 5:
                if (0 == pinValue)
                    GPIO5_SetLow();
                else
                    GPIO5_SetHigh();
                break;
            case 6:
                if (0 == pinValue)
                    GPIO6_SetLow();
                else
                    GPIO6_SetHigh();
                break;
            case 7:
                if (0 == pinValue)
                    GPIO7_SetLow();
                else
                    GPIO7_SetHigh();
                break;
            case 8:
                if (0 == pinValue)
                    GPIO8_SetLow();
                else
                    GPIO8_SetHigh();
                break;
            case 9:
                if (0 == pinValue)
                    GPIO9_SetLow();
                else
                    GPIO9_SetHigh();
                break;
            case 10:
                if (0 == pinValue)
                    GPIO10_SetLow();
                else
                    GPIO10_SetHigh();
                break;
            case 11:
                if (0 == pinValue)
                    GPIO11_SetLow();
                else
                    GPIO11_SetHigh();
                break;
            case 12:
                if (0 == pinValue)
                    GPIO12_SetLow();
                else
                    GPIO12_SetHigh();
                break;
            case 13:
                if (0 == pinValue)
                    GPIO13_SetLow();
                else
                    GPIO13_SetHigh();
                break;
            case 14:
                if (0 == pinValue)
                    UART_RTS_SetLow();
                else
                    UART_RTS_SetHigh();
                break;
            case 15:
                if (0 == pinValue)
                    UART_CTS_SetLow();
                else
                    UART_CTS_SetHigh();
                break;
            case 16:
                if (0 == pinValue)
                    TEST0_SetLow();
                else
                    TEST0_SetHigh();
                break;
            case 17:
                if (0 == pinValue)
                    TEST1_SetLow();
                else
                    TEST1_SetHigh();
                break;
        }
    }

    pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[status];
}

void Parser_SystemGetPinDig(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;
    int8_t pinNumber = -1;
    int8_t pinValue = -1;

    pinNumber = Pin_Index(pParam1);
    
    if (pinNumber >= 0)
    {
        switch (pinNumber)
        {
            case 0:
                pinValue = GPIO0_GetValue();
                break;
            case 1:
                pinValue = GPIO1_GetValue();
                break;
            case 2:
                pinValue = GPIO2_GetValue();
                break;
            case 3:
                pinValue = GPIO3_GetValue();
                break;
            case 4:
                pinValue = GPIO4_GetValue();
                break;
            case 5:
                pinValue = GPIO5_GetValue();
                break;
            case 6:
                pinValue = GPIO6_GetValue();
                break;
            case 7:
                pinValue = GPIO7_GetValue();
                break;
            case 8:
                pinValue = GPIO8_GetValue();
                break;
            case 9:
                pinValue = GPIO9_GetValue();
                break;
            case 10:
                pinValue = GPIO10_GetValue();
                break;
            case 11:
                pinValue = GPIO11_GetValue();
                break;
            case 12:
                pinValue = GPIO12_GetValue();
                break;
            case 13:
                pinValue = GPIO13_GetValue();
                break;
            case 14:
                pinValue = UART_RTS_GetValue();
                break;
            case 15:
                pinValue = UART_CTS_GetValue();
                break;
            case 16:
                pinValue = TEST0_GetValue();
                break;
            case 17:
                pinValue = TEST1_GetValue();
                break;
        }
        status = OK_STATUS_IDX;
    }

    if (status != OK_STATUS_IDX)
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[status];
    }
    else
    {
        utoa(aParserData, pinValue, 10U);
        pParserCmdInfo->pReplyCmd = aParserData;
    }
}

void Parser_SystemGetPinAnalog(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t* pParam1 = (uint8_t*)pParserCmdInfo->pParam1;
    uint8_t status = INVALID_PARAM_IDX;
    int8_t pinNumber = -1;
    uint16_t pinValue;

    pinNumber = Pin_Index(pParam1);
    
    if (pinNumber >= 0)
    {
        if ((gaAvailablePinFunctions[pinNumber] & (1 << PM_ANAIN)) != 0)
        {
            // Selected pin has analog functionality
            status = OK_STATUS_IDX;
            switch (pinNumber)
            {
                case 0:
                    pinValue = System_GetAnalogReading(0);
                    break;
                case 1:
                    pinValue = System_GetAnalogReading(1);
                    break;
                case 2:
                    pinValue = System_GetAnalogReading(2);
                    break;
                case 3:
                    pinValue = System_GetAnalogReading(3);
                    break;
                case 5:
                    pinValue = System_GetAnalogReading(4);
                    break;
                case 6:
                    pinValue = System_GetAnalogReading(5);
                    break;
                case 7:
                    pinValue = System_GetAnalogReading(6);
                    break;
                case 8:
                    pinValue = System_GetAnalogReading(22);
                    break;
                case 9:
                    pinValue = System_GetAnalogReading(7);
                    break;
                case 10:
                    pinValue = System_GetAnalogReading(17);
                    break;
                case 11:
                    pinValue = System_GetAnalogReading(25);
                    break;
                case 12:
                    pinValue = System_GetAnalogReading(27);
                    break;
                case 13:
                    pinValue = System_GetAnalogReading(26);
                    break;
            }
        }
    }

    if (status != OK_STATUS_IDX)
    {
        pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[status];
    }
    else
    {
        utoa(aParserData, pinValue, 10U);
        pParserCmdInfo->pReplyCmd = aParserData;
    }
}


void Parser_SystemGetVer(parserCmdInfo_t* pParserCmdInfo)
{
    Parser_GetSwVersion(aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}

void Parser_SystemGetBattery(parserCmdInfo_t* pParserCmdInfo)
{
    uint16_t refVoltage;
    uint32_t batteryVoltage;

    pParserCmdInfo->pReplyCmd = (char*)gapParserSysStatus[ERR_STATUS_IDX];

    ADCMD = 0;

    ADC_Initialize();
    FVR_Initialize();

    while(!FVR_IsOutputReady());

    refVoltage = ADC_GetConversion(channel_FVRBuf2);

    FVR_DeInitialize();

    ADCMD = 1;

    if (refVoltage == 0)
    {
        batteryVoltage = 0xFFFF;
    }
    else
    {
        batteryVoltage = 1047552; // 1023 (max 10 bit value) * 1024 (mV)
        batteryVoltage = batteryVoltage / refVoltage; // value in mV
    }

    if (0xFFFF != batteryVoltage)
    {
        utoa(aParserData, batteryVoltage, 10U);
        pParserCmdInfo->pReplyCmd = aParserData;
    }
}

void Parser_SystemGetHwEui(parserCmdInfo_t* pParserCmdInfo)
{
    uint8_t aDevEui[8];

    System_GetExternalEui(aDevEui);
    Parser_IntArrayToHexAscii(8, aDevEui, aParserData);
    pParserCmdInfo->pReplyCmd = aParserData;
}