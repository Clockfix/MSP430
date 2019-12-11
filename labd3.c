// programma veic Kontaktu drebesanas noversanu
#include <msp430xG46x.h> 

// aparaturas definicijas
#define POGA1 0x01 // P1.0
#define POGA2 0x02 // P1.1
#define LED1 0x04  // P2.2
#define LED2 0x02  // P2.1

// konstantes
#define BMASK	POGA1+POGA2	// Pogu maska
#define B_DEL_VAL	437		// So mes mainisim pec algoritma izveides

void main(void)
{
// mainigie
unsigned char	Butt_S1=POGA1 + POGA2;
unsigned char	B_Del_Cnt=B_DEL_VAL;
unsigned char	X;				// starprezultatu glabasana
unsigned char   Butt_S2;        // 7.punkta mainigais pogas ieprieksejas stavokla saglabasanai

// aparaturas sakuma inicializacija
WDTCTL = WDTPW+WDTHOLD;	// apturam WDT
P2DIR = LED1 + LED2;	// parsledzam uz izeju
P2OUT |= LED1 + LED2;	// iesledzam abas diodes


// programmas galvenais cikls
while(1)
	{
	X=P1IN & BMASK;
	if(X == Butt_S1)
		{	// pogas nav mainijusas
		B_Del_Cnt--;
		if(B_Del_Cnt==0)
			{	// kontaktu drebseana ir beigusies
////////////////// 5.punkts - Pogu apstrades algoritms (visu iekomentejam, ja velamies izpildit 7.punktu)
//		    if( (Butt_S1 & POGA2) == 0)
//		        {
//		        P2OUT |= LED1;
//		        }
//		    if( (Butt_S1 & POGA1) == 0)
//		        {
//		        P2OUT &= ~LED1;
//		        }
//////////////////////////////////////// 5.punkta beigas

////////////////// 7.punkts - Pogu apstrades algoritms (visu iekomentejam, ja velamies izpildit 5.punktu)
		    if( (Butt_S1 & POGA1) == 0)
                {
		        if(Butt_S1 != Butt_S2)
		        {
                P2OUT ^= LED2;
                }
                }
		    Butt_S2 = Butt_S1;
//////////////////////////////////////// 7.punkta beigas
			B_Del_Cnt=B_DEL_VAL;		// atkartojam kontaktu drebsaanas apstradi
			}
		}
	else
		{	// NEWSTAT
		Butt_S1=X;	// jauna vertiba
		B_Del_Cnt=B_DEL_VAL;
		}

	// NEXT
	// te bus citi, programmas galvenaja cikla veicamie darbi
	};
}
