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
 *                           parser.h
 *
 * Main parser file
 *
 *
 * Hardware:
 *  USB Carrier with RN2903 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029		   2016.09.30
 ******************************************************************************/

#ifndef _PARSER_H
#define _PARSER_H

#include <stdint.h>

#define PARSER_MAX_DATA_LEN 530U

typedef union
{
    uint32_t value;
    uint8_t buffer[4];
} auint32_t;


typedef union
{
    uint16_t value;
    uint8_t buffer[2];
}auint16_t;

typedef union
{
    uint16_t value;
    struct
    {
        unsigned int deveui         : 1;
        unsigned int appeui         : 1;
        unsigned int appkey         : 1;
        unsigned int devaddr        : 1;
        unsigned int nwkskey        : 1;
        unsigned int appskey        : 1;
        unsigned int devmultiaddr   : 1;
        unsigned int appmultiskey   : 1;
        unsigned int nwkmultiskey   : 1;
    } flags;
} parserConfiguredJoinParameters_t;


uint8_t aParserData[PARSER_MAX_DATA_LEN];

void Parser_Init(void);
void Parser_Main(void);

void Parser_GetSwVersion(uint8_t* pBuffData);

uint8_t Parser_GetConfiguredJoinParameters();
void Parser_SetConfiguredJoinParameters(uint8_t val);


#endif /* _PARSER_H */
