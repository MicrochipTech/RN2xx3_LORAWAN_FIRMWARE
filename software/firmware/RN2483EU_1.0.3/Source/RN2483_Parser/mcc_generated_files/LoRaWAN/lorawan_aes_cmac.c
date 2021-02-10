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
 *                           lorawan_aes_cmac.c
 *
 * LoRaWAN AES - CMAC file
 *
 ******************************************************************************/

/****************************** INCLUDES **************************************/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "lorawan_aes.h"
#include "lorawan_defs.h"

/************************ PRIVATE FUNCTION PROTOTYPES *************************/
static void GenerateSubkey (uint8_t* key, uint8_t* k1, uint8_t* k2);
static void FillSubKey( uint8_t *source, uint8_t *key, uint8_t size);

/****************************** PUBLIC FUNCTIONS ******************************/
void AESCmac(uint8_t* key, uint8_t* output, uint8_t* input, uint8_t size)
{
    uint8_t n = 0, i = 0, j =0;
    bool flag = false;
    uint8_t k1[16], k2[16];
    uint8_t x[16], y[16], mLast[16], padded[16];
    uint8_t *ptr = NULL;

    GenerateSubkey(key, k1, k2);

    n = (size + 15) >> SHIFT4;
    if (n == 0)
    {
        n = 1;
        flag = 0;
    }
    else
    {
        flag = !(size % 16);
    }

    if ( flag == 1 )
    {
        j = 0;
        for (i=((n-1) << SHIFT4); i<(n << SHIFT4); i++)
        {
            mLast[j] = input[i] ^ k1[j];
            j++;
        }
    }
    else
    {
        // padding
        ptr = &input[size - (size%16)];
        for (i=0; i<16; i++)
        {
            if ( i < (size%16) )
            {
                padded[i] = ptr[i];
            }
            else
            {
                if ( i == (size%16) )
                {
                    padded[i] = 0x80;
                }
                else
                {
                    padded[i] = 0x00;
                }
            }
        }

        // XOR
        for (i=0; i<sizeof(mLast); i++)
        {
            mLast[i] = padded[i] ^ k2[i];
        }
    }

    memset(x, 0, sizeof(x));

    for (i=0; i<(n-1); i++)
    {
        for (j=0; j<16; j++)
        {
            y[j] = x[j] ^ input[(i << SHIFT4)+j];
        }
        memcpy(x, y, sizeof(y));
        AESEncodeLoRa(x,key);
    }

    for (i=0; i<sizeof(x); i++)
    {
        y[i] = x[i] ^ mLast[i];
    }

    AESEncodeLoRa(y, key);

    memcpy(output, y, sizeof(y));
}

/****************************** PRIVATE FUNCTIONS *****************************/
static void GenerateSubkey (uint8_t* key, uint8_t* k1, uint8_t* k2)
{
    uint8_t i = 0;
    uint8_t l[16];
    uint8_t const_Rb[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87};


    memset(l, 0, sizeof(l));

    AESEncodeLoRa(l, key);

    // compute k1 sub-key
    if ( (l[0] & 0x80) == 0x00 )  // MSB( bufferLocal[0] ) is '0'
    {
        FillSubKey( l, k1, (sizeof(l) ) );
    }
    else
    {
        FillSubKey( l, k1, (sizeof(l) ) );

        for (i=0; i<sizeof(l); i++)
        {
            k1[i] ^= const_Rb[i];
        }
    }

    // compute k2 sub-key
    if ( (k1[0] & 0x80) == 0x00 )   // MSB( k1[0] ) is '0'
    {
        FillSubKey( k1, k2, (sizeof(l)) );
    }
    else
    {
        FillSubKey( k1, k2, (sizeof(l) ) );

        for (i=0; i<sizeof(l); i++)
        {
            k2[i] = k2[i] ^ const_Rb[i];
        }
    }
}

static void FillSubKey( uint8_t *source, uint8_t *key, uint8_t size)
{
    uint8_t i = 0;
    uint8_t carry = 0;

    i=size;

    while(i--)
    {
        key[i] = (source[i] << SHIFT1) | carry;
        carry = !!(source[i] & 0x80);
    }
}