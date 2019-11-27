#include <msp430xG46x.h>
// šeit ir vieta segmentu definīcijām
#define sa 0x00
#define sb 0x00
#define sc 0x00
#define sd 0x00
#define se 0x00
#define sf 0x00
#define sg 0x00
#define sh 0x00
// zīmju ģeneratora tabulu, aizpildām atbilstoši norādītajiem zīmju
const char s_gen[] =
    {
        sa,      // 0
        sb,      // 1
        sc,      // 2
        sd,      // 3
        se,      // 4
        sf,      // 5
        sg,      // 6
        sh,      // 7
        sa + sb, // 8
        sb + sc, // 9
        sc + sd, // A
        sd + se, // b
        se + sf, // C
        sf + sg, // d
        sg + se, // E
        se + sc  // F
};
void main(void)
{
    unsigned int i;
    int j;
    int k;
    unsigned int offset = 2;
    const unsigned int delay = 60000;
    WDTCTL = WDTPW + WDTHOLD; // Apturam WDT
    // LCD_A
    LCDACTL = LCDFREQ_128 + LCD4MUX + LCDON;
    LCDAPCTL0 = 0x7E;
    P5SEL = 0x1C;

    for (j = 0; j < 11; j++)
    {
        LCDMEM[j + offset] = 0;
    }
    for (j = 0; j < 11; j++)
    {
        LCDMEM[j + offset] = 0xFF;
        for (i = delay; i > 0; i--)
            ;
    }
    for (j = 0; j < 12; j++)
    {
        LCDMEM[j + offset] = 0;
        for (i = delay; i > 0; i--)
            ;
    }
    j = 0;
    k = 0;
    while (1)
    {
        LCDMEM[k + offset] = s_gen[0xF & j];
        for (i = delay; i > 0; i--)
            ;
        j++;
        k++;
        if (k > 6)
            k = 0;
    }
}