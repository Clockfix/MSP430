/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2012, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 * 
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430F20xx Demo - Timer_A, PWM TA1, Up Mode, 32kHz ACLK
//
//  Description: This program generates one PWM output on P1.2 using
//  Timer_A configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the value in CCR1 the PWM duty cycles. Using 32kHz ACLK
//  as TACLK, the timer period is 15.6ms with a 75% duty cycle on P1.2.
//  Normal operating mode is LPM3.
//  ACLK = TACLK = LFXT1 = 32768Hz, MCLK = SMCLK = default DCO.
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//	
//
//               MSP430F20xx
//            -----------------
//        /|\|              XIN|-
//         | |                 | 32kHz
//         --|RST          XOUT|-
//           |                 |
//           |         P1.2/TA1|--> CCR1 - 75% PWM
//
//  M.Buccini / L. Westlund
//  Texas Instruments, Inc
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************

#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x0C;                            // P1.2 and P1.3 output
  P1SEL |= 0x0C;                            // P1.2 and P1.3 TA1/2 options
  CCR0 = 512-1;                             // PWM Period
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 = 384;                               // CCR1 PWM duty cycle
  TACTL = TASSEL_1 + MC_1;                  // ACLK, up mode

  __bis_SR_register(LPM3_bits);             // Enter LPM3
}

