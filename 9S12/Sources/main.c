/*
************************************************************************
 ECE 362 - Mini-Project C Source File - Spring 2016
***********************************************************************
	 	   			 		  			 		  		
 Team ID: < ? >

 Project Name: < ? >

 Team Members:

   - Team/Doc Leader: < ? >      Signature: ______________________
   
   - Software Leader: < ? >      Signature: ______________________

   - Interface Leader: < ? >     Signature: ______________________

   - Peripheral Leader: < ? >    Signature: ______________________


 Academic Honesty Statement:  In signing above, we hereby certify that we 
 are the individuals who created this HC(S)12 source file and that we have
 not copied the work of any other student (past or present) while completing 
 it. We understand that if we fail to honor this agreement, we will receive 
 a grade of ZERO and be subject to possible disciplinary action.

***********************************************************************

 The objective of this Mini-Project is to .... < ? >


***********************************************************************

 List of project-specific success criteria (functionality that will be
 demonstrated):

 1.

 2.

 3.

 4.

 5.

***********************************************************************

  Date code started: < ? >

  Update history (add an entry every time a significant change is made):

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >

  Date: < ? >  Name: < ? >   Update: < ? >


***********************************************************************
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);

void lcdwait(void);
void putcspi(char);
void putsspi(char[]);

/*spi test variable*/
char tmp;

char SampleChar;


char onesec = 0;	// one second flag
char tenths	= 0;	// tenth of a second flag
char tin	= 0;	// SCI transmit display buffer IN pointer
char tout	= 0;	// SCI transmit display buffer OUT pointer
int pulscnt 	= 0;	// pulse count (read from PA every second)
int tencnt = 0;
int onecnt = 0;

void SpiInit(void);
/* Variable declarations */

   	   			 		  			 		       

/* Special ASCII characters */
#define CR 0x0D		// ASCII return 
#define LF 0x0A		// ASCII new line 

/* LCD COMMUNICATION BIT MASKS (note - different than previous labs) */
#define RS 0x10		// RS pin mask (PTT[4])
#define RW 0x20		// R/W pin mask (PTT[5])
#define LCDCLK 0x40	// LCD EN/CLK pin mask (PTT[6])

/* LCD INSTRUCTION CHARACTERS */
#define LCDON 0x0F	// LCD initialization command
#define LCDCLR 0x01	// LCD clear display command
#define TWOLINE 0x38	// LCD 2-line enable command
#define CURMOV 0xFE	// LCD cursor move instruction
#define LINE1 = 0x80	// LCD line 1 cursor position
#define LINE2 = 0xC0	// LCD line 2 cursor position

	 	   		
/*	 	   		
***********************************************************************
 Initializations
***********************************************************************
*/

void  initializations(void) {

/* Set the PLL speed (bus clock = 24 MHz) */
  CLKSEL = CLKSEL & 0x80; //; disengage PLL from system
  PLLCTL = PLLCTL | 0x40; //; turn on PLL
  SYNR = 0x02;            //; set PLL multiplier
  REFDV = 0;              //; set PLL divider
  while (!(CRGFLG & 0x08)){  }
  CLKSEL = CLKSEL | 0x80; //; engage PLL

/* Disable watchdog timer (COPCTL register) */
  COPCTL = 0x40   ; //COP off; RTI and COP stopped in BDM-mode

/* Initialize asynchronous serial port (SCI) for 9600 baud, interrupts off initially */
  SCIBDH =  0x00; //set baud rate to 9600
  SCIBDL =  0x9C; //24,000,000 / 16 / 156 = 9600 (approx)  
  SCICR1 =  0x00; //$9C = 156
  SCICR2 =  0x0C; //initialize SCI for program-driven operation
  DDRB   =  0x10; //set PB4 for output mode
  PORTB  =  0x10; //assert DTR pin on COM port

/* Initialize peripherals */

            
/* Initialize interrupts */

  /*TIM*/
  /*
  DDRT = 0xFF;
  TSCR1 = 0x80;
  TSCR2 = 0x0C;
  TIOS = 0x80;
  TIE = 0x80;
  TC7 = 1500;
  */
  
  /*SPI*/
  
  
  
  DDRM = 0xFF;
  
  /*
  PTM_PTM2 = 0;
  PTM_PTM5 = 0; //SCK low
  PTM_PTM4 = 0; //MOSI low
  PTM_PTM3 = 1; //SS high
  */
  SPICR1 = 0x50;
  SPICR2 = 0x02; // should probably be 0x00
  SPIBR = 0x77; //options are 0x00 and 0x60
  //WOMM = 0x00; //enable port M pull up
  /*
  PTM_PTM2 = 1;
  PTM_PTM3 = 1;
  PTM_PTM4 = 1;
  PTM_PTM5 = 1;
  */
	      
}

	 		  			 		  		
/*	 		  			 		  		
***********************************************************************
Main
***********************************************************************
*/
void main(void) {
  	DisableInterrupts
	initializations(); 		  			 		  		
	EnableInterrupts;
  

    
  //putsspi("Hello, World");
                 
  

  
 for(;;) {
  
  
  
  putcspi('a');
  
  
   } /* loop forever */
   
}   /* do not leave main */




/*
***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flagt 
  	CRGFLG = CRGFLG | 0x80; 
 

}

/*
***********************************************************************                       
  TIM interrupt service routine	  		
***********************************************************************
*/

interrupt 15 void TIM_ISR(void)
{
  	// clear TIM CH 7 interrupt flag 
 	TFLG1 = TFLG1 | 0x80; 
 	tencnt++;
 	onecnt++;
 	if(tencnt >= 10){ //10 of 10 ms = 0.1 sec
    tenths = 1;
    tencnt = 0; 	
 	}
 	if(onecnt >= 100){ //100 of 10 ms = 1 sec
 	  onecnt = 0;
 	  onesec = 1;
 	  
 	  
 	  
 	  
 	}
}

/*
***********************************************************************                       
  SCI interrupt service routine		 		  		
***********************************************************************
*/

interrupt 20 void SCI_ISR(void)
{
 


}

/*
***********************************************************************
 Character I/O Library Routines for 9S12C32 
***********************************************************************
 Name:         inchar
 Description:  inputs ASCII character from SCI serial port and returns it
 Example:      char ch1 = inchar();
***********************************************************************
*/

char inchar(void) {
  /* receives character from the terminal channel */
        while (!(SCISR1 & 0x20)); /* wait for input */
    return SCIDRL;
}

/*
***********************************************************************
 Name:         outchar    (use only for DEBUGGING purposes)
 Description:  outputs ASCII character x to SCI serial port
 Example:      outchar('x');
***********************************************************************
*/

void outchar(char x) {
  /* sends a character to the terminal channel */
    while (!(SCISR1 & 0x80));  /* wait for output buffer empty */
    SCIDRL = x;
}


void lcdwait()
{
  int countero = 8;
  int counteri =  6000;
  while(countero != 0){
    while(counteri != 0) {
      counteri--;
    }
    countero--;
  } 
}

void putcspi(char cx) {
  
  
  char temp;
  //while(!(SPISR & SPISR_SPTEF));
  PTM_PTM3 = 0;
  while(!SPISR_SPTEF);
  //while(!(SPISR & SPISR_SPTEF));
  SPIDR = cx;
  lcdwait();
  while(!SPISR_SPIF);
  //while(!(SPISR & SPISR_SPTEF));
  temp = SPIDR;
  PTM_PTM3 = 1;
}


void putsspi(char* ptr){
  while(*ptr){
    putcspi(*ptr);
    ptr++; 
  }
}

char getcspi(void){
  while(!(SPISR & SPISR_SPTEF));
  SPIDR = 0x00;
  while(!(SPISR & SPISR_SPIF));
  return SPIDR;
}

void getsspi(char* ptr, char count){
  while(count){
    *ptr++ = getcspi();
    count--;
  }
  *ptr = 0;
}