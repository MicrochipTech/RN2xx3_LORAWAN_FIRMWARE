/******************************************************************************
  @Company:
    Microchip Technology Inc.

  @File Name:
    lorawan_defs.h

  @Summary:
    This is the LoRaWAN utility definitions file 

  @Description:
    This header file provides LoRaWAN required initialization APIs declarations.
    Copyright (c) 2013 - 2016 released Microchip Technology Inc.  All rights reserved.

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
 *************************************************************************
 *                           lorawan_defs.h
 * 
 * LoRaWAN utility definitions header file
 *
 ******************************************************************************/

#ifndef MCC_LORA_DEFS_H
#define	MCC_LORA_DEFS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define SHIFT0                                  (0)
#define SHIFT1                                  (1)
#define SHIFT2                                  (2)
#define SHIFT3                                  (3)
#define SHIFT4                                  (4)
#define SHIFT5                                  (5)
#define SHIFT6                                  (6)
#define SHIFT7                                  (7)
#define SHIFT8                                  (8)
#define SHIFT9                                  (9)
#define SHIFT10                                 (10)
#define SHIFT11                                 (11)
#define SHIFT12                                 (12)
#define SHIFT13                                 (13)
#define SHIFT14                                 (14)
#define SHIFT15                                 (15)
#define SHIFT16                                 (16)
#define SHIFT17                                 (17)
#define SHIFT18                                 (18)
#define SHIFT19                                 (19)
#define SHIFT20                                 (20)
#define SHIFT21                                 (21)
#define SHIFT22                                 (22)
#define SHIFT23                                 (23)
#define SHIFT24                                 (24)
#define SHIFT25                                 (25)
#define SHIFT26                                 (26)
#define SHIFT27                                 (27)
#define SHIFT28                                 (28)
#define SHIFT29                                 (29)
#define SHIFT30                                 (30)
#define SHIFT31                                 (31)

#define BIT0                                    (1 << SHIFT0)
#define BIT1                                    (1 << SHIFT1)
#define BIT2                                    (1 << SHIFT2)
#define BIT3                                    (1 << SHIFT3)
#define BIT4                                    (1 << SHIFT4)
#define BIT5                                    (1 << SHIFT5)
#define BIT6                                    (1 << SHIFT6)
#define BIT7                                    (1 << SHIFT7)
#define BIT8                                    (1 << SHIFT8)
#define BIT9                                    (1 << SHIFT9)
#define BIT10                                   (1 << SHIFT10)
#define BIT11                                   (1 << SHIFT11)
#define BIT12                                   (1 << SHIFT12)
#define BIT13                                   (1 << SHIFT13)
#define BIT14                                   (1 << SHIFT14)
#define BIT15                                   (1 << SHIFT15)
#define BIT16                                   (1 << SHIFT16)
#define BIT17                                   (1 << SHIFT17)
#define BIT18                                   (1 << SHIFT18)
#define BIT19                                   (1 << SHIFT19)
#define BIT20                                   (1 << SHIFT20)
#define BIT21                                   (1 << SHIFT21)
#define BIT22                                   (1 << SHIFT22)
#define BIT23                                   (1 << SHIFT23)
#define BIT24                                   (1 << SHIFT24)
#define BIT25                                   (1 << SHIFT25)
#define BIT26                                   (1 << SHIFT26)
#define BIT27                                   (1 << SHIFT27)
#define BIT28                                   (1 << SHIFT28)
#define BIT29                                   (1 << SHIFT29)
#define BIT30                                   (1 << SHIFT30)
#define BIT31                                   (1 << SHIFT31)

#define FREQ_200KHZ                             200000
#define FREQ_500KHZ                             500000
#define FREQ_600KHZ                             600000
#define FREQ_1600KHZ                            1600000

#define FREQ_137000KHZ                          137000000
#define FREQ_175000KHZ                          175000000
#define FREQ_410000KHZ                          410000000
#define FREQ_433050KHZ                          433050000
#define FREQ_433300KHZ                          433300000
#define FREQ_434665KHZ                          434665000
#define FREQ_434790KHZ                          434790000
#define FREQ_525000KHZ                          525000000
#define FREQ_786000KHZ                          862000000
#define FREQ_862000KHZ                          862000000
#define FREQ_863000KHZ                          863000000
#define FREQ_868100KHZ                          868100000
#define FREQ_869525KHZ                          869525000
#define FREQ_870000KHZ                          870000000
#define FREQ_902300KHZ                          902300000
#define FREQ_903000KHZ                          903000000
#define FREQ_923300KHZ                          923300000
#define FREQ_927500KHZ                          927500000
#define FREQ_1020000KHZ                         1020000000

#define EU868_CALIBRATION_FREQ                  (FREQ_868100KHZ)
#define EU868_DEFAULT_RX_WINDOW2_DR             (DR0)
#define EU868_DEFAULT_RX_WINDOW2_FREQ           (FREQ_869525KHZ)

#define EU433_CALIBRATION_FREQ                  (FREQ_433300KHZ)
#define EU433_DEFAULT_RX_WINDOW2_DR             (DR0)
#define EU433_DEFAULT_RX_WINDOW2_FREQ           (FREQ_434665KHZ)

#define NA915_CALIBRATION_FREQ                  (FREQ_923300KHZ)
#define NA915_DEFAULT_RX_WINDOW2_DR             (DR8)
#define NA915_DEFAULT_RX_WINDOW2_FREQ           (FREQ_923300KHZ)

#define BATTERY_LEVEL_INVALID                   (0xFF)

#define NA915_UPSTREAM_CH0                      (FREQ_902300KHZ)
#define NA915_UPSTREAM_CH64                     (FREQ_903000KHZ)
#define NA915_DOWNSTREAM_CH0                    (FREQ_923300KHZ)

#ifdef	__cplusplus
}
#endif

#endif	/* MCC_LORA_DEFS_H */

/**
 End of File
*/