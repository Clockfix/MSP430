// LABD2.C
//
// From TI examples
//
//******************************************************************************
//  MSP430F20xx Demo - Software Toggle P1.0, MCLK = VLO/8
//
//  Description; Pulse P1.0 with a 1/100 active duty cycle using software.
//  Ultra-low frequency ~ 1.5kHz, ultra-low power active mode demonstrated.
//  ACLK = VL0, MCLK = VLO/8 ~1.5kHz, SMCLK = n/a
//
//                MSP430F20xx
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//            |             P1.0|-->LED
//
//  M.Buccini / L. Westlund
//  Texas Instruments, Inc
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************

//#include<MSP430x20x3.h>


//void main(void)
//{
//
// laboratorijas darba 5.punkts
//   WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
//    P1DIR |= 0x01;              // Set P1.0 to output direction
//
//    // from example msp430x20x3_1_vlo.c
//    BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
//    IFG1 &= ~OFIFG;                           // Clear OSCFault flag
//    __bis_SR_register(SCG1 + SCG0);           // Stop DCO
//    BCSCTL2 |= SELM_3;                        // MCLK = LFXT1
//
//   for(;;)
//    {
//        //volatile unsigned int a;// Force compiler not optimize integer a
//        P1OUT ^= 0x01;          // P1.0 = toggle
//    }
//}

//laboratorijas darba 8.punkts

// from example msp430x20x3_ta_04.c

//******************************************************************************
//  MSP430F20xx Demo - Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK
//
//  Description: Toggle P1.0 using software and the Timer_A overflow ISR.
//  In this example an ISR triggers when TA overflows. Inside the ISR P1.0
//  is toggled. Toggle rate is exactly 0.5Hz. Proper use of the TAIV interrupt
//  vector generator is demonstrated.
//  ACLK = TACLK = 32768Hz, MCLK = SMCLK = default DCO
//  //* An external watch crystal on XIN XOUT is required for ACLK *//
//
//           MSP430F20xx
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|-->LED
//
//  M. Buccini / L. Westlund
//  Texas Instruments Inc.
//  September 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************



//int main(void)
//{
//  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
//  P1DIR |= 0x01;                            // P1.0 output
//
//    P1SEL |= 0x01;                            // P1.0 option select
//    CCTL0 = OUTMOD_4;                         // CCR0 toggle mode  OUTMOD_4 ->  Izeju pārslēdz uz pretējo, kad TAR ir vienāds ar TACCRx
//    CCR0 = 10-1;
//
//    //  In this
//    //  example, CCR0 is loaded with 10-1 and TA0 will toggle P1.1 at TACLK/10.
//    //  Thus the output frequency on P1.1 will be the TACLK/20. No CPU or software
//    //  resources required. Normal operating mode is LPM3.
//
//    TACTL = TASSEL_1 + MC_1;                  // ACLK, up_mode
//
//
//}
//// Timer_A3 Interrupt Vector (TAIV) handler
//
//
//// Vai šo vajag ja augšā jau ir toggle?????????
////Taimera pārtraukuma apstrādes funkcija
//#pragma vector = TIMERA1_VECTOR;
//interrupt void myTimer(void)
//    {
//    if(TAIV==10)     // overflow
//        // Taimera pārpildīšanās TAR == TACCR0
//    {
//        P1OUT^=0x01; //pārslēdzam LED
//    }
//}


// Laboratorijas darba 13.punkts
// 13. Noskaņot DCO uz 1MHz frekvenci, izmantojot DCO kalibrētās vērtības [1,16.lpp].

// msp430x20xx_dco_flashcal.c       DCO Calibration Constants Programmer

//******************************************************************************
//  MSP430F20xx Demo - DCO Calibration Constants Programmer
//
//  NOTE: THIS CODE REPLACES THE TI FACTORY-PROGRAMMED DCO CALIBRATION
//  CONSTANTS LOCATED IN INFOA WITH NEW VALUES. USE ONLY IF THE ORIGINAL
//  CONSTANTS ACCIDENTALLY GOT CORRUPTED OR ERASED.
//
//  Description: This code re-programs the F2xx DCO calibration constants.
//  A software FLL mechanism is used to set the DCO based on an external
//  32kHz reference clock. After each calibration, the values from the
//  clock system are read out and stored in a temporary variable. The final
//  frequency the DCO is set to is 1MHz, and this frequency is also used
//  during Flash programming of the constants. The program end is indicated
//  by the blinking LED.
//  ACLK = LFXT1/8 = 32768/8, MCLK = SMCLK = target DCO
//  //* External watch crystal installed on XIN XOUT is required for ACLK *//
//
//           MSP430F20xx
//         ---------------
//     /|\|            XIN|-
//      | |               | 32kHz
//      --|RST        XOUT|-
//        |               |
//        |           P1.0|--> LED
//        |           P1.4|--> SMLCK = target DCO
//
//  A. Dannenberg
//  Texas Instruments Inc.
//  May 2007
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.42A
//******************************************************************************

//   msp430x20x3_ta_16.c

//******************************************************************************
//  MSP430F20xx Demo - Timer_A, PWM TA1-2, Up Mode, DCO SMCLK
//
//  Description: This program generates one PWM output on P1.2 using
//  Timer_A configured for up mode. The value in CCR0, 512-1, defines the PWM
//  period and the value in CCR1 the PWM duty cycles.
//  A 75% duty cycle on P1.2.
//  ACLK = na, SMCLK = MCLK = TACLK = default DCO
//
//               MSP430F20xx
//            -----------------
//        /|\|              XIN|-
//         | |                 |
//         --|RST          XOUT|-
//           |                 |
//           |         P1.2/TA1|--> CCR1 - 75% PWM
//
//  M.Buccini / L. Westlund
//  Texas Instruments, Inc
//  October 2005
//  Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 3.40A
//******************************************************************************

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P1DIR |= 0x01;                            // P1.0 0 output
  P1SEL |= 0x01;                            // P1.0 0 TA1/2 options

//  DCO kalibrēšanas vērtību
//  lietošana

  DCOCTL = 0;
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;

  CCR0 = 512-1;                             // PWM Period
  CCTL1 = OUTMOD_7;                         // CCR1 reset/set
  CCR1 = 384;                               // CCR1 PWM duty cycle
  TACTL = TASSEL_2 + MC_1;                  // SMCLK, up mode



  __bis_SR_register(CPUOFF);                // Enter LPM0
}


// Vai šo vajag ja augšā jau ir toggle?????????
//Taimera pārtraukuma apstrādes funkcija
#pragma vector = TIMERA1_VECTOR;
interrupt void myTimer(void)
    {
    if(TAIV==10)     // overflow
        // Taimera pārpildīšanās TAR == TACCR0
    {
        P1OUT^=0x01; //pārslēdzam LED
    }
}


/*
 MCLK pārslēgšana uz VLO,LFXT1, XT2
do {
 ... ... ...
    }
 while( IFG1 & OFIFG );
 */



/*
Pārtraukuma apstrādes procedūra

#pragma vector = TIMERA1_VECTOR
interrupt void myTimer(void)
    {
        ......
    }
*/

/*
 TAIFG pārbaude TAIV reģistrā

if(TAIV == 10) // ? TAIFG
    {
    // apstrādes procedūra
    ...
    }

    Vairāku pārtraukumu gadījumā lietosim switch, case, break konstrukciju
 */
