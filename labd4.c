#include <msp430xG46x.h>

// seit ir vieta segmentu definicijam ---no datasheet MSP430x4xx_slau056l.pdf 26-20 748.lpp
#define sa  0x01
#define sb  0x02
#define sc  0x04
#define sd  0x08
#define se  0x40
#define sf  0x10
#define sg  0x20
#define sh  0x80

#define LCD_OFFSET	2
#define DELAY		60000

//volatile unsigned int vieta =  0;     //definejam vietu displejaa 6.2.4.1.
//volatile unsigned int skaits = 2;     //definejam attelotu ciparu skaitu 6.2.4.2.

volatile unsigned int sec = 0;      // 6.1. punkts
volatile unsigned int min = 0;      // 8. punkts
volatile unsigned int hour = 0;     // 8. punkts

// zimju generatora tabulu, aizpildam atbilstosi noraditajiem skaitliem
const char s_gen[] =
{
sa+sb+sc+sd+se+sf,               // 0
sb+sc,                           // 1
sa+sb+sg+se+sd,                  // 2
sa+sb+sc+sd+sg,                  // 3
sb+sc+sf+sg,                     // 4
sa+sc+sd+sf+sg,                  // 5
sa+sc+sd+sf+sg+se,               // 6
sa+sb+sc,                        // 7
sa+sb+sc+sd+se+sf+sg,            // 8
sa+sb+sc+sd+sf+sg,               // 9
sa+sb+sc+se+sf+sg,               // A
sc+sd+se+sf+sg,                  // b
sa+sd+se+sf,                     // C
sb+sc+sd+se+sg,                  // d
sa+sd+se+sf+sg,                  // E
sa+se+sf+sg,                     // F
sh                               // . vai :
};

// function prototyps
void Delay(unsigned int);

/////-------Bin2Hex2LCD-------
//////////////////////// 6.2.punkts ----- JAAIEKOMENTEE sk. apaksaa 6.2.3.punkts un ---Bin2BCD2LCD---
//void Bin2Hex2LCD(unsigned int bin, unsigned char vieta, unsigned char skaits)
//{
//int i;
//for(i=0; i<skaits; i++)
//{
//LCDMEM[i+LCD_OFFSET+vieta]=s_gen[bin&0x0F];
//bin>>=4;
//}
//}
//////////////////////////////////////////////////////////////

/////-------Bin2BCD2LCD-------
//////////////////////// 7.punkts ----- JAAIEKOMENTEE visu ---Bin2Hex2LCD---
void Bin2BCD2LCD(unsigned int bin, unsigned char vieta, unsigned char skaits)
{
int i;
for(i=0; i<skaits; i++)
{
    if (i==0)
    {
LCDMEM[i+LCD_OFFSET+vieta]=s_gen[bin%10];
bin/=10;
    }
    //--- 10.punkta papildus kods kola attelosanai ->> hh:mm:ss -----------
    else
    {
        if(vieta>4)
        {
            LCDMEM[i+LCD_OFFSET+vieta]=s_gen[bin%10];
            bin/=10;
        }else{

            LCDMEM[i+LCD_OFFSET+vieta]=s_gen[bin%10] | s_gen[16];
            bin/=10;
        }
    }
    //--- 10.punkta beigas-------------------------------------------------
}
}
//////////////////////////////////////////////////////////////

void main(void)
{
volatile unsigned int i;
int j;
int k;

// initialization of hardware
WDTCTL = WDTPW + WDTHOLD;	// stop WDT

// LCD_A initialization
LCDACTL		=	LCDFREQ_128 + LCD4MUX +  LCDON;
LCDAPCTL0	=	0x7E;
P5SEL		=	0x1C;

// preparing for start
for ( j=0; j<11; j++)               //izdzesam visu no ekrana
	{
	LCDMEM[j+LCD_OFFSET]=0;
	}

for ( j=0; j<11; j++)               //iekrasojam visu ekranu
	{
	LCDMEM[j+LCD_OFFSET]=0xFF;
	Delay(DELAY);
	}

for ( j=0; j<12; j++)               //izdzesam visu (ko iekrasojam) no ekrana
	{
	LCDMEM[j+LCD_OFFSET]=0;
	Delay(DELAY);
	}
j=0;
k=0;

//programmas galvenais cikls
while(1)
	{
//    Bin2Hex2LCD(sec,0,4);                   //6.-6.2.5.punkti konvertejam sec= 12345 hex'aa sanaak - 3039
//    Bin2BCD2LCD(sec,0,4);                   //7.2. punkts, !!!--jaiekomentee iepriekseju rindinu--!!! vienkarsi attelojam sec= 12345

//	LCDMEM[k+LCD_OFFSET]=s_gen[0xF & j];    // 6.2.3 jaaiekomentee // sk. AUGHSAA 6.2.punkts
//	Delay(DELAY);                           // 6.2.3 jaaiekomentee
//	j++; k++;                               // 6.2.3 jaaiekomentee
//	if(k>6)k=0;                             // 6.2.3 jaaiekomentee

//	sec++;                                  // 6.3. punkts, vienkarsi pieskaitam klaat pa sekundei


	// Veidojam pulkstenu skaititaju 9.4. punkts
	                sec++;
	                    if (sec==60)
	                    {
	                        sec=0;
	                        min++;
	                    }
	                    if (min==60)
	                    {
	                        min=0;
	                        hour++;
	                    }
	                    if (hour==24)
	                    {
	                        hour=0;
	                    }

	                    Bin2BCD2LCD(sec,1,2);       // 9.1. punkts - vieta, kur aatelojam secundes
	                    Bin2BCD2LCD(min,3,2);       // 9.2. punkts - vieta, kur aatelojam minutes
	                    Bin2BCD2LCD(hour,5,2);      // 9.3. punkts - vieta, kur aatelojam stundas


Delay(DELAY);
Delay(DELAY);

	}       // while(1) - beigas
}

/* software delay */
void Delay(unsigned int del)
{
volatile unsigned int t=del;
                                    while(t)
	{
	t--;
	}
}

