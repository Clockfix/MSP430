// programma labd2.c

#include <msp430x20x3.h>
volatile unsigned int cnt = 0;	//izveidojam globalo mainigo skititajam

void main (void)
{

WDTCTL = WDTPW+WDTHOLD;	// apturam WDT
P1DIR=0x01;				// P1.0 bus izeja

/////////////////// DCO parslegsana uz kalibretam vertibam.
//13.punkts - 1MHz; 17.punkts - 12 MHz frekvenci
DCOCTL = 0;
BCSCTL1 = CALBC1_12MHZ;
DCOCTL = CALDCO_12MHZ;
//////////////////////////////////////////

BCSCTL3 |= LFXT1S_2;    	// Signala avots VLOCLK 5.1. punkts

/////////////////// Cilpas cikls gaidisanai lai stabilizejas oscilators
for (;;)
{
IFG1 &= ~OFIFG; 			// Izdzesam Osc Fault partraukuma karodzinu
__delay_cycles(60); 		// Gaidam 60 ciklus
if ( (IFG1 & OFIFG) == 0 )	// Parbaudam karodzinus
break;						// izejam no cilpas cikla
}
//////////////////////////////////////////

BCSCTL2 |= SELM_3;      	// Izvelamies MCLK signala avotu, lai butu LFTX1 oscillator
//BCSCTL2 |= SELS;        	// Izvelamies MCLK signala avotu, lai butu LFXT1; Atslegts, lai pieslegt DCO -13.punkts

/////////////////// Taimer A konfiguresana
CCR0 = 62500;
//TACTL = TAIE + TACLR + TASSEL_1 + MC_1;     		// 8.punkts TaimerA partraukumu atlausana + nodzes TAR + ACLK + up_mode
TACTL = TAIE + TACLR + TASSEL_2 + MC_1 + ID_3;     // 14.punkts (ieprieksejo rindinu atsl.) TaimerA partraukumu atlausana + nodzes TAR + SMCLK + up_mode + ieejas dalitajs f/8
//////////////////////////////////////////

_enable_interrupts();		//Atlaut maskejamos partraukumus

/////////////////// Programmas galvenais cikls
// Lidz 12.punktam
// Pec 12.punkta iznemam darbibu ar LED'u
while(1)
    {
//    volatile unsigned int i;	//iekomentets pec 12.punkta
//    P1OUT^=0x01;        // invertējam izeju //iekomentets pec 12.punkta
//    i=500;              // sagatavojam aizturi //iekomentets pec 12.punkta
//    while(i)					//iekomentets pec 12.punkta
//        {						//iekomentets pec 12.punkta
//            i--;        // aizture //iekomentets pec 12.punkta
        }
//    }							//iekomentets pec 12.punkta
}
////////////////////////////////////////

///////////////////// Taimera partraukuma apstrades funkcija
//Nokopets no 9.punkta
#pragma vector = TIMERA1_VECTOR;
interrupt void myTimer(void)
    {
    if(TAIV==10)     // overflow
        // Taimera parpildisanas TAR == TACCR0
    {
        cnt++; 				//skaititaja vertibas unarais inkrements //Pielikts sakot ar 13.punktu
        if (cnt == 12)		//salidzinam vertibu //Pielikts sakot ar 13.punktu
           {				//Pielikts sakot ar 13.punktu
            P1OUT^=0x01; //parsledzam LED
            cnt = 0;		// nonullejam skatitaju //Pielikts sakot ar 13.punktu
           }				//Pielikts sakot ar 13.punktu
    }
}
////////////////////////////////////////


