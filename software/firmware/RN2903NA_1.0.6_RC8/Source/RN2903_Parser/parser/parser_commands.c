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
 *                           parser_commands.c
 *
 * Command definition source file
 *
 *
 * Hardware:
 *  USB Carrier with RN2903 module
 *
 * Author            Date            Ver     Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * M16126          2015.01.12        0.5
 * M18029          2016.09.30
 * C15220          2018.02.08      1.0.3+Class-C-development
 ******************************************************************************/

#include <stdlib.h>
#include "parser_commands.h"

#include "parser_lorawan.h"
#include "parser_system.h"
#include "parser_radio.h"


static const parserCmdEntry_t maParserLoraSetChCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"status",      NULL,                       Parser_LoraSetChannelStatus,    0,                              2},
    {"drrange",     NULL,                       Parser_LoraSetDatarateRange,    0,                              3},

};
#define mParserLoraSetChCmdSize (sizeof(maParserLoraSetChCmd) / sizeof(maParserLoraSetChCmd[0]))

static const parserCmdEntry_t maParserLoraGetChCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"freq",        NULL,                       Parser_LoraGetChannelFreq,      0,                              1},
    {"status",      NULL,                       Parser_LoraGetChannelStatus,    0,                              1},
    {"drrange",     NULL,                       Parser_LoraGetDatarateRange,    0,                              1},
};
#define mParserLoraGetChCmdSize (sizeof(maParserLoraGetChCmd) / sizeof(maParserLoraGetChCmd[0]))

static const parserCmdEntry_t maParserLoraSetSubBandCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"status",      NULL,                       Parser_LoraSetSubBandStatus,    0,                              2},
};
#define mParserLoraSetSubBandCmdSize (sizeof(maParserLoraSetSubBandCmd) / sizeof(maParserLoraSetSubBandCmd[0]))

static const parserCmdEntry_t maParserLoraGetSubBandCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"status",      NULL,                       Parser_LoraGetSubBandStatus,    0,                              1},
};
#define mParserLoraGetSubBandCmdSize (sizeof(maParserLoraGetSubBandCmd) / sizeof(maParserLoraGetSubBandCmd[0]))

static const parserCmdEntry_t maParserLoraSetCmd[] =
{
  /* pCommand           pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------      --------------------------- ------------------------------- ------------------------------- -----   */
    {"dr",              NULL,                       Parser_LoraSetCrtDataRate,      0,                              1},
    {"adr",             NULL,                       Parser_LoraSetAdr,              0,                              1},
    {"devaddr",         NULL,                       Parser_LoraSetDevAddr,          0,                              1},
    {"deveui",          NULL,                       Parser_LoraSetDevEui,           0,                              1},
    {"appeui",          NULL,                       Parser_LoraSetAppEui,           0,                              1},
    {"nwkskey",         NULL,                       Parser_LoraSetNwkSKey,          0,                              1},
    {"appskey",         NULL,                       Parser_LoraSetAppSKey,          0,                              1},
    {"appkey",          NULL,                       Parser_LoraSetAppKey,           0,                              1},
    {"pwridx",          NULL,                       Parser_LoraSetTxPower,          0,                              1},
    {"ch",              maParserLoraSetChCmd,       NULL,                           mParserLoraSetChCmdSize,        0},
    {"rx2",             NULL,                       Parser_LoraSetRx2WindowParams,  0,                              2},
    {"bat",             NULL,                       Parser_LoraSetBatLevel,         0,                              1},
    {"retx",            NULL,                       Parser_LoraSetReTxNb,           0,                              1},
    {"ar",              NULL,                       Parser_LoraSetAutoReply,        0,                              1},
    {"rxdelay1",        NULL,                       Parser_LoraSetRxDelay1,         0,                              1},
    {"linkchk",         NULL,                       Parser_LoraLinkCheck,           0,                              1},
    {"sync",            NULL,                       Parser_LoraSetSyncWord,         0,                              1},
    {"upctr",           NULL,                       Parser_LoraSetUplinkCounter,    0,                              1},
    {"dnctr",           NULL,                       Parser_LoraSetDownlinkCounter,  0,                              1},
    {"class",           NULL,                       Parser_LoraSetClass,            0,                              1},
    {"mcast",           NULL,                       Parser_LoraSetMcast,            0,                              1},
    {"mcastdnctr",      NULL,                       Parser_LoraSetMcastDownCounter, 0,                              1},
    {"mcastdevaddr",    NULL,                       Parser_LoraSetMcastDevAddr,     0,                              1},
    {"mcastnwkskey",    NULL,                       Parser_LoraSetNwkMultiSKey,     0,                              1},
    {"mcastappskey",    NULL,                       Parser_LoraSetAppMultiSKey,     0,                              1},
    //#
    {"subband", maParserLoraSetSubBandCmd,  NULL,                           mParserLoraSetSubBandCmdSize,   0},
//    {"deveuiarray", NULL, Parser_LoraSetDevEuiArray, 0, 1},
//    {"appeuiarray", NULL, Parser_LoraSetAppEuiArray, 0, 1},
};
#define mParserLoraSetCmdSize (sizeof(maParserLoraSetCmd) / sizeof(maParserLoraSetCmd[0]))

static const parserCmdEntry_t maParserLoraGetCmd[] =
{
  /* pCommand           pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------      --------------------------- ------------------------------- ------------------------------- -----   */
    {"dr",              NULL,                       Parser_LoraGetCrtDataRate,      0,                              0},
    {"adr",             NULL,                       Parser_LoraGetAdr,              0,                              0},
    {"devaddr",         NULL,                       Parser_LoraGetDevAddr,          0,                              0},
    {"deveui",          NULL,                       Parser_LoraGetDevEui,           0,                              0},
    {"appeui",          NULL,                       Parser_LoraGetAppEui,           0,                              0},
    {"ch",              maParserLoraGetChCmd,       NULL,                           mParserLoraGetChCmdSize,        0},
    {"pwridx",          NULL,                       Parser_LoraGetTxPower,          0,                              0},
    {"rx2",             NULL,                       Parser_LoraGetRx2WindowParams,  0,                              0},
    {"dcycleps",        NULL,                       Parser_LoraGetPrescaler,        0,                              0},
    {"status",          NULL,                       Parser_LoraGetMacStatus,        0,                              0},
    {"retx",            NULL,                       Parser_LoraGetReTxNb,           0,                              0},
    {"mrgn",            NULL,                       Parser_LoraGetLinkCheckMargin,  0,                              0},
    {"gwnb",            NULL,                       Parser_LoraGetLinkCheckGwCnt,   0,                              0},
    {"ar",              NULL,                       Parser_LoraGetAutoReply,        0,                              0},
    {"rxdelay1",        NULL,                       Parser_LoraGetRxDelay1,         0,                              0},
    {"rxdelay2",        NULL,                       Parser_LoraGetRxDelay2,         0,                              0},
    {"sync",            NULL,                       Parser_LoraGetSyncWord,         0,                              0},
    {"upctr",           NULL,                       Parser_LoraGetUplinkCounter,    0,                              0},
    {"dnctr",           NULL,                       Parser_LoraGetDownlinkCounter,  0,                              0},
    {"class",           NULL,                       Parser_LoraGetClass,            0,                              0},
    {"mcast",           NULL,                       Parser_LoraGetMcast,            0,                              0},
    {"mcastdnctr",      NULL,                       Parser_LoraGetMcastDownCounter, 0,                              0},
    {"mcastdevaddr",    NULL,                       Parser_LoraGetMcastDevAddr,     0,                              0},
    //#
    {"subband", maParserLoraGetSubBandCmd, NULL, mParserLoraGetSubBandCmdSize, 0},
//    {"deveuiarray", NULL, Parser_LoraGetDevEuiArray, 0, 0},
//    {"appeuiarray", NULL, Parser_LoraGetAppEuiArray, 0, 0},
    
};
#define mParserLoraGetCmdSize (sizeof(maParserLoraGetCmd) / sizeof(maParserLoraGetCmd[0]))

const parserCmdEntry_t maParserLoraCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"reset",       NULL,                       Parser_LoraReset,               0,                              0},
    {"join",        NULL,                       Parser_LoraJoin,                0,                              1},
    {"tx",          NULL,                       Parser_LoraSend,                0,                              3},
    {"set",         maParserLoraSetCmd,         NULL,                           mParserLoraSetCmdSize,          0},
    {"get",         maParserLoraGetCmd,         NULL,                           mParserLoraGetCmdSize,          0},
    {"pause",       NULL,                       Parser_LoraPause,               0,                              0},
    {"resume",      NULL,                       Parser_LoraResume,              0,                              0},
    {"save",        NULL,                       Parser_LoraSave,                0,                              0},
    {"forceENABLE", NULL,                       Parser_LoraForceEnable,         0,                              0},
};

#define mParserLoraCmdSize  (sizeof(maParserLoraCmd) / sizeof(maParserLoraCmd[0]))

static const parserCmdEntry_t maParserSysSetCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"nvm",         NULL,                       Parser_SystemSetNvm,            0,                              2},
    {"pinmode",     NULL,                       Parser_SystemSetPinMode,        0,                              2},
    {"pindig",      NULL,                       Parser_SystemSetPinDig,         0,                              2},
    {"abdtime",     NULL,                       Parser_SystemSetABDTime,        0,                              1},
};
#define mParserSysSetCmdSize (sizeof(maParserSysSetCmd) / sizeof(maParserSysSetCmd[0]))

static const parserCmdEntry_t maParserSysGetCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"nvm",         NULL,                       Parser_SystemGetNvm,            0,                              1},
    {"ver",         NULL,                       Parser_SystemGetVer,            0,                              0},
    {"vdd",         NULL,                       Parser_SystemGetBattery,        0,                              0},
    {"hweui",       NULL,                       Parser_SystemGetHwEui,          0,                              0},
    {"pindig",      NULL,                       Parser_SystemGetPinDig,         0,                              1},
    {"pinana",      NULL,                       Parser_SystemGetPinAnalog,      0,                              1},
    {"abdtime",     NULL,                       Parser_SystemGetABDTime,        0,                              0},
};
#define mParserSysGetCmdSize (sizeof(maParserSysGetCmd) / sizeof(maParserSysGetCmd[0]))

const parserCmdEntry_t maParserSysCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"sleep",       NULL,                       Parser_SystemSleep,             0,                              1},
    {"reset",       NULL,                       Parser_SystemReboot,            0,                              0},
    {"eraseFW",     NULL,                       Parser_SystemBootload,          0,                              0},
    {"factoryRESET",NULL,                       Parser_SystemFactReset,         0,                              0},
    {"set",         maParserSysSetCmd,          NULL,                           mParserSysSetCmdSize,           0},
    {"get",         maParserSysGetCmd,          NULL,                           mParserSysGetCmdSize,           0},
};

#define mParserSysCmdSize  (sizeof(maParserSysCmd) / sizeof(maParserSysCmd[0]))


static const parserCmdEntry_t maParserRadioSetCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"freq",        NULL,                       Parser_RadioSetChannelFreq,     0,                              1},
    {"pwr",         NULL,                       Parser_RadioSetOutputPwr,       0,                              1},
    {"sf",          NULL,                       Parser_RadioSetSf,              0,                              1},
    {"mod",         NULL,                       Parser_RadioSetModulation,      0,                              1},
    {"crc",         NULL,                       Parser_RadioSetCrc,             0,                              1},
    {"bw",          NULL,                       Parser_RadioSetBw,              0,                              1},
    {"iqi",         NULL,                       Parser_RadioSetIqInverted,      0,                              1},
    {"prlen",       NULL,                       Parser_RadioSetPreambleLen,     0,                              1},
    {"fdev",        NULL,                       Parser_RadioSetFdev,            0,                              1},
    {"cr",          NULL,                       Parser_RadioSetCr,              0,                              1},
    {"bt",          NULL,                       Parser_RadioSetBt,              0,                              1},
    {"afcbw",       NULL,                       Parser_RadioSetAfcBw,           0,                              1},
    {"rxbw",        NULL,                       Parser_RadioSetRxBw,            0,                              1},
    {"wdt",         NULL,                       Parser_RadioSetWdt,             0,                              1},
    {"bitrate",     NULL,                       Parser_RadioSetBitrate,         0,                              1},
    {"sync",        NULL,                       Parser_RadioSetSyncWord,        0,                              1},
    {"reg",         NULL,                       Parser_RadioSetReg,             0,                              2},

};
#define mParserRadioSetCmdSize (sizeof(maParserRadioSetCmd) / sizeof(maParserRadioSetCmd[0]))

static const parserCmdEntry_t maParserRadioGetCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"freq",        NULL,                       Parser_RadioGetChannelFreq,     0,                              0},
    {"pwr",         NULL,                       Parser_RadioGetOutputPwr,       0,                              0},
    {"sf",          NULL,                       Parser_RadioGetSf,              0,                              0},
    {"mod",         NULL,                       Parser_RadioGetModulation,      0,                              0},
    {"crc",         NULL,                       Parser_RadioGetCrc,             0,                              0},
    {"bw",          NULL,                       Parser_RadioGetBw,              0,                              0},
    {"iqi",         NULL,                       Parser_RadioGetIqInverted,      0,                              0},
    {"prlen",       NULL,                       Parser_RadioGetPreambleLen,     0,                              0},
    {"fdev",        NULL,                       Parser_RadioGetFdev,            0,                              0},
    {"cr",          NULL,                       Parser_RadioGetCr,              0,                              0},
    {"bt",          NULL,                       Parser_RadioGetBt,              0,                              0},
    {"afcbw",       NULL,                       Parser_RadioGetAfcBw,           0,                              0},
    {"rxbw",        NULL,                       Parser_RadioGetRxBw,            0,                              0},
    {"wdt",         NULL,                       Parser_RadioGetWdt,             0,                              0},
    {"bitrate",     NULL,                       Parser_RadioGetBitrate,         0,                              0},
    {"sync",        NULL,                       Parser_RadioGetSyncWord,        0,                              0},
    {"rssi",        NULL,                       Parser_RadioGetRSSI,            0,                              0},
    {"snr",         NULL,                       Parser_RadioGetSnr,             0,                              0},
    {"reg",         NULL,                       Parser_RadioGetReg,             0,                              1},
};
#define mParserRadioGetCmdSize (sizeof(maParserRadioGetCmd) / sizeof(maParserRadioGetCmd[0]))

const parserCmdEntry_t maParserRadioCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"set",         maParserRadioSetCmd,        NULL,                           mParserRadioSetCmdSize,         0},
    {"get",         maParserRadioGetCmd,        NULL,                           mParserRadioGetCmdSize,         0},
    {"tx",          NULL,                       Parser_RadioTransmit,           0,                              1},
    {"rx",          NULL,                       Parser_RadioReceive,            0,                              1},
    {"cw",          NULL,                       Parser_RadioTransmitCW,         0,                              1},
    {"rxstop",      NULL,                       Parser_RadioStopRx,             0,                              0},
};

#define mParserRadioCmdSize  (sizeof(maParserRadioCmd) / sizeof(maParserRadioCmd[0]))


const parserCmdEntry_t maParserBaseCmd[] =
{
  /* pCommand       pNextParserCmd              pActionCbFct                    nextParserCmdSize               flags   */
  /* -------------  --------------------------- ------------------------------- ------------------------------- -----   */
    {"mac",         maParserLoraCmd,            NULL,                           mParserLoraCmdSize,             0},
    {"sys",         maParserSysCmd,             NULL,                           mParserSysCmdSize,              0},
    {"radio",       maParserRadioCmd,           NULL,                           mParserRadioCmdSize,            0},
#if defined(DEBUGLINKADR)
    {"debuglinkadr",NULL,                       Parser_DebugLinkADR,            0,                              1},
#endif // DEBUGLINKADR
};

#define mParserBaseCmdSize  (sizeof(maParserBaseCmd) / sizeof(maParserBaseCmd[0]))

const parserCmdEntry_t* gpParserStartCmd = &maParserBaseCmd[0];
const uint8_t gParserStartCmdSize = mParserBaseCmdSize;
