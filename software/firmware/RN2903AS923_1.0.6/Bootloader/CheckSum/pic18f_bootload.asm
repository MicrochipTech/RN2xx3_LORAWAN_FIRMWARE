; *****************************************************************************
;        Software License Agreement				    
;		 			 			    
; The software supplied herewith by Microchip Technology 	    
; Incorporated (the ?Company?) for its PICmicro? Microcontroller is 
; intended and supplied to you, the Company?s customer, for use     
; solely and exclusively on Microchip PICmicro Microcontroller	    
; products. The software is owned by the Company and/or its         
; supplier, and is protected under applicable copyright laws. All   
; rights are reserved. Any use in violation of the foregoing 	     
; restrictions may subject the user to criminal sanctions under	    
; applicable laws, as well as to civil liability for the breach of  
; the terms and conditions of this license.			    
;								    
; THIS SOFTWARE IS PROVIDED IN AN ?AS IS? CONDITION. NO WARRANTIES, 
; WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED 
; TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 	    
; PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT, 
; IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR 	    
; CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.		    
;		 						    
;
;
; Memory Map
;   -----------------
;   |    0x0000     |
;   |    0x0002     |	Boot vector
;   |               |
;   |               |
;   |               |
;   |  Boot Block   | 	(this program)
;   |               |   (PIC18Fxxx Devices)
;   |    0x0200     |		Re-mapped Reset Vector
;   |    0x0208     |		Re-mapped High Priority Interrupt Vector
;   |    0x0218     |		Re-mapped Low Priority Interrupt Vector
;   |               |
;   |               |
;   |  Code Space   |   User program space
;   |               |
;   |               |
;   |               |
;   |   0xXXXXXX    |
;   -----------------
;
;
; Incoming data format:
;
; [<Command><DataLengthL><DataLengthH> <EE_Key_1><EE_Key_2> <AddressL><AddressH><AddressU><unused><Data(optional)>]
;
; Definitions:
;
;   COMMAND    -   Base command
;   DataLength -   Length of data associated to the command
;   Address    -   Address up to 24 bits
;   Data       -   General data up to 255 bytes
;
;
; Commands:
;
;   Read Version  0x00    Read Version Information
;   Read Flash    0x01    Read Program Memory
;   Write Flash   0x02    Write Program Memory
;   Erase Flash   0x03    Erase Program Memory
;   Read EE Data  0x04    Read EEDATA Memory
;   Write EE Data 0x05    Write EEDATA Memory
;   Read Config   0x06    Read Config Memory (NOT supported by PIC16)
;   Write Config  0x07    Write Config Memory (NOT supported by PIC16)
;   Checksum      0x08    Calculate 16 bit checksum of specified region of memory
;   Reset         0x09    Reset Device and run application
;
; *****************************************************************************

#define  RD_VERSION  0x00
#define  RD_FLASH    0x01
#define  WR_FLASH    0x02
#define  ER_FLASH    0x03
#define  RD_EEDATA   0x04
#define  WR_EEDATA   0x05
#define  RD_CONFIG   0x06
#define  WR_CONFIG   0x07
; *****************************************************************************
    #include "p18lf46k22.inc"       ; Standard include
    #include "PIC18F_hardware_defs.inc"
; *****************************************************************************
    config FOSC = INTIO67   ;// Oscillator Selection bits (Internal oscillator block)
    config PLLCFG = OFF      ;// 4X PLL Enable (Oscillator multiplied by 4)
    config PRICLKEN = OFF    ;// Primary clock enable bit (Primary clock is always enabled)
    config FCMEN = OFF      ;// Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
    config IESO = OFF       ;// Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

;// CONFIG2L
    config PWRTEN = OFF     ;// Power-up Timer Enable bit (Power up timer disabled)
    config BOREN = NOSLP       ;// Brown-out Reset Enable bits (Brown-out Reset enabled and controlled by software (SBOREN is enabled))
    config BORV = 190       ;// Brown Out Reset Voltage bits (VBOR set to 2.20 V nominal)

;// CONFIG2H
    config WDTEN = OFF      ;// Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
    config WDTPS = 32768    ;// Watchdog Timer Postscale Select bits (1:32768)

;// CONFIG3H
    config CCP2MX = PORTC1  ;// CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
    config PBADEN = ON     ;// PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
    config CCP3MX = PORTB5  ;// P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
    config HFOFST = ON      ;// HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
    config T3CMX = PORTC0   ;// Timer3 Clock input mux bit (T3CKI is on RC0)
    config P2BMX = PORTD2   ;// ECCP2 B output mux bit (P2B is on RD2)
    config MCLRE = EXTMCLR  ;// MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

;// CONFIG4L
    config STVREN = ON      ;// Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
    config LVP = ON         ;// Single-Supply ICSP Enable bit (Single-Supply ICSP enabled if MCLRE is also 1)
    config XINST = OFF      ;// Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

;// CONFIG5L
    config CP0 = ON        ;// Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
    config CP1 = ON        ;// Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
    config CP2 = ON        ;// Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
    config CP3 = ON        ;// Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

;// CONFIG5H
    config CPB = ON        ;// Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
    config CPD = ON        ;// Data EEPROM Code Protection bit (Data EEPROM not code-protected)

;// CONFIG6L
    config WRT0 = OFF       ;// Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
    config WRT1 = OFF       ;// Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
    config WRT2 = OFF       ;// Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
    config WRT3 = OFF       ;// Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

;// CONFIG6H
    config WRTC = OFF       ;// Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
    config WRTB = OFF       ;// Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
    config WRTD = OFF       ;// Data EEPROM Write Protection bit (Data EEPROM not write-protected)

;// CONFIG7L
    config EBTR0 = OFF      ;// Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
    config EBTR1 = OFF      ;// Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
    config EBTR2 = OFF      ;// Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
    config EBTR3 = OFF      ;// Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

;// CONFIG7H
    config EBTRB = OFF      ;// Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

;        __idlocs _IDLOC0, 0x01
;        __idlocs _IDLOC1, 0x02
;        __idlocs _IDLOC2, 0x03
;        __idlocs _IDLOC3, 0x04
;        __idlocs _IDLOC4, 0x05
;        __idlocs _IDLOC5, 0x06
;        __idlocs _IDLOC6, 0x07
;        __idlocs _IDLOC7, 0x08

;#define  RCSTA  RCSTA1
;#define  TXSTA  TXSTA1
;#define  RCREG  RCREG1
;#define  TXREG  TXREG1
;#define  SPBRG  SPBRG1

; *****************************************************************************

#define	MAJOR_VERSION	0x01
#define	MINOR_VERSION	0x01		; Version


#define	STX		0x55

 variable  ERASE_FLASH_BLOCKSIZE  =    0x40    ; erase clears 0x40 words at a time
 variable  WRITE_FLASH_BLOCKSIZE  =    0x40    ; 0x40 write latches.
 variable  LAST_WORD_MASK = WRITE_FLASH_BLOCKSIZE -1

;#define	TBLWT_BUG			; Timing bug found in some PIC18Fxx20s
; *****************************************************************************



; *****************************************************************************
    cblock 0x00
CheckSum_L                  ; Checksum accumulator
CheckSum_H
COUNTER                 ; General counter
ABTIME_L
ABTIME_H
RXDATA
TXDATA
    endc

; Frame Format
;
; [<Command><DataLengthL><DataLengthH> <EE_Key_1><EE_Key_2> <AddressL><AddressH><AddressU><unused><Data(optional)>]

DATA_BUFF	equ	0x09		; Start of receive buffer
	
    cblock 0x09
COMMAND				; Data mapped in receive buffer
DATA_COUNT_L
DATA_COUNT_H
EE_KEY_1
EE_KEY_2
ADDRESS_L
ADDRESS_H
ADDRESS_U
ADDRESS_X
PACKET_DATA:0x40
    endc

FSR_LOG equ 0x80
#define  LOG_W   movwf   POSTINC1
	
; *****************************************************************************




; *****************************************************************************
pmwtpi macro				; tblwt*+ macro for PIC18Fxx20 bug
 IFDEF TBLWT_BUG
	tblwt	*
	tblrd	*+
 ELSE
	tblwt	*+
 ENDIF
	endm
; *****************************************************************************


 
; *****************************************************************************
	ORG	0x0000			; Re-map Reset vector
	bra	Setup	


	ORG	0x0008
VIntH
	bra	RVIntH			; Re-map Interrupt vector

	ORG	0x0018
VIntL
	bra	RVIntL			; Re-map Interrupt vector
; *****************************************************************************

 

; *****************************************************************************
; Setup the appropriate registers.
Setup
    bcf     TRIS_BOOTLOADER_INDICATOR
    bsf     BOOTLOADER_INDICATOR
    movlw   OSCCON_VALUE     ; clock as specified by config bits.
    movwf   OSCCON
    ;bsf     OSCTUNE, PLLEN      ; if you get a syntax error on PLLEN, try SPLLEN instead.


    clrf    TBLPTRU ; Load TBLPTR with the base
    movlw   high RVReset
    movwf   TBLPTRH ; address of the word
    movlw   low RVReset
    movwf   TBLPTRL
    movlw   high (0x10000 - 0x300 - 2)
    movwf   DATA_COUNT_H
    movlw   low  (0x10000 - 0x300 - 2)
    movwf   DATA_COUNT_L

    call    CalcCheckSum
    movlw   low (0x10000 - 2)
    movwf   TBLPTRL
    movlw   high (0x10000 - 2)
    movwf   TBLPTRH
    tblrd   *+			; Read low order stored checksum byte
    movf    TABLAT,w
    xorwf   CheckSum_L,w
    bnz     CheckSumFail
    tblrd   *+			; Read high order stored checksum byte
    movf    TABLAT,w
    xorwf   CheckSum_H,w
    bz      StartApp            ; if both checksums match, launch the application

CheckSumFail                    ; if the checksum fails, run the bootloader
; *****************************************************************************

    rcall   InitComms


#define  UART_RCIF   PIR1,RC1IF
#define  UART_TXIF   PIR1,TX1IF

; *****************************************************************************
AutoBaud:
;
; ___     ___     ___     ___     ___     __________
;    \_S_/ 1 \_0_/ 1 \_0_/ 1 \_0_/ 1 \_0_/ Stop
;       |                                |
;       |-------------- p ---------------|
;
;    p = The number of instructions between the first and last
;        rising edge of the RS232 control sequence 0x55. This is
;        the code that the hardware autobaud looks for.
;
;    SPBRG = (p / 32) - 1    BRGH = 1
;                                  B0/B1/B2

;	bcf     RCSTA1, CREN		; Stop receiving
;
;    movlw   b'00000011'		; x16 Prescale
;    movwf   T0CON
;    clrf    TMR0H			; Reset timer
;    clrf    TMR0L
;
;    rcall   WaitForRise
;
;    bsf     T0CON, TMR0ON		; Start counting
;
;    rcall   WaitForRise
;
;    bcf     T0CON, TMR0ON		; Stop counting
;
;    movff   TMR0L, ABTIME_L		; Read the timer
;
;DivB32
;    rrcf    TMR0H, F		; divide by 2
;    rrcf    ABTIME_L, F
;    btfss   STATUS, C		; Rounding
;    decf    ABTIME_L, F
;
;    movff   ABTIME_L, SPBRG1	; Sync
;
;    bsf     RCSTA1, CREN		; Start receiving
    clrwdt
    btfss   TXSTA,TRMT		; wait for rest of data to send
    bra     AutoBaud            ; without this the autobaud kills that last character in the buffer

    bsf     BAUDCON1,ABDEN
WaitForAutoBaud
    btfss   BAUDCON1,ABDEN
    bra     AutoBaudComplete
    btfss    BAUDCON1,ABDOVF
    bra     WaitForAutoBaud
    bcf     BAUDCON1,ABDEN
    bcf     BAUDCON1,ABDOVF
    bra     AutoBaud

AutoBaudComplete
    movf    RCREG1, W		; Empty the buffer
    movf    RCREG1, W
    ;decf    SPBRG1,f

; *****************************************************************************


; *****************************************************************************
; Read and parse the data.
StartOfLine

    lfsr    0, DATA_BUFF		; Point to the buffer
    movlw   9
    movwf   COUNTER			; Reset buffer count

GetNextDat
    rcall   RdRS232			; Get the data
    movf    RXDATA, W
    movwf   POSTINC0		; Store the data

    movf    FSR0L,w         ;
    xorlw   COMMAND+2       ; have we received the command and data length?
    bnz     EndGetNextLoop

    movf    COMMAND,w
    xorlw   WR_FLASH        ; is it a Write Flash command?
    bz      AddLength

    movf    COMMAND,w
    xorlw   WR_EEDATA
    bz      AddLength

    movf    COMMAND,w
    xorlw   WR_CONFIG
    bnz     EndGetNextLoop

AddLength
    movf    DATA_COUNT_L,w
    addwf   COUNTER,f

EndGetNextLoop
    decfsz  COUNTER, F		; Limit buffer to 256 bytes
    bra     GetNextDat

; ***********************************************

    rcall    ProcessBootBuffer              ; B3



; ***********************************************
; Send the data buffer back.
;
; <STX>[<DATA>...]

WritePacket
    movwf   COUNTER

    movlw   STX			; Send start condition
    rcall   WrRS232

    lfsr    FSR0, DATA_BUFF		; Setup pointer to buffer area

SendNext				; Send DATA
    movf    POSTINC0, W
    rcall   WrRS232
    decfsz  COUNTER, F
    bra     SendNext

    bra     AutoBaud
; *****************************************************************************




; *****************************************************************************
; Write a byte to the serial port.

WrRS232
    clrwdt

    btfss   UART_TXIF		; Write only if TXREG is ready
    bra     WrRS232

    movwf   TXREG1			; Start sending

    return
; *****************************************************************************




; *****************************************************************************
RdRS232
	clrwdt

	btfsc	RCSTA1, OERR		; Reset on overun
	reset
ReadWait
	btfss	UART_RCIF		; Wait for data from RS232
	bra	ReadWait

	movf	RCREG1, W		; Save the data

	movwf	RXDATA

	return
; *****************************************************************************


; *****************************************************************************
InitComms:                  ; configure the UART for communication
    CONFIGURE_IO

    movlw   b'10010000'		; Setup rx and tx
    movwf   RCSTA1
    movlw   b'00100110'
    movwf   TXSTA1
    movlw   0x08
    movwf   BAUDCON1
    return

; *****************************************************************************
;	ORG	0x000A

;WaitForRise
;	btfsc	UART_RX		; Wait for a falling edge
;	bra	WaitForRise
;	clrwdt
;WtSR	btfss	UART_RX		; Wait for starting edge
;	bra	WtSR
;	return
; *****************************************************************************


StartApp
    bcf     BOOTLOADER_INDICATOR
    bra     RVReset




ProcessBootBuffer
; ***********************************************
; Pre-setup, common to all commands.
PreSetupRegisters
    movf    ADDRESS_L, W		; Set all possible pointers
    movwf   TBLPTRL
    movwf   EEADR
    movf    ADDRESS_H, W
    movwf   TBLPTRH
;    movwf   EEADRH
    movff   ADDRESS_U, TBLPTRU

    lfsr    FSR0, PACKET_DATA

    movf    DATA_COUNT_L, W 		; Setup counter
    movwf   COUNTER
;    movff   EE_KEY1,EE_KEY_1
;    movff   EE_KEY2,EE_KEY_2
; ***********************************************

 
; ***********************************************
; Test the command field and sub-command.
CheckCommand
    movf    COMMAND, W		; Test for a valid command
    sublw   .9              ; Only commands 0-9 are valid
    bc      CommandValid
CommandError
    movlw   0xFF            ; return 0xFF to signal bad command
    movwf   COMMAND
    retlw   0x01

CommandValid
;    clrf    PCLATH			; Setup for a calculated jump
    clrf    PCLATU

    movlw   high JumpTableBaseAddress
    movwf   PCLATH

    rlncf   COMMAND, W		; Jump
    addlw   low JumpTableBaseAddress
    btfsc   STATUS,C
    incf    PCLATH
    movwf   PCL

JumpTableBaseAddress
    bra     ReadVersion     ; 0

    bra     ReadProgMem     ; 1
    bra     WriteProgMem    ; 2
    bra     EraseProgMem    ; 3
    bra     ReadEE          ; 4
    bra     WriteEE         ; 5
    bra     ReadProgMem     ; 6 ReadConfig
    bra     WriteConfig     ; 7
    bra     CalcCheckSum    ; 8
    reset   ;bra     ResetDevice     ; 9
; ***********************************************



; ***********************************************
; Commands
; 
; In:	[<0x01><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x01><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused><DATA>...]
; Data returned:<VerL><VerH><MaxPacketSizeL><MaxPacketSizeH><AckPacketSizeL><AckPacketSizeH>..
;            ...<DeviceIDL><DeviceIDH><0x00><0x00><EraseRowSize><WriteLatches>...
;            ...<ID1><ID2><ID3><ID4><
ReadVersion
    movlw   MINOR_VERSION
    movwf   PACKET_DATA
    movlw   MAJOR_VERSION
    movwf   PACKET_DATA + .1
    clrf    PACKET_DATA + .2   ; Max packet size (not used)
    clrf    PACKET_DATA + .3
    clrf    PACKET_DATA + .4   ; ack packet size (not used)
    clrf    PACKET_DATA + .5
    movlw   0x3f            ; point to Device ID (0x3F FF FE)
    movwf   TBLPTRU
    movlw   0xff            ; point to Device ID (0x3F FF FE)
    movwf   TBLPTRH
    movlw   0xfe            ; point to Device ID (0x3F FF FE)
    movwf   TBLPTRL
    tblrd   *+			; Fill buffer
    movff   TABLAT, PACKET_DATA + .6
    tblrd   *+			; Fill buffer
    movff   TABLAT, PACKET_DATA + .7
    clrf    PACKET_DATA + .8   ; Pad to 4 bytes
    clrf    PACKET_DATA + .9

    movlw   ERASE_FLASH_BLOCKSIZE
    movwf   PACKET_DATA + .10
    movlw   WRITE_FLASH_BLOCKSIZE
    movwf   PACKET_DATA + .11

    movlw   0x20            ; point to User ID (0x200,000)
    movwf   TBLPTRU
    clrf    TBLPTRH
    clrf    TBLPTRL
    tblrd   *+			; Fill buffer
    movff   TABLAT, PACKET_DATA + .12
    tblrd   *+			; Fill buffer
    swapf   PACKET_DATA + .12,w
    iorwf   TABLAT,w
    movwf   PACKET_DATA + .12

    tblrd   *+			; Fill buffer
    movff   TABLAT, PACKET_DATA + .13
    tblrd   *+			; Fill buffer
    swapf   PACKET_DATA + .13,w
    iorwf   TABLAT,w
    movwf   PACKET_DATA + .13

    tblrd   *+			; Fill buffer
    movff   TABLAT, PACKET_DATA + .14
    tblrd   *+			; Fill buffer
    swapf   PACKET_DATA + .14,w
    iorwf   TABLAT,w
    movwf   PACKET_DATA + .14
    tblrd   *+			; Fill buffer
    movff   TABLAT, PACKET_DATA + .15
    tblrd   *+			; Fill buffer
    swapf   PACKET_DATA + .15,w
    iorwf   TABLAT,w
    movwf   PACKET_DATA + .15

    movlw   .25  ; 9 byte header + 16 data bytes
    return


; **************************************************************************************
; ReadProgMem
;
; In:	[<0x01><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x01><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused><DATA>...]
;
ReadProgMem
    tblrd   *+			; Fill buffer
    movff   TABLAT, POSTINC0

    decfsz  COUNTER, F
    bra     ReadProgMem		; Not finished then repeat

    movf    DATA_COUNT_L, W		; Setup packet length
    addlw   0x09
				
    return

; **************************************************************************************
; WriteProgMem
;
; In:	[<0x02><DataLengthL><DataLengthH> <0x55><0xAA> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x02><DataLengthL><DataLengthH> <0x00><0x00> <ADDRL><ADDRH><ADDRU><unused><ACK>]
; Data Length is the length of bytes (2x the number of words)
WriteProgMem
 

Lp1
	movff   POSTINC0, TABLAT	; Load the holding registers

    pmwtpi				; Same as tblwt *+

    dcfsnz  COUNTER,f
    bra     StartFlashWrite

    movf    TBLPTRL,w
    andlw   WRITE_FLASH_BLOCKSIZE-1
    bnz     Lp1


StartFlashWrite
    tblrd   *-			; Point back into the block

    movlw   b'10000100'		; Setup writes
    movwf   EECON1
    rcall   StartWrite		; Write the data

    tblrd   *+			; Point to the beginning of the next block

    movf    COUNTER, f
    bnz     WriteProgMem

    bra     SendAcknowledge		; Send acknowledge

; **************************************************************************************
; EraseProgMem
;
; In:	[<0x03><EraseRowsL><EraseRowsH> <0x55><0xAA> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x03><EraseRowsL><EraseRowsH> <0x55><0xAA> <ADDRL><ADDRH><ADDRU><unused><ACK>]
EraseProgMem
	movlw	b'10010100'		; Setup writes
	movwf	EECON1

	rcall	StartWrite		; Erase the row

	movlw	ERASE_FLASH_BLOCKSIZE			; Point to next row
	addwf	TBLPTRL, F
	clrf	WREG
	addwfc	TBLPTRH, F
	addwfc	TBLPTRU, F

	decfsz	COUNTER, F
	bra	EraseProgMem	

	bra	SendAcknowledge		; Send acknowledge

; **************************************************************************************
; ReadEE
;
; In:	[<0x04><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x04><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused><DATA>...]
;
ReadEE
    clrf	EECON1

    bsf	EECON1, RD		; Read the data
    movff	EEDATA, POSTINC0
;    infsnz	EEADR, F		; Adjust EEDATA pointer
;    incf   EEADRH, F
    incf    EEADR, F

	decfsz	COUNTER, F
	bra	ReadEE			; Not finished then repeat

	movf	DATA_COUNT_L, W		; Setup packet length
	addlw	0x09
				
	return


; **************************************************************************************
; WriteEE
;
; In:	[<0x04><DataLengthL><DataLengthH> <0x55><0xAA> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x04><DataLengthL><DataLengthH> <0x00><0x00> <ADDRL><ADDRH><ADDRU><unused><DATA>...]
;
WriteEE	
	movff	POSTINC0, EEDATA

	movlw	b'00000100'		; Setup for EEData
	movwf	EECON1
	rcall	StartWrite		
	btfsc	EECON1, WR		; Write and wait
	bra	$ - 2

    incf    EEADR,f
;	infsnz	EEADR, F		; Adjust EEDATA pointer
;	incf	EEADRH, F

	decfsz	COUNTER, F
	bra	WriteEE			; Not finished then repeat
	bra	SendAcknowledge		; Send acknowledge

; **************************************************************************************
; ReadConfigWords
;   Read config words is no different than read flash so that code supports this command as well.
; In:	[<0x06><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x06><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused><CONFIG1L><CONFIG1H><CONFIG2L><CONFIG2H>]
;ReadConfig
;	movlw	b'11000000'
;	movwf	EECON1
;
;Lp5
;    tblrd	*+
;	movff	TABLAT, POSTINC0
;
;	decfsz	COUNTER, F
;	bra     Lp5			; Not finished then repeat
;
;	movf    DATA_COUNT_L, W		; Setup packet length
;	addlw   0x09
;
;	return

; **************************************************************************************
; WriteConfigWords
;
; In:	[<0x07><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x07><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused><CONFIG word data>
;
WriteConfig
	movlw	b'11000100'
	movwf	EECON1

	movff	POSTINC0, TABLAT	; Write to config area
	tblwt	*

	rcall	StartWrite

	tblrd	*+

	decfsz	COUNTER, F
	bra	WriteConfig		; Not finished then repeat
	
	bra	SendAcknowledge		; Send acknowledge
;************************************************


; **************************************************************************************
; CalcChecksum
;
; In:	[<0x08><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused>...]
; OUT:	[<0x08><DataLengthL><DataLengthH> <unused><unused> <ADDRL><ADDRH><ADDRU><unused><ChecksumL><ChecksumH>]
;
CalcCheckSum
    clrf    CheckSum_L
    clrf    CheckSum_H
    bcf     DATA_COUNT_L,0  ; force count to be an even number
    movf    DATA_COUNT_L,f
    btfss   STATUS,Z
    incf    DATA_COUNT_H,f  ; bump high order byte

CheckSumLoop
    tblrd   *+			; Fill buffer
    movf    TABLAT,w
    addwf   CheckSum_L,f

    tblrd   *+			; Fill buffer
    movf    TABLAT,w
    addwfc  CheckSum_H,f

    movlw   2
    subwf   DATA_COUNT_L,f
    bnz     CheckSumLoop
    decfsz  DATA_COUNT_H,f

    bra     CheckSumLoop		; Not finished then repeat

    movff   CheckSum_L,POSTINC0
    movff   CheckSum_H,POSTINC0
    movlw   .11
    return



SendAcknowledge
    movlw   0x01
    movwf   PACKET_DATA
	movlw	.10			; Send acknowledge
    return




; *****************************************************************************
; Unlock and start the write or erase sequence.

StartWrite
	clrwdt

	movf	EE_KEY_1,w			; Unlock
	movwf	EECON2
	movf    EE_KEY_2,w
	movwf	EECON2
	bsf	EECON1, WR		; Start the write
	nop

	return
; *****************************************************************************








; *****************************************************************************
	ORG	0x300
RVReset					

	ORG	0x308
RVIntH

	ORG	0x318
RVIntL
; *****************************************************************************


	END
