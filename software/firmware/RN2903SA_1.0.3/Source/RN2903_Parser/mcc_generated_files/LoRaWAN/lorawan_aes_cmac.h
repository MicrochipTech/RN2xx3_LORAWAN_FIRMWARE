/********************************************************************
 * Copyright (C) 2016 Microchip Technology Inc. and its subsidiaries
 * (Microchip).  All rights reserved.
 *
 * You are permitted to use the software and its derivatives with Microchip
 * products. See the license agreement accompanying this software, if any, for
 * more info about your rights and obligations.
 *
 * SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
 * MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR
 * PURPOSE. IN NO EVENT SHALL MICROCHIP, SMSC, OR ITS LICENSORS BE LIABLE OR
 * OBLIGATED UNDER CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH
 * OF WARRANTY, OR OTHER LEGAL EQUITABLE THEORY FOR ANY DIRECT OR INDIRECT
 * DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR OTHER SIMILAR COSTS. To the fullest
 * extend allowed by law, Microchip and its licensors liability will not exceed
 * the amount of fees, if any, that you paid directly to Microchip to use this
 * software.
 *************************************************************************
 *
 *                           lorawan_aes_cmac.h
 *
 * LoRaWAN AES - CMAC header file
 *
 ******************************************************************************/

#ifndef _LORAWAN_AES_CMAC_H
#define	_LORAWAN_AES_CMAC_H

#ifdef	__cplusplus
extern "C" {
#endif



/****************************** INCLUDES **************************************/

    
/****************************** DEFINES ***************************************/


/***************************** TYPEDEFS ***************************************/


/*************************** FUNCTIONS PROTOTYPE ******************************/
void AESCmac(uint8_t* key, uint8_t* output, uint8_t* input, uint8_t size);



#ifdef	__cplusplus
}
#endif

#endif	/* _LORAWAN_AES_CMAC_H */



