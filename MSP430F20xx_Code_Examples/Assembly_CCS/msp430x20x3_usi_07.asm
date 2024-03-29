; --COPYRIGHT--,BSD_EX
;  Copyright (c) 2012, Texas Instruments Incorporated
;  All rights reserved.
; 
;  Redistribution and use in source and binary forms, with or without
;  modification, are permitted provided that the following conditions
;  are met:
; 
;  *  Redistributions of source code must retain the above copyright
;     notice, this list of conditions and the following disclaimer.
; 
;  *  Redistributions in binary form must reproduce the above copyright
;     notice, this list of conditions and the following disclaimer in the
;     documentation and/or other materials provided with the distribution.
; 
;  *  Neither the name of Texas Instruments Incorporated nor the names of
;     its contributors may be used to endorse or promote products derived
;     from this software without specific prior written permission.
; 
;  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
;  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
;  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
;  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
;  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
;  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
;  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
;  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
;  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
;  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
; 
; ******************************************************************************
;  
;                        MSP430 CODE EXAMPLE DISCLAIMER
; 
;  MSP430 code examples are self-contained low-level programs that typically
;  demonstrate a single peripheral function or device feature in a highly
;  concise manner. For this the code may rely on the device's power-on default
;  register values and settings such as the clock configuration and care must
;  be taken when combining code from several examples to avoid potential side
;  effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
;  for an API functional library-approach to peripheral configuration.
; 
; --/COPYRIGHT--
;******************************************************************************
;  MSP430F20xx Demo - I2C Master Transmitter, single byte
;
;  Description: I2C Master communicates with I2C Slave using
;  the USI. Master data is sent and increments from 0x00 with each transmitted
;  byte which is verified by the slave.
;  LED off for address or data Ack; LED on for address or data NAck.
;  ACLK = n/a, MCLK = SMCLK = Calibrated 1MHz
;
;  ***THIS IS THE MASTER CODE***
;
;                  Slave                      Master
;          (msp430x20x3_usi_08.asm)
;               MSP430F20x2/3              MSP430F20x2/3
;             -----------------          -----------------
;         /|\|              XIN|-    /|\|              XIN|-
;          | |                 |      | |                 |
;          --|RST          XOUT|-     --|RST          XOUT|-
;            |                 |        |                 |
;      LED <-|P1.0             |        |                 |
;            |                 |        |             P1.0|-> LED
;            |         SDA/P1.7|<-------|P1.7/SDA         |
;            |         SCL/P1.6|<-------|P1.6/SCL         |
;
;  Note: internal pull-ups are used in this example for SDA & SCL
;
;  P. Thanigai
;  Texas Instruments Inc.
;  May 2007
;  Built with Code Composer Essentials Version: 2.0
;******************************************************************************

I2CState   .equ     R4
MST_data   .equ     R5
slav_add   .equ     R6

 .cdecls C,LIST,  "msp430.h"
;-------------------------------------------------------------------------------
            .def    RESET                   ; Export program entry-point to
                                            ; make it known to linker.
;------------------------------------------------------------------------------
            .text                           ; Program Start
;------------------------------------------------------------------------------
RESET       mov.w   #0280h,SP               ; Initialize stackpointer
StopWDT     mov.w   #WDTPW+WDTHOLD,&WDTCTL  ; Stop watchdog timer
CheckCal    cmp.b   #0xFF,&CALBC1_1MHZ      ; Check calibration constants
            jne     Load                    ; if not erased, load.  
Trap        jmp     Trap                    ; if erased do not load, trap CPU!
Load        clr.b   &DCOCTL                 ; Select lowest DCOx and MODx settings
            mov.b   &CALBC1_1MHZ,&BCSCTL1   ; Set DCO to 1MHz
            mov.b   &CALDCO_1MHZ,&DCOCTL
SetupP1     mov.b   #0xC0,&P1OUT            ; P1.6 & P1.7 as pullups
            bis.b   #0xC0,&P1REN            ; P1.6 & P1.7 as pullups
            mov.b   #0xFF,&P1DIR            ; unused pins as output
SetupP2     mov.b   #0x00,&P2OUT                  
            mov.b   #0xFF,&P2DIR            
SetupUSI    mov.b   #USIPE6+USIPE7+USIMST+USISWRST,&USICTL0; Port, I2C master
            mov.b   #USIIE+USII2C,&USICTL1  ; Enable I2C
            mov.b   #USIDIV_3+USISSEL_2+USICKPL,&USICKCTL    ;  SMCLK/8
            bis.b   #USIIFGCC,&USICNT       ; Disable automatic clear control
            bic.b   #USISWRST,&USICTL0      ; Enable USI
            bic.b   #USIIFG,&USICTL1        ; Clear pending flag
            clr.w   I2CState
            clr.b   MST_data
            mov.b   #0x90,slav_add                                             
Mainloop    bis.b   #USIIFG,&USICTL1        
            bis.w   #LPM0+GIE,SR            ; Enter LPM0, enable interrupts
            call    #Delay
            jmp     Mainloop                         
;------------------------------------------------------------------------------
USI_ISR  ;
;------------------------------------------------------------------------------
            add.w   I2CState,PC             ; I2C State Machine
            jmp     STATE0
            jmp     STATE2
            jmp     STATE4
            jmp     STATE6
            jmp     STATE8
            jmp     STATE10
            
STATE0      
            bis.b   #0x01,&P1OUT            ; LED on: sequence start
            clr.b   &USISRL                 ; Generate start condition
            bis.b   #USIGE+USIOE,&USICTL0   ; 
            bic.b   #USIGE,&USICTL0         ;
            mov.b   slav_add,&USISRL        ; Transmit address, R/W =0
            mov.b   &USICNT,R8              ; Bit counter = 8, Tx address
            and.b   #0xE0,R8
            add.b   #0x08,R8
            mov.b   R8,&USICNT
            mov.w   #2,I2CState             ; Go to next state: Rx address(N)ACK
            bic.b   #USIIFG,&USICTL1
            reti
STATE2                                      ; Rx address             
            bic.b   #USIOE,&USICTL0         ; SDA = input
            bis.b   #0x01,&USICNT           ; Bit counter = 1, rx (N)ACK 
            mov.w   #4,I2CState             ; Go to next state, chk (N)ACK
            bic.b   #USIIFG,&USICTL1
            reti       
STATE4                                      ; Process address (N)Ack, data Tx
            bis.b   #USIOE,&USICTL0         ; SDA = output
            bit.b   #0x1,&USISRL            ; if NACK received
            jnc     Data_Tx                 ; ACK 
            clr.b   &USISRL
            bis.b   #0x01,&USICNT           ; bit counter = 1, SCL high,SDA low
            mov.w   #10,I2CState            ; goto next state, generate stop
            bis.b   #0x01,&P1OUT            ; Turn on LED : error
            bic.b   #USIIFG,&USICTL1
            reti
Data_Tx     
            mov.b   MST_data,&USISRL
            bis.b   #0x8,&USICNT            ; Bit counter = 8, Rx data
            mov.w   #6,I2CState             ; next state: Test data, (N)ACK
            bic.b   #0x1,&P1OUT             ; LED off
            bic.b   #USIIFG,&USICTL1 
            reti                      
STATE6                                      ; Receive Data Ack/Nack bit
            bic.b   #USIOE,&USICTL0         ; SDA = input
            bis.b   #0x01,&USICNT           ; Bit counter = 1, receive (N)Ack bit     
            mov.w   #8,I2CState             ; Go to next state: check (N)Ack
            bic.b   #USIIFG,&USICTL1 
            reti
STATE8                                      ; Process data (N)Ack bit
            bis.b   #USIOE,&USICTL0
            bit.b   #0x01,&USISRL           ; if NACK received
            jz      Data_Ack
            bis.b   #0x1,&P1OUT             ; LED on:error ; nack
            jmp     STATE8_Exit
Data_Ack
            inc.b   MST_data                ; Increment Master data
            bic.b   #0x1,&P1OUT             ; LED off
STATE8_Exit            
            clr.b   &USISRL       
            bis.b   #0x1,&USICNT            ; Bit counter = 1, SCL high,SDA low
            mov.w   #10,I2CState            ; Go to next state : generate stop
            bic.b   #USIIFG,&USICTL1 
            reti          
STATE10           
            mov.b   #0xFF,&USISRL           ; USISRL=1 to release SDA
            bis.b   #USIGE,&USICTL0         ; Transparent latch enabled
            bic.b   #USIGE+USIOE,&USICTL0   ; Latch/SDA output disabled
            clr.w   I2CState                ; Reset state machine for next Tx
            bic.w   #LPM0,0(SP)             ; Exit active for next transfer
            bic.b   #USIIFG,&USICTL1 
            reti          
;-------------------------------------------------------------------------------
Delay                                       ; Delay betn. communication cycles
;-------------------------------------------------------------------------------
            mov.w   #0xFFFF,R7
DL1         dec.w   R7
            jnz     DL1
            ret                              
;-------------------------------------------------------------------------------
;           Interrupt Vectors 
;-------------------------------------------------------------------------------
            .sect   ".reset"                ; MSP430 RESET Vector
            .short  RESET                   ;
            .sect   ".int04"                ; USI Vector
            .short  USI_ISR                 ;
            .end
