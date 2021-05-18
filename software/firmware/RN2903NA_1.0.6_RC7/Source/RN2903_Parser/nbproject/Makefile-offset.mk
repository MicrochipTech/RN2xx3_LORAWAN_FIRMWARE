#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-offset.mk)" "nbproject/Makefile-local-offset.mk"
include nbproject/Makefile-local-offset.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=offset
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=mcc_generated_files/ext_int.c mcc_generated_files/LoRaWAN/sw_timer.c mcc_generated_files/mcc.c mcc_generated_files/LoRaWAN/radio_driver_SX1276.c mcc_generated_files/tmr1.c mcc_generated_files/pin_manager.c mcc_generated_files/LoRaWAN/lorawan_na.c mcc_generated_files/LoRaWAN/lorawan.c mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.c mcc_generated_files/interrupt_manager.c mcc_generated_files/LoRaWAN/AES.c mcc_generated_files/i2c1.c mcc_generated_files/memory.c mcc_generated_files/spi2.c mcc_generated_files/fvr.c mcc_generated_files/LoRaWAN/tmr_lora_addons.c mcc_generated_files/LoRaWAN/radio_driver_hal.c mcc_generated_files/LoRaWAN/lorawan_aes_cmac.c mcc_generated_files/LoRaWAN/lorawan_init.c mcc_generated_files/adc.c mcc_generated_files/LoRaWAN/lorawan_aes.c mcc_generated_files/eusart1.c parser/parser.c parser/parser_commands.c parser/parser_lorawan.c parser/parser_radio.c parser/parser_system.c parser/parser_tsp.c parser/parser_utils.c system/system.c system/system_low_power.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/mcc_generated_files/ext_int.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1 ${OBJECTDIR}/mcc_generated_files/tmr1.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1 ${OBJECTDIR}/mcc_generated_files/i2c1.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1 ${OBJECTDIR}/mcc_generated_files/spi2.p1 ${OBJECTDIR}/mcc_generated_files/fvr.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1 ${OBJECTDIR}/mcc_generated_files/adc.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1 ${OBJECTDIR}/mcc_generated_files/eusart1.p1 ${OBJECTDIR}/parser/parser.p1 ${OBJECTDIR}/parser/parser_commands.p1 ${OBJECTDIR}/parser/parser_lorawan.p1 ${OBJECTDIR}/parser/parser_radio.p1 ${OBJECTDIR}/parser/parser_system.p1 ${OBJECTDIR}/parser/parser_tsp.p1 ${OBJECTDIR}/parser/parser_utils.p1 ${OBJECTDIR}/system/system.p1 ${OBJECTDIR}/system/system_low_power.p1 ${OBJECTDIR}/main.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/mcc_generated_files/ext_int.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d ${OBJECTDIR}/mcc_generated_files/fvr.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d ${OBJECTDIR}/parser/parser.p1.d ${OBJECTDIR}/parser/parser_commands.p1.d ${OBJECTDIR}/parser/parser_lorawan.p1.d ${OBJECTDIR}/parser/parser_radio.p1.d ${OBJECTDIR}/parser/parser_system.p1.d ${OBJECTDIR}/parser/parser_tsp.p1.d ${OBJECTDIR}/parser/parser_utils.p1.d ${OBJECTDIR}/system/system.p1.d ${OBJECTDIR}/system/system_low_power.p1.d ${OBJECTDIR}/main.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/mcc_generated_files/ext_int.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1 ${OBJECTDIR}/mcc_generated_files/mcc.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1 ${OBJECTDIR}/mcc_generated_files/tmr1.p1 ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1 ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1 ${OBJECTDIR}/mcc_generated_files/i2c1.p1 ${OBJECTDIR}/mcc_generated_files/memory.p1 ${OBJECTDIR}/mcc_generated_files/spi2.p1 ${OBJECTDIR}/mcc_generated_files/fvr.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1 ${OBJECTDIR}/mcc_generated_files/adc.p1 ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1 ${OBJECTDIR}/mcc_generated_files/eusart1.p1 ${OBJECTDIR}/parser/parser.p1 ${OBJECTDIR}/parser/parser_commands.p1 ${OBJECTDIR}/parser/parser_lorawan.p1 ${OBJECTDIR}/parser/parser_radio.p1 ${OBJECTDIR}/parser/parser_system.p1 ${OBJECTDIR}/parser/parser_tsp.p1 ${OBJECTDIR}/parser/parser_utils.p1 ${OBJECTDIR}/system/system.p1 ${OBJECTDIR}/system/system_low_power.p1 ${OBJECTDIR}/main.p1

# Source Files
SOURCEFILES=mcc_generated_files/ext_int.c mcc_generated_files/LoRaWAN/sw_timer.c mcc_generated_files/mcc.c mcc_generated_files/LoRaWAN/radio_driver_SX1276.c mcc_generated_files/tmr1.c mcc_generated_files/pin_manager.c mcc_generated_files/LoRaWAN/lorawan_na.c mcc_generated_files/LoRaWAN/lorawan.c mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.c mcc_generated_files/interrupt_manager.c mcc_generated_files/LoRaWAN/AES.c mcc_generated_files/i2c1.c mcc_generated_files/memory.c mcc_generated_files/spi2.c mcc_generated_files/fvr.c mcc_generated_files/LoRaWAN/tmr_lora_addons.c mcc_generated_files/LoRaWAN/radio_driver_hal.c mcc_generated_files/LoRaWAN/lorawan_aes_cmac.c mcc_generated_files/LoRaWAN/lorawan_init.c mcc_generated_files/adc.c mcc_generated_files/LoRaWAN/lorawan_aes.c mcc_generated_files/eusart1.c parser/parser.c parser/parser_commands.c parser/parser_lorawan.c parser/parser_radio.c parser/parser_system.c parser/parser_tsp.c parser/parser_utils.c system/system.c system/system_low_power.c main.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

# The following macros may be used in the pre and post step lines
Device=PIC18LF46K22
ProjectDir="C:\_RN2xx3\RN2903NA_1.0.6_RC7\Source\RN2903_Parser"
ConfName=offset
ImagePath="dist\offset\${IMAGE_TYPE}\RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\offset\${IMAGE_TYPE}"
ImageName="RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IsDebug="true"
else
IsDebug="false"
endif

.build-conf:  .pre ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-offset.mk dist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18LF46K22
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/mcc_generated_files/ext_int.p1: mcc_generated_files/ext_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/ext_int.p1 mcc_generated_files/ext_int.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/ext_int.d ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1: mcc_generated_files/LoRaWAN/sw_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1 mcc_generated_files/LoRaWAN/sw_timer.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1: mcc_generated_files/LoRaWAN/radio_driver_SX1276.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1 mcc_generated_files/LoRaWAN/radio_driver_SX1276.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr1.p1: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr1.p1 mcc_generated_files/tmr1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1: mcc_generated_files/LoRaWAN/lorawan_na.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1 mcc_generated_files/LoRaWAN/lorawan_na.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1: mcc_generated_files/LoRaWAN/lorawan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1 mcc_generated_files/LoRaWAN/lorawan.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1: mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1 mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1: mcc_generated_files/LoRaWAN/AES.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1 mcc_generated_files/LoRaWAN/AES.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/i2c1.p1: mcc_generated_files/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/i2c1.p1 mcc_generated_files/i2c1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/i2c1.d ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi2.p1: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/spi2.p1 mcc_generated_files/spi2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi2.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/fvr.p1: mcc_generated_files/fvr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/fvr.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/fvr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/fvr.p1 mcc_generated_files/fvr.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/fvr.d ${OBJECTDIR}/mcc_generated_files/fvr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/fvr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1: mcc_generated_files/LoRaWAN/tmr_lora_addons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1 mcc_generated_files/LoRaWAN/tmr_lora_addons.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1: mcc_generated_files/LoRaWAN/radio_driver_hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1 mcc_generated_files/LoRaWAN/radio_driver_hal.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1: mcc_generated_files/LoRaWAN/lorawan_aes_cmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1 mcc_generated_files/LoRaWAN/lorawan_aes_cmac.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1: mcc_generated_files/LoRaWAN/lorawan_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1 mcc_generated_files/LoRaWAN/lorawan_init.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adc.p1: mcc_generated_files/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/adc.p1 mcc_generated_files/adc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adc.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1: mcc_generated_files/LoRaWAN/lorawan_aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1 mcc_generated_files/LoRaWAN/lorawan_aes.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart1.p1: mcc_generated_files/eusart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/eusart1.p1 mcc_generated_files/eusart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser.p1: parser/parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser.p1 parser/parser.c 
	@-${MV} ${OBJECTDIR}/parser/parser.d ${OBJECTDIR}/parser/parser.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_commands.p1: parser/parser_commands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_commands.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_commands.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_commands.p1 parser/parser_commands.c 
	@-${MV} ${OBJECTDIR}/parser/parser_commands.d ${OBJECTDIR}/parser/parser_commands.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_commands.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_lorawan.p1: parser/parser_lorawan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_lorawan.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_lorawan.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_lorawan.p1 parser/parser_lorawan.c 
	@-${MV} ${OBJECTDIR}/parser/parser_lorawan.d ${OBJECTDIR}/parser/parser_lorawan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_lorawan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_radio.p1: parser/parser_radio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_radio.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_radio.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_radio.p1 parser/parser_radio.c 
	@-${MV} ${OBJECTDIR}/parser/parser_radio.d ${OBJECTDIR}/parser/parser_radio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_radio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_system.p1: parser/parser_system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_system.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_system.p1 parser/parser_system.c 
	@-${MV} ${OBJECTDIR}/parser/parser_system.d ${OBJECTDIR}/parser/parser_system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_tsp.p1: parser/parser_tsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_tsp.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_tsp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_tsp.p1 parser/parser_tsp.c 
	@-${MV} ${OBJECTDIR}/parser/parser_tsp.d ${OBJECTDIR}/parser/parser_tsp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_tsp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_utils.p1: parser/parser_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_utils.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_utils.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_utils.p1 parser/parser_utils.c 
	@-${MV} ${OBJECTDIR}/parser/parser_utils.d ${OBJECTDIR}/parser/parser_utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/system/system.p1: system/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/system" 
	@${RM} ${OBJECTDIR}/system/system.p1.d 
	@${RM} ${OBJECTDIR}/system/system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/system/system.p1 system/system.c 
	@-${MV} ${OBJECTDIR}/system/system.d ${OBJECTDIR}/system/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/system/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/system/system_low_power.p1: system/system_low_power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/system" 
	@${RM} ${OBJECTDIR}/system/system_low_power.p1.d 
	@${RM} ${OBJECTDIR}/system/system_low_power.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/system/system_low_power.p1 system/system_low_power.c 
	@-${MV} ${OBJECTDIR}/system/system_low_power.d ${OBJECTDIR}/system/system_low_power.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/system/system_low_power.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1  --debugger=none  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/mcc_generated_files/ext_int.p1: mcc_generated_files/ext_int.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/ext_int.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/ext_int.p1 mcc_generated_files/ext_int.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/ext_int.d ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/ext_int.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1: mcc_generated_files/LoRaWAN/sw_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1 mcc_generated_files/LoRaWAN/sw_timer.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/sw_timer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/mcc.p1: mcc_generated_files/mcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/mcc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/mcc.p1 mcc_generated_files/mcc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/mcc.d ${OBJECTDIR}/mcc_generated_files/mcc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/mcc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1: mcc_generated_files/LoRaWAN/radio_driver_SX1276.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1 mcc_generated_files/LoRaWAN/radio_driver_SX1276.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_SX1276.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/tmr1.p1: mcc_generated_files/tmr1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/tmr1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/tmr1.p1 mcc_generated_files/tmr1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/tmr1.d ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/tmr1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/pin_manager.p1: mcc_generated_files/pin_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/pin_manager.p1 mcc_generated_files/pin_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/pin_manager.d ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/pin_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1: mcc_generated_files/LoRaWAN/lorawan_na.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1 mcc_generated_files/LoRaWAN/lorawan_na.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_na.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1: mcc_generated_files/LoRaWAN/lorawan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1 mcc_generated_files/LoRaWAN/lorawan.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1: mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1 mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/interrupt_manager_lora_addons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1: mcc_generated_files/interrupt_manager.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1 mcc_generated_files/interrupt_manager.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.d ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/interrupt_manager.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1: mcc_generated_files/LoRaWAN/AES.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1 mcc_generated_files/LoRaWAN/AES.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/AES.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/i2c1.p1: mcc_generated_files/i2c1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/i2c1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/i2c1.p1 mcc_generated_files/i2c1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/i2c1.d ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/i2c1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/memory.p1: mcc_generated_files/memory.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/memory.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/memory.p1 mcc_generated_files/memory.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/memory.d ${OBJECTDIR}/mcc_generated_files/memory.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/memory.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/spi2.p1: mcc_generated_files/spi2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/spi2.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/spi2.p1 mcc_generated_files/spi2.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/spi2.d ${OBJECTDIR}/mcc_generated_files/spi2.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/spi2.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/fvr.p1: mcc_generated_files/fvr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/fvr.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/fvr.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/fvr.p1 mcc_generated_files/fvr.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/fvr.d ${OBJECTDIR}/mcc_generated_files/fvr.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/fvr.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1: mcc_generated_files/LoRaWAN/tmr_lora_addons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1 mcc_generated_files/LoRaWAN/tmr_lora_addons.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/tmr_lora_addons.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1: mcc_generated_files/LoRaWAN/radio_driver_hal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1 mcc_generated_files/LoRaWAN/radio_driver_hal.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/radio_driver_hal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1: mcc_generated_files/LoRaWAN/lorawan_aes_cmac.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1 mcc_generated_files/LoRaWAN/lorawan_aes_cmac.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes_cmac.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1: mcc_generated_files/LoRaWAN/lorawan_init.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1 mcc_generated_files/LoRaWAN/lorawan_init.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_init.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/adc.p1: mcc_generated_files/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/adc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/adc.p1 mcc_generated_files/adc.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/adc.d ${OBJECTDIR}/mcc_generated_files/adc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/adc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1: mcc_generated_files/LoRaWAN/lorawan_aes.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files/LoRaWAN" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1 mcc_generated_files/LoRaWAN/lorawan_aes.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.d ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/LoRaWAN/lorawan_aes.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/mcc_generated_files/eusart1.p1: mcc_generated_files/eusart1.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/mcc_generated_files" 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${RM} ${OBJECTDIR}/mcc_generated_files/eusart1.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/mcc_generated_files/eusart1.p1 mcc_generated_files/eusart1.c 
	@-${MV} ${OBJECTDIR}/mcc_generated_files/eusart1.d ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/mcc_generated_files/eusart1.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser.p1: parser/parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser.p1 parser/parser.c 
	@-${MV} ${OBJECTDIR}/parser/parser.d ${OBJECTDIR}/parser/parser.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_commands.p1: parser/parser_commands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_commands.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_commands.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_commands.p1 parser/parser_commands.c 
	@-${MV} ${OBJECTDIR}/parser/parser_commands.d ${OBJECTDIR}/parser/parser_commands.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_commands.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_lorawan.p1: parser/parser_lorawan.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_lorawan.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_lorawan.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_lorawan.p1 parser/parser_lorawan.c 
	@-${MV} ${OBJECTDIR}/parser/parser_lorawan.d ${OBJECTDIR}/parser/parser_lorawan.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_lorawan.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_radio.p1: parser/parser_radio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_radio.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_radio.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_radio.p1 parser/parser_radio.c 
	@-${MV} ${OBJECTDIR}/parser/parser_radio.d ${OBJECTDIR}/parser/parser_radio.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_radio.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_system.p1: parser/parser_system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_system.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_system.p1 parser/parser_system.c 
	@-${MV} ${OBJECTDIR}/parser/parser_system.d ${OBJECTDIR}/parser/parser_system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_tsp.p1: parser/parser_tsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_tsp.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_tsp.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_tsp.p1 parser/parser_tsp.c 
	@-${MV} ${OBJECTDIR}/parser/parser_tsp.d ${OBJECTDIR}/parser/parser_tsp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_tsp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/parser/parser_utils.p1: parser/parser_utils.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/parser" 
	@${RM} ${OBJECTDIR}/parser/parser_utils.p1.d 
	@${RM} ${OBJECTDIR}/parser/parser_utils.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/parser/parser_utils.p1 parser/parser_utils.c 
	@-${MV} ${OBJECTDIR}/parser/parser_utils.d ${OBJECTDIR}/parser/parser_utils.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/parser/parser_utils.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/system/system.p1: system/system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/system" 
	@${RM} ${OBJECTDIR}/system/system.p1.d 
	@${RM} ${OBJECTDIR}/system/system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/system/system.p1 system/system.c 
	@-${MV} ${OBJECTDIR}/system/system.d ${OBJECTDIR}/system/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/system/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/system/system_low_power.p1: system/system_low_power.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/system" 
	@${RM} ${OBJECTDIR}/system/system_low_power.p1.d 
	@${RM} ${OBJECTDIR}/system/system_low_power.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/system/system_low_power.p1 system/system_low_power.c 
	@-${MV} ${OBJECTDIR}/system/system_low_power.d ${OBJECTDIR}/system/system_low_power.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/system/system_low_power.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist -DXPRJ_offset=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     -o${OBJECTDIR}/main.p1 main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.map  -D__DEBUG=1  --debugger=none  -DXPRJ_offset=$(CND_CONF)  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" --MSGDISABLE=1090:off;        $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.map  -DXPRJ_offset=$(CND_CONF)  --double=24 --float=24 --emi=wordwrite --rom=default,300-ffff --opt=+asm,-asmfile,-speed,+space,-debug,-local --addrqual=ignore --mode=pro -DDEBUGLINKADR=1 -P -N255 -I"system" -I"parser" -I"mcc_generated_files" -I"mcc_generated_files/LoRaWAN" --warn=0 --asmlist --summary=default,-psect,-class,+mem,-hex,-file --fill=0xffff@0x0300:0xffff --codeoffset=0300 --checksum=0300-FFFD@FFFE,width=-2,algorithm=2 --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=hybrid:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s" --MSGDISABLE=1090:off;     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/RN2903_Parser.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif

.pre:
	@echo "--------------------------------------"
	@echo "User defined pre-build step: [copy /b /Y .\parser\parser.c +,, .\parser\parser.c]"
	@copy /b /Y .\parser\parser.c +,, .\parser\parser.c
	@echo "--------------------------------------"

# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/offset
	${RM} -r dist/offset

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
