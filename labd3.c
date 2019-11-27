#include <msp430xG46x.h>

// aparatūras definīcijas
#define POGA1 0x01 // P1.0
#define POGA2 0x02 // P1.1

#define LED1 0x04 // P2.2
#define LED2 0x02 // P2.1

// konstantes
#define BMASK  POGA1 + POGA2   // Pogu maska
#define B_DEL_VAL 10    // Šo mēs mainīsim pēc algoritma izveides


void main(void)
{
// mainīgie
unsigned char Butt_S1 = POGA1 + POGA2;
unsigned char B_Del_Cnt = B_DEL_VAL;
unsigned char X;    // starprezultātu glabāšana

// aparatūras sākuma inicializācija
WDTCTL = WDTPW+WDTHOLD; // apturam WDT
P2DIR = LED1 + LED2; // pārslēdzam uz izeju
P2OUT |= LED1 + LED2; // ieslēdzam abas diodes
// programmas galvenais cikls
while(1)
{
X=P1IN & BMASK;
if(X == Butt_S1)
{
// pogas nav mainījušās
B_Del_Cnt--;
if(B_Del_Cnt == 0)
{
// kontaktu drebēšana ir beigusies
// Pogu apstrāde
if( (Butt_S1 & POGA1) == 0)
{
P2OUT |= LED1;
}
if( (Butt_S1 & POGA2) == 0)
{
P2OUT &= ~LED1;
}
B_Del_Cnt = B_DEL_VAL;
// atkārtojam apstrādi
}
}
else
{
// NEWSTAT
Butt_S1 = X;
// jaunā vērtība
B_Del_Cnt = B_DEL_VAL;
}
// NEXT
// te būs citi, programmas galvenajā ciklā veicamie darbi
};
}
