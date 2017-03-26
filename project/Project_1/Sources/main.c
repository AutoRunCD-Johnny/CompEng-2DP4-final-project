#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "SCI.h"


void setClk(void);
void delay(void);
void freq_check(void);
unsigned short val,i,buffer[420];
unsigned int flag=0;

/*void OutCRLF(void){
  SCI_OutChar(CR);
  SCI_OutChar(LF);
  PTJ ^= 0x20;          // toggle LED D2
}*/

void main(void) {
  /* put your own code here */
  setClk();  
	
	DDRJ=0x01;
	DDRP=0x01;
	
	ATDCTL1 = 0x2F;		// set for 10-bit resolution
	ATDCTL3 = 0x88;		// right justified, one sample per sequence
	ATDCTL4 = 0x00;		// prescaler = 0; ATD clock = 2MHz / (2 * (0 + 1)) == 1MHz
	ATDCTL5 = 0x24;		// continuous conversion on channel 4 (AN4)
    SCI_Init(9600);

	
	TSCR1=0x90;
	TIOS=0xFE;
	PERT=0x01;
	TCTL3=0x00;
	TCTL4=0x02;
	TIE=0x01;
	
	EnableInterrupts; //CodeWarrior's method of enabling interrupts

  for(;;) {
	if(flag){
		for(i=0;i<420;i++){
		  PTP^=0x01;
			buffer[i]=ATDDR0;
			delay();
		  }  
/*	PTJ=0x01;	
	val=ATDDR0;
	SCI_OutUDec(val);
	SCI_OutChar(CR);
	delay();  */
	  for(i=0;i<420;i++){
		  PTJ^=0x01;
  		SCI_OutUDec(buffer[i]);
	  	SCI_OutChar(CR);
	    }	
	  }    
	  
	  //freq_check();
  } /* loop forever */
}  /* please make sure that you never leave main */




interrupt  VectorNumber_Vtimch0 void ISR_Vtimch0(void){
	unsigned int temp;
	flag^=1;
	temp=TC0;  //fast clear flag
}    



#define initSYNR  0x04        //
#define VCOFRQ    0x00        //vco gain   (depend on Fvco range)
#define initREFDV 0x01        //
#define REFFRQ    0x10        //pll stability (depend on the Fref range) 

void setClk(void){//2MHz bus clock

  //CPMUPROT = 0;               //Protection of clock configuration is disabled
  CPMUCLKS = 0x80;   //pllssel=1
  CPMUOSC = 0x80;    // Enable external oscillator      Fosc=8MHz
  
  CPMUSYNR=initSYNR+VCOFRQ;     //set PLL multiplier   (set Fvco)
  CPMUREFDIV = initREFDV+REFFRQ;//set PLL divider   (set the Fref=Fosc/(REFDIV+1))
  CPMUPOSTDIV=0x09;           // set Post Divider    (set Fpll)          
                 
  while (CPMUFLG_LOCK == 0) {}  // wait for it
  CPMUPLL = CPMUCLKS;           // Engage PLL to system
}


void delay(void){
	/* enable timer and fast timer flag clear */
	TSCR1 = 0x90;
	/* disable timer interrupt, set prescaler to 1*/
	TSCR2 = 0x00;

	TIOS |= 0x02;
	TC1 = TCNT + 4762; //for 420Hz sampling frequency, the period is 2.38ms so require 4762 clock cycles to delay
  while(!(TFLG1_C1F));	  
	
	TIOS &= ~0x02;	
}


void freq_check(void){
  PTJ^=0x01;
  delay(); 
}


