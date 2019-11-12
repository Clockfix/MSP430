// LABD1.C
//#include<MSP430x20x3.h>

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD;   // Stop watchdog timer
    P1DIR |= 0x01;              // Set P1.0 to output direction
    for(;;)
    {
        volatile unsigned int a;// Force compiler not optimize integer a
        P1OUT ^= 0x01;          // P1.0 = toggle
        a=55000;
        while(a>0)
        {
            a--;
        }
    }
}
