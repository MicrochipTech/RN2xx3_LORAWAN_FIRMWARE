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
*                           AES.c
 *
 * AES file
 *
 *
 * Hardware:
 *  USB Carrier with RN2483 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M91511          2014.12.12        0.5
 ******************************************************************************/


#include <stdlib.h>
#include <stdint.h>

#include "AES.h"

//#define block_copy_nn(d, s, l)    copy_block_nn(d, s, l)
//#define block_copy(d, s)          copy_block(d, s)


#define BLOCKSIZE 16

/****************************** Variables ***************************/
static unsigned char _roundCounter;
static unsigned char _rcon;

/****************************** Macros ******************************/
#define xtime(a) (((a)<0x80)?(a)<<1:(((a)<<1)^0x1b) )

const unsigned char STable[] =
{
	0x63,0x7C,0x77,0x7B,0xF2,0x6B,0x6F,0xC5,0x30,0x01,0x67,0x2B,0xFE,0xD7,0xAB,0x76,
	0xCA,0x82,0xC9,0x7D,0xFA,0x59,0x47,0xF0,0xAD,0xD4,0xA2,0xAF,0x9C,0xA4,0x72,0xC0,
	0xB7,0xFD,0x93,0x26,0x36,0x3F,0xF7,0xCC,0x34,0xA5,0xE5,0xF1,0x71,0xD8,0x31,0x15,
	0x04,0xC7,0x23,0xC3,0x18,0x96,0x05,0x9A,0x07,0x12,0x80,0xE2,0xEB,0x27,0xB2,0x75,
	0x09,0x83,0x2C,0x1A,0x1B,0x6E,0x5A,0xA0,0x52,0x3B,0xD6,0xB3,0x29,0xE3,0x2F,0x84,
	0x53,0xD1,0x00,0xED,0x20,0xFC,0xB1,0x5B,0x6A,0xCB,0xBE,0x39,0x4A,0x4C,0x58,0xCF,
	0xD0,0xEF,0xAA,0xFB,0x43,0x4D,0x33,0x85,0x45,0xF9,0x02,0x7F,0x50,0x3C,0x9F,0xA8,
	0x51,0xA3,0x40,0x8F,0x92,0x9D,0x38,0xF5,0xBC,0xB6,0xDA,0x21,0x10,0xFF,0xF3,0xD2,
	0xCD,0x0C,0x13,0xEC,0x5F,0x97,0x44,0x17,0xC4,0xA7,0x7E,0x3D,0x64,0x5D,0x19,0x73,
	0x60,0x81,0x4F,0xDC,0x22,0x2A,0x90,0x88,0x46,0xEE,0xB8,0x14,0xDE,0x5E,0x0B,0xDB,
	0xE0,0x32,0x3A,0x0A,0x49,0x06,0x24,0x5C,0xC2,0xD3,0xAC,0x62,0x91,0x95,0xE4,0x79,
	0xE7,0xC8,0x37,0x6D,0x8D,0xD5,0x4E,0xA9,0x6C,0x56,0xF4,0xEA,0x65,0x7A,0xAE,0x08,
	0xBA,0x78,0x25,0x2E,0x1C,0xA6,0xB4,0xC6,0xE8,0xDD,0x74,0x1F,0x4B,0xBD,0x8B,0x8A,
	0x70,0x3E,0xB5,0x66,0x48,0x03,0xF6,0x0E,0x61,0x35,0x57,0xB9,0x86,0xC1,0x1D,0x9E,
	0xE1,0xF8,0x98,0x11,0x69,0xD9,0x8E,0x94,0x9B,0x1E,0x87,0xE9,0xCE,0x55,0x28,0xDF,
	0x8C,0xA1,0x89,0x0D,0xBF,0xE6,0x42,0x68,0x41,0x99,0x2D,0x0F,0xB0,0x54,0xBB,0x16
};


//static void copy_block( void *d, const void *s )
//{
//    ((uint8_t*)d)[ 0] = ((uint8_t*)s)[ 0];
//    ((uint8_t*)d)[ 1] = ((uint8_t*)s)[ 1];
//    ((uint8_t*)d)[ 2] = ((uint8_t*)s)[ 2];
//    ((uint8_t*)d)[ 3] = ((uint8_t*)s)[ 3];
//    ((uint8_t*)d)[ 4] = ((uint8_t*)s)[ 4];
//    ((uint8_t*)d)[ 5] = ((uint8_t*)s)[ 5];
//    ((uint8_t*)d)[ 6] = ((uint8_t*)s)[ 6];
//    ((uint8_t*)d)[ 7] = ((uint8_t*)s)[ 7];
//    ((uint8_t*)d)[ 8] = ((uint8_t*)s)[ 8];
//    ((uint8_t*)d)[ 9] = ((uint8_t*)s)[ 9];
//    ((uint8_t*)d)[10] = ((uint8_t*)s)[10];
//    ((uint8_t*)d)[11] = ((uint8_t*)s)[11];
//    ((uint8_t*)d)[12] = ((uint8_t*)s)[12];
//    ((uint8_t*)d)[13] = ((uint8_t*)s)[13];
//    ((uint8_t*)d)[14] = ((uint8_t*)s)[14];
//    ((uint8_t*)d)[15] = ((uint8_t*)s)[15];
//}

//static void copy_block_nn( uint8_t * d, const uint8_t *s, uint8_t nn )
//{
//    while( nn-- )
//        //*((uint8_t*)d)++ = *((uint8_t*)s)++;
//        *d++ = *s++;
//}

//static void xor_block( void *d, const void *s )
//{
//    ((uint8_t*)d)[ 0] ^= ((uint8_t*)s)[ 0];
//    ((uint8_t*)d)[ 1] ^= ((uint8_t*)s)[ 1];
//    ((uint8_t*)d)[ 2] ^= ((uint8_t*)s)[ 2];
//    ((uint8_t*)d)[ 3] ^= ((uint8_t*)s)[ 3];
//    ((uint8_t*)d)[ 4] ^= ((uint8_t*)s)[ 4];
//    ((uint8_t*)d)[ 5] ^= ((uint8_t*)s)[ 5];
//    ((uint8_t*)d)[ 6] ^= ((uint8_t*)s)[ 6];
//    ((uint8_t*)d)[ 7] ^= ((uint8_t*)s)[ 7];
//    ((uint8_t*)d)[ 8] ^= ((uint8_t*)s)[ 8];
//    ((uint8_t*)d)[ 9] ^= ((uint8_t*)s)[ 9];
//    ((uint8_t*)d)[10] ^= ((uint8_t*)s)[10];
//    ((uint8_t*)d)[11] ^= ((uint8_t*)s)[11];
//    ((uint8_t*)d)[12] ^= ((uint8_t*)s)[12];
//    ((uint8_t*)d)[13] ^= ((uint8_t*)s)[13];
//    ((uint8_t*)d)[14] ^= ((uint8_t*)s)[14];
//    ((uint8_t*)d)[15] ^= ((uint8_t*)s)[15];
//
//}

//static void copy_and_key( void *d, const void *s, const void *k )
//{
//    ((uint8_t*)d)[ 0] = ((uint8_t*)s)[ 0] ^ ((uint8_t*)k)[ 0];
//    ((uint8_t*)d)[ 1] = ((uint8_t*)s)[ 1] ^ ((uint8_t*)k)[ 1];
//    ((uint8_t*)d)[ 2] = ((uint8_t*)s)[ 2] ^ ((uint8_t*)k)[ 2];
//    ((uint8_t*)d)[ 3] = ((uint8_t*)s)[ 3] ^ ((uint8_t*)k)[ 3];
//    ((uint8_t*)d)[ 4] = ((uint8_t*)s)[ 4] ^ ((uint8_t*)k)[ 4];
//    ((uint8_t*)d)[ 5] = ((uint8_t*)s)[ 5] ^ ((uint8_t*)k)[ 5];
//    ((uint8_t*)d)[ 6] = ((uint8_t*)s)[ 6] ^ ((uint8_t*)k)[ 6];
//    ((uint8_t*)d)[ 7] = ((uint8_t*)s)[ 7] ^ ((uint8_t*)k)[ 7];
//    ((uint8_t*)d)[ 8] = ((uint8_t*)s)[ 8] ^ ((uint8_t*)k)[ 8];
//    ((uint8_t*)d)[ 9] = ((uint8_t*)s)[ 9] ^ ((uint8_t*)k)[ 9];
//    ((uint8_t*)d)[10] = ((uint8_t*)s)[10] ^ ((uint8_t*)k)[10];
//    ((uint8_t*)d)[11] = ((uint8_t*)s)[11] ^ ((uint8_t*)k)[11];
//    ((uint8_t*)d)[12] = ((uint8_t*)s)[12] ^ ((uint8_t*)k)[12];
//    ((uint8_t*)d)[13] = ((uint8_t*)s)[13] ^ ((uint8_t*)k)[13];
//    ((uint8_t*)d)[14] = ((uint8_t*)s)[14] ^ ((uint8_t*)k)[14];
//    ((uint8_t*)d)[15] = ((uint8_t*)s)[15] ^ ((uint8_t*)k)[15];
//}



/*********************************************************************
* Function: void EncKeySchedule(unsigned char* key)
*
* PreCondition:
*
* Input: key - array of the key data
*
* Output:
*
* Side Effects: values of the key changed
*
* Overview: None
*
* Note: None
********************************************************************/
void EncKeySchedule(unsigned char* key)
{
	/* column 1 */
	key[0]^=STable[key[13]];
	key[1]^=STable[key[14]];
	key[2]^=STable[key[15]];
	key[3]^=STable[key[12]];

	key[0]^=_rcon;
	_rcon = xtime(_rcon);

	/* column 2 */
	key[4]^=key[0];
	key[5]^=key[1];
	key[6]^=key[2];
	key[7]^=key[3];

	/* column 3 */
	key[8]^=key[4];
	key[9]^=key[5];
	key[10]^=key[6];
	key[11]^=key[7];

	/* column 4 */
	key[12]^=key[8];
	key[13]^=key[9];
	key[14]^=key[10];
	key[15]^=key[11];
}

/*********************************************************************
* Function: void EncodeShiftRow(unsigned char* stateTable)
*
* PreCondition:
*
* Input: stateTable - the data needing to be shifted
*
* Output:
*
* Side Effects: values of the inputed data changed
*
* Overview: None
*
* Note: None
********************************************************************/
void EncodeShiftRow(unsigned char* stateTable)
{
	unsigned char temp;

	/* first row (row 0) unchanged */

	/* second row (row 1) shifted left by one */
	temp=stateTable[1];
	stateTable[1]=stateTable[5];
	stateTable[5]=stateTable[9];
	stateTable[9]=stateTable[13];
	stateTable[13]=temp;

	/* third row (row 2) shifted left by two */
	temp=stateTable[2];
	stateTable[2]=stateTable[10];
	stateTable[10]=temp;
	temp=stateTable[14];
	stateTable[14]=stateTable[6];
	stateTable[6]=temp;

	/* fourth row (row 3) shifted left by three (or right by one) */
	temp=stateTable[3];
	stateTable[3]=stateTable[15];
	stateTable[15]=stateTable[11];
	stateTable[11]=stateTable[7];
	stateTable[7]=temp;
}



void AESEncode(unsigned char* block, unsigned char* masterKey)
{
	unsigned char i;
	//unsigned char temp;
        unsigned char key[16];

        for (i = 0; i < 16; i++)
        {
            key[i] = masterKey[i];
        }

	/* initiate round counter = 10 */
	_roundCounter = 10;

	_rcon = 0x01;

	/* key addition */
	for(i=0;i<BLOCKSIZE;i++)
	{
		block[i] ^= key[i];
	}

	do
	{
		/* s table substitution */
		for(i=0;i<BLOCKSIZE;i++)
		{
			block[i]=STable[block[i]];
		}

		/*   encode shift row */
		EncodeShiftRow(block);

		/*   if round counter != 1 */
		if(_roundCounter != 1)
		{
			unsigned char aux,aux1,aux2,aux3;
			/*     mix column */
			for(i=0;i<16;i+=4)
			{
				aux1= block[i+0] ^ block[i+1];
				aux3= block[i+2]^block[i+3];
				aux = aux1 ^ aux3;
				aux2= block[i+2]^block[i+1];

				aux1 = xtime(aux1);
				aux2 = xtime(aux2);
				aux3 = xtime(aux3);

				block[i+0]= aux^aux1^block[i+0];
				block[i+1]= aux^aux2^block[i+1];
				block[i+2]= aux^aux3^block[i+2];
				block[i+3]= block[i+0]^block[i+1]^block[i+2]^aux;
			}
		}
		/*   encode key schedule */
		EncKeySchedule(key);
		/*   key addition */
		for(i=0;i<BLOCKSIZE;i++)
		{
			block[i] ^= key[i];
		}

		_roundCounter--;
	}
	while(_roundCounter !=0);
}



