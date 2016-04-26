#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include <mc9s12c32.h>

/* All functions after main should be initialized here */
char inchar(void);
void outchar(char x);

void led_strip(int,int,int,int);
void delay(void);
void led_strip2(void);
void clock_delay(void);

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
  
  
  DDRT = 0xFF;
  PTT_PTT5 = 0;//clock
  PTT_PTT4 = 0;//Data
	      
	      
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
  
 for(;;) 
 {
    led_strip(6,6,0,50);


  
  } /* loop forever */
   
}   /* do not leave main */




/* This below function is used to operate LPD 6803 RGB LED Strip
    
   parameters:
    r -> Red Part of the RGB
    g -> Green Part of the RGB
    b -> Blue part of the RGB
    nDots => This is used to indicate how many LED lights on the Strip will be lit. On our LED Strip, we have 50 LED Lights on the Strip   

 */

void led_strip(int r,int g,int b,int nDots)
{
  // r g b (0,5)
  int i=0;
  int jr=0;
  int jb=0;
  int jg=0;
  int kr =0;
  int kb=0;
  int kg=0;
  
  
  /*Error for Parameters checking*/
  if(r < 0){r=0;}
  if(g < 0){g=0;}
  if(b < 0){b=0;}
  if(r > 5){r=5;}
  if(g > 5){g=5;}
  if(b > 5){b=5;}
  
  if(nDots <= 0){ nDots = 1;}
  if(nDots > 50){nDots = 50;}
  
  
  
  PTT_PTT4 = 0;
  PTT_PTT5 = 0;
 
 
  // This loop is used to configure the LED Strip to take the input data Sequence.
  
  for(i=0;i<32;i++)
  {
    PTT_PTT5 = 1;
    clock_delay();
    PTT_PTT5 = 0;
    clock_delay();
  }
  
  
  /*
    
    For the LPD 6803 LED Strip, we need 16 bits of data to light up one LED Light in the Strip. 
    
    First Bit -> should always be 1
   
    Bit 1 - Bit 5 -> in the Data Sequence is used to indicate the brightness of the Green part of the RGB LED
    
    Bit 6 - Bit 10 -> in the Data Sequence is used to indicate the brightness of the Red part of the RGB LED
    
    Bit 11 - Bit 15 -> in the Data Sequence is used to indicate the brightness of the Blue part of the RGB LED
    
    Remember: The Data frame is shifted on Rising Edge of the Clocking Signal.
    
  */
  
  for(i=0;i<nDots;i++)
  {
    // Below three lines inputs the first bit of the data frame to be 1 on the rising edge of the Clocking Signal
    PTT_PTT4 = 1;
    PTT_PTT5 = 1;
    PTT_PTT5 = 0;
    
    //GREEN LED
    for(jg=0;jg<g;jg++)
    {
      PTT_PTT4 = 1;
      PTT_PTT5 = 1;
      clock_delay();
      PTT_PTT5 = 0;
      clock_delay();
    
    }
    for(kg=0;kg<5-jg;jg++)
    {
      PTT_PTT4 = 0;
      PTT_PTT5 = 1;
      clock_delay();
      PTT_PTT5 = 0;
      clock_delay();
    
    }
  
  
    //RED LED  
    for(jr=0;jr<r;jr++)
    {
      PTT_PTT4 = 1;
      PTT_PTT5 = 1;
      clock_delay();
      PTT_PTT5 = 0;
      clock_delay();
    
    }
    
    for(kr=0;kr<5-jr;kr++)
    {
      PTT_PTT4 = 0;
      PTT_PTT5 = 1;
      clock_delay();
      PTT_PTT5 = 0;
      clock_delay();
    
    }
    
    //Blue LED
    for(jb=0;jb<b;jb++)
    {
      PTT_PTT4 = 1;
      PTT_PTT5 = 1;
      clock_delay();
      PTT_PTT5 = 0;
      clock_delay();
    
    }
    for(kb=0;kb<5-jb;kb++)
    {
      PTT_PTT4 = 0;
      PTT_PTT5 = 1;
      clock_delay();
      PTT_PTT5 = 0;
      clock_delay();
    
    }
  }
  
  
  PTT_PTT4 = 0;  // Pulling the Data Line Low
  
  // This loop is used to indicate that the Data Sequence has ended.
  for(i=0;i<nDots;i++)
  {
    PTT_PTT5 = 1;
    clock_delay();
    PTT_PTT5 = 0;
    clock_delay();  
  }
  
  // Just Creating some delay to finish the whole operation
  clock_delay();
  clock_delay();
  clock_delay();


}    // led_strip


// Based on Trial and Error, It is found that using a clocking signal of frequency 25 KHz we can make sure that LED Strip gives steady display.
void clock_delay(void)
{
  int i=0;
  int j = 0;
  for(i=0;i<6;i++)
  {
  
    for(j=0;j<10;j++)
    {
  
    }
  
  } // 

}   // clock_delay

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