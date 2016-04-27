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
#include "flash.h"

/****************** All functions after main should be initialized here ********************/
char inchar(void);
void outchar(char x);

void SpiInit(void);
void IncrementTimer(void);

void lcdwait(void);
void putcspi(char);
void putsspi(char[]);
void ShiftOutMainTime(void);;
void ShiftOutFirstCar(void);
void ShiftOutSecondCar(void);
void led_strip(int,int,int,int);
void clock_delay(void);
int compare_highscore(void);
void store_highscore(int);
void populate_record(void);


/******************************************************************************************/



/****************************** Variable declarations ********************************/

/*TIM ISR related variables*/
char onesec = 0;	// one second flag
char tenths	= 0;	// tenth of a second flag
int tencnt = 0;
int onecnt = 0;


/* Push button states */

int prev_leftpb = 0; // Previous left pushbutton state
int prev_rghtpb = 0; // Previous right pushbutton state
int prev_ready1 = 0; 
int prev_ready2 = 0;
int prev_finish1 = 0;
int prev_finish2 = 0;
int prev_reset = 0;

int RTICNT = 0;

/* Status variables*/
int start = 0;   // Status that the clock has started/stopped 0 -> stopped , 1 -> started
int player1_ready = 0;
int player2_ready = 0;
int player1_finish = 0;
int player2_finish = 0;
int cars_released = 0;
int is_time1_recorded = 0;
int is_time2_recorded = 0;
int reset = 0;
int race_winner = 0; //0 = record, 1 = player1, 2 = player2


/* Timer variables */
char ten_sec = 0; // MM:"S"S
char ten_min = 0; // "M"M:SS
char one_sec = 0; // MM:S"S"
char one_min = 0; // M"M":SS

char ten_sec_finish2 = 0;
char one_sec_finish2 = 0;
char ten_min_finish2 = 0;
char one_min_finish2 = 0;

char ten_sec_finish1 = 0;
char one_sec_finish1 = 0;
char ten_min_finish1 = 0;
char one_min_finish1 = 0;

char ten_sec_record = 0;
char one_sec_record = 0;
char ten_min_record = 0;
char one_min_record = 0;
int whole_min_record = 0;
int whole_sec_record = 0;

int total_sec_record = 0;
int total_sec_current=0;
/*LED Strip*/
int dot_per_sec = 0;



/* LOOK UP TABLE FOR CHAR TO BE SHIFTED OUT*/

/* 8 bits = 8 segmented from left to right DP G F E... B A*/
/*led_rep[0] = segmented needed to display number 0 on 7 segmented LEDs*/
char led_rep[10] = { 0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};


/*flash storage*/

int rval = 0; // return value from writing
int is_there_record = 0;




/*Debug*/
char tm=5;
char om = 4;
char ts = 3;
char os = 2;
int m = 0;
int s = 0;
int debug_store=0;


/***************************************************************************************/


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
  DDRT = 0xFF;
  TSCR1 = 0x80;
  TSCR2 = 0x0C;
  TIOS = 0x80;
  TIE = 0x80;
  TC7 = 1500;
  
 /* ATD */
  DDRAD = 0;
  ATDCTL2 = 0x80;
  ATDCTL3 = 0x10;
  ATDCTL4 = 0x85;
  
  //ch01 34
  ATDDIEN = 0b11011111;
  
  RTICTL = 0x1F;
  CRGINT = 0x80;
 
  /*SPI*/
  DDRM = 0xFF;
  SPICR1 = 0x50;
  SPICR2 = 0x02; // should probably be 0x00
  SPIBR = 0x77; //options are 0x00 and 0x60
  
  
  /*Flash storage*/
  Flash_Init(8000);
  
  if(*(unsigned int*)0x4000 == 0xFFFF){
    //default value is 0xFFFF, nothing is stored
    ten_sec_record = 9;
    one_sec_record = 9;
    ten_min_record = 9;
    one_min_record = 9;
    is_there_record = 0;
  } else{
    //high score was recorded
    //location 0x4000 | 0x4001 = ten min | one min
    ten_min_record = (*(unsigned char*)0x4000);
    one_min_record = (*(unsigned char*)0x4001);
    ten_sec_record = (*(unsigned char*)0x4002);
    one_sec_record = (*(unsigned char*)0x4003);
    is_there_record = 1;
  
  }
  
  /*Record Init*/
  total_sec_record = ten_min_record*600 + one_min_record*60 + ten_sec_record*10 + one_sec_record;
  dot_per_sec = 50/total_sec_record;
  total_sec_current = 0;
  
  /*LED STRIP INIT*/
  MODRR = 0;
  PTT_PTT4 = 0;
  PTT_PTT5 = 0;
  led_strip(5,5,5,50);
  
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


  //MC9S12C32 is using 8MHZ oscillator
  
  Flash_Init(8000);
  

 
 
 
 for(;;) {
  
  if(player1_ready == 1 && player2_ready == 1 && start == 0){
    start = 1;
    led_strip(5,5,5,50);
    populate_record();
    //clear all the display first
  }
   
  if(start == 1)
  {  
    if(onesec == 1)
    {
      onesec = 0;
      
      //shift time out to main display (#0)
      ShiftOutMainTime();
      
      total_sec_current++;
      IncrementTimer();
      if(is_there_record == 1){
        
        led_strip(5,0,0,(int)(total_sec_current*dot_per_sec));
      }
    }
    if(cars_released == 0){
      //set solenoid out pin high
      
      //wait 5 secs
      //set the pin low
      
      //make sure solenoid won't be activated again
      cars_released = 1;
      
    }
    
  }
  
  if(player1_finish == 1 && is_time1_recorded == 0){
    ten_sec_finish1 = ten_sec;
    one_sec_finish1 = one_sec;
    ten_min_finish1 = ten_min;
    one_min_finish1 = one_min;
    ShiftOutFirstCar();
    is_time1_recorded = 1;
        
  }
  if(player2_finish == 1 && is_time2_recorded == 0){
    ten_sec_finish2 = ten_sec;
    one_sec_finish2 = one_sec;
    ten_min_finish2 = ten_min;
    one_min_finish2 = one_min;
    ShiftOutSecondCar();
    is_time2_recorded =1;
        
  }
  
  if(player1_finish == 1 && player2_finish == 1 & is_time1_recorded == 1 & is_time2_recorded == 1){
    /*race ended, reset everything*/
    start = 0;
    race_winner = compare_highscore();
    store_highscore(race_winner);
    //populate_record();
    player1_ready = 0;
    player2_ready = 0;
    player1_finish = 0;
    player2_finish = 0;
    //be careful here, this statement will mess with solenoid
    cars_released = 0;
    is_time2_recorded = 0;
    is_time1_recorded = 0;
    ten_sec = 0;
    ten_min = 0;
    one_sec = 0;
    one_min = 0;
    total_sec_current = 0;
    //
  }
  if(reset == 1){
    //populate_record();
    reset = 0;
    ten_min = 0;
    one_min = 0;
    ten_sec = 0;
    one_sec = 0;
    start = 0;
    cars_released = 0;
    ten_sec_finish2 = 0;
    one_sec_finish2 = 0;
    ten_min_finish2 = 0;
    one_min_finish2 = 0;
    ten_sec_finish1 = 0;
    one_sec_finish1 = 0;
    ten_min_finish1 = 0;
    one_min_finish1 = 0;
    is_time1_recorded = 0;
    is_time2_recorded = 0;
    player1_ready = 0;
    player2_ready = 0;
    ShiftOutMainTime();
    ShiftOutFirstCar();
    ShiftOutSecondCar();
    led_strip(5,5,5,50);
    //
  }
  

  
  
 }/* loop forever */
   
}   /* do not leave main */




/*
***********************************************************************                       
 RTI interrupt service routine: RTI_ISR
************************************************************************
*/

interrupt 7 void RTI_ISR(void)
{
  	// clear RTI interrupt flag
  	CRGFLG = CRGFLG | 0x80;
  	RTICNT++;
  	if(RTICNT>=48){
  	  RTICNT = 0;
  	}
  	
  	if(prev_leftpb == 1 && PTAD_PTAD7 == 0){
      debug_store = 1;
      
    }
    if(prev_rghtpb == 1 && PTAD_PTAD6 == 0){
      debug_store = 0; 
    }
    
    if(prev_ready1 == 1 && PTAD_PTAD0 == 0){
      if(player1_ready == 1){
        player1_ready = 0;
      } else{
        player1_ready = 1;
      }  
    }
    if(prev_ready2 == 1 && PTAD_PTAD1 == 0){
      if(player2_ready == 1){
        player2_ready = 0;
      } else{
        player2_ready = 1;
      }
    }
    
    if(prev_finish1 == 1 && PTAD_PTAD3 == 0){
      player1_finish = 1;  
    }
    if(prev_finish2 == 1 && PTAD_PTAD4 == 0){
      player2_finish = 1;
    }
    if(prev_reset == 1 && PTAD_PTAD2 == 0){
      reset = 1;
    }
    
    prev_rghtpb = PTAD_PTAD6;
    prev_leftpb = PTAD_PTAD7;
    prev_ready1 = PTAD_PTAD0;
    prev_ready2 = PTAD_PTAD1;
    prev_reset = PTAD_PTAD2;
    prev_finish1 = PTAD_PTAD3;
    prev_finish2 = PTAD_PTAD4;
    

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
 	if(onecnt >= 1000){ //100 of 10 ms = 1 sec
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

/*
***********************************************************************
 Name:         lcdwait 
 Description:  SPI delay
***********************************************************************
*/
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
/*
***********************************************************************
 Name:         putcspi
 Description:  shifting a single character out via spi
***********************************************************************
*/
void putcspi(char cx) {
    
  char temp;
  PTM_PTM3 = 0;
  while(!SPISR_SPTEF);
  SPIDR = cx;
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  PTM_PTM3 = 1;
}

/*
***********************************************************************
 Name:         putcspi
 Description:  shifting a STRING out via spi
***********************************************************************
*/
void putsspi(char* ptr){
  while(*ptr){
    putcspi(*ptr);
    ptr++; 
  }
}

/*
***********************************************************************
 Name:         ShiftOutMainTime
 Description:  Shift out race time to the main display
***********************************************************************
*/

void ShiftOutMainTime(void){

  char temp;
  PTM_PTM3 = 0;
  PTM_PTM1 = 1;
  PTM_PTM0 = 0;
  
  while(!SPISR_SPTEF);
  SPIDR = led_rep[one_sec];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  
  while(!SPISR_SPTEF);
  SPIDR = led_rep[ten_sec];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  while(!SPISR_SPTEF);
  SPIDR = led_rep[one_min];
  lcdwait();  
  while(!SPISR_SPIF);
  temp = SPIDR;
  SPIDR = led_rep[ten_min];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  
  while(!SPISR_SPTEF);
  SPIDR = '\n';
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  PTM_PTM3 = 1;  
}
/*
***********************************************************************
 Name:         ShiftOutFirstCar
 Description:  Shift out first player's finish time to the display
***********************************************************************
*/
void ShiftOutFirstCar(void){

  char temp;
  char ptm1tmp = PTM_PTM1;
  char ptm0tmp = PTM_PTM1;
  PTM_PTM3 = 0;
  PTM_PTM1 = 1;
  PTM_PTM0 = 1;
  
  while(!SPISR_SPTEF);
  SPIDR = led_rep[one_sec_finish1];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  
  while(!SPISR_SPTEF);
  SPIDR = led_rep[ten_sec_finish1];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  while(!SPISR_SPTEF);
  SPIDR = led_rep[one_min_finish1];
  lcdwait();  
  while(!SPISR_SPIF);
  temp = SPIDR;
  SPIDR = led_rep[ten_min_finish1];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  
  while(!SPISR_SPTEF);
  SPIDR = '\n';
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  PTM_PTM3 = 1;
  PTM_PTM1 = ptm1tmp;
  PTM_PTM0 = ptm0tmp;
    

}

/*
***********************************************************************
 Name:         ShiftOutSecondCar
 Description:  Shift out second player's finish time to the display
***********************************************************************
*/

void ShiftOutSecondCar(void){

  char temp;
  char ptm1tmp = PTM_PTM1;
  char ptm0tmp = PTM_PTM1;
  
  PTM_PTM3 = 0;
  PTM_PTM1 = 0;
  PTM_PTM0 = 1;
  
  while(!SPISR_SPTEF);
  SPIDR = led_rep[one_sec_finish2];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  
  while(!SPISR_SPTEF);
  SPIDR = led_rep[ten_sec_finish2];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  while(!SPISR_SPTEF);
  SPIDR = led_rep[one_min_finish2];
  lcdwait();  
  while(!SPISR_SPIF);
  temp = SPIDR;
  SPIDR = led_rep[ten_min_finish2];
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  
  while(!SPISR_SPTEF);
  SPIDR = '\n';
  lcdwait();
  while(!SPISR_SPIF);
  temp = SPIDR;
  PTM_PTM3 = 1;
  PTM_PTM1 = ptm1tmp;
  PTM_PTM0 = ptm0tmp;  

}

/*
***********************************************************************
 Name:         IncrementTimer
 Description:  Increment the main race time
***********************************************************************
*/
void IncrementTimer(void){
  one_sec++;
  if(one_sec > 9)
  {
    one_sec = 0;
    ten_sec++;
    if(ten_sec > 5)
    {
      ten_sec = 0;
      one_min++;
      if(one_min > 9)
      {
        one_min = 0;
        ten_min++;
        if(ten_min > 5)
        {
          ten_min = 0;
          one_min = 0;
          ten_sec = 0;
          one_sec = 0;
        }  
        
      } 
    }
  }

}

/*
*********************************************************************************************************
                                                                                                       
  FUNCTION    : led_strip
  DESCRIPTION : Take in RGB value out of five, take in number of dots out of fifty
                and display appropriate color on that amount of dots (pixels)
**********************************************************************************************************
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


/*
*********************************************************************************************************
                                                                                                       
  FUNCTION    : clock_delay
  DESCRIPTION : Give enough delay for clocking signal

**********************************************************************************************************
*/

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
*********************************************************************************************************
                                                                                                       
  FUNCTION    : compare_highscore
  DESCRIPTION : Give enough delay for clocking signal

**********************************************************************************************************
*/
int compare_highscore(void){
  int winner_of_2 = 0;
  char winner2_ten_sec=0;
  char winner2_one_sec=0;
  char winner2_ten_min=0;
  char winner2_one_min=0;
  int final_winner = 0;
  
  if(ten_min_finish1 > ten_min_finish2)
  {
  // two is clearly the winner
    winner_of_2 = 2;
  
  } 
  else if(ten_min_finish1 < ten_min_finish2)
  {
  // one is clearly the winner
    winner_of_2 = 1; 
  
  } 
  else 
  {
    //move on to one_min
    if(one_min_finish1 > one_min_finish2)
    {
      // two is clearly the winner
      winner_of_2 = 2; 
    } 
    else if(ten_min_finish1 < ten_min_finish2)
    {
    // one is clearly the winner
      winner_of_2 = 1;
    } 
    else
    {
      if(ten_sec_finish1 > ten_sec_finish2)
      {
        // two is clearly the winner
        winner_of_2 = 2;
      } 
      else if(ten_sec_finish1 < ten_sec_finish2)
      {
      // one is clearly the winner
        winner_of_2 = 1;
      } 
      else
      {
        if(one_sec_finish1 > one_sec_finish2)
        {
          // two is clearly the winner
          winner_of_2 = 2;
        } 
        else if(one_sec_finish1 < one_sec_finish2)
        {
        // one is clearly the winner
          winner_of_2 = 1;
        } 
    
      }
    
    }
  }
  //compare with record
  switch(winner_of_2){
    case 1:winner2_ten_min = ten_min_finish1;
           winner2_ten_sec = ten_sec_finish1;
           winner2_one_sec = one_sec_finish1;
           winner2_one_min = one_min_finish1;
           break;
    case 2:winner2_ten_min = ten_min_finish2;
           winner2_ten_sec = ten_sec_finish2;
           winner2_one_sec = one_sec_finish2;
           winner2_one_min = one_min_finish2;
           break;
    default:winner2_ten_min = ten_min_finish1;
            winner2_ten_sec = ten_sec_finish1;
            winner2_one_sec = one_sec_finish1;
            winner2_one_min = one_min_finish1;
            winner_of_2 = 1;     
  }
  if(winner2_ten_min > ten_min_record)
  {
  // two is clearly the winner
    final_winner = winner_of_2 * 0;
  
  } 
  else if(winner2_ten_min < ten_min_record)
  {
  // one is clearly the winner
    final_winner = winner_of_2 + 0; 
  
  } 
  else 
  {
    //move on to one_min
    if(winner2_one_min > one_min_record)
    {
      // two is clearly the winner
      final_winner = winner_of_2 * 0; 
    } 
    else if(winner2_one_min < one_min_record)
    {
    
      final_winner = winner_of_2 + 0; 
    } 
    else
    {
      if(winner2_ten_sec > ten_sec_record)
      {
        // two is clearly the winner
        final_winner = winner_of_2 * 0;
      } 
      else if(winner2_ten_sec < ten_sec_record)
      {
      // one is clearly the winner
        final_winner = winner_of_2 + 0;
      } 
      else
      {
        if(winner2_one_sec > one_sec_record)
        {
          // two is clearly the winner
          final_winner = winner_of_2 * 0; 
        } 
        else if(winner2_one_sec < one_sec_record)
        {
        // one is clearly the winner
          final_winner = winner_of_2 + 0;
        } 
    
      }
    
    }
  }
  return final_winner;
  
  

}


/*
*********************************************************************************************************
                                                                                                       
  FUNCTION    : store_highscore
  DESCRIPTION : Store the main score

**********************************************************************************************************
*/

void store_highscore(int winner){
  char to_store_ten_min=0;
  char to_store_ten_sec=0;
  char to_store_one_min=0;
  char to_store_one_sec=0;
  int to_store_whole_min = 0;
  int to_store_whole_sec = 0;
  int retval;
  switch(winner){
    case 0:to_store_ten_min = ten_min_record;
           to_store_ten_sec = ten_sec_record;
           to_store_one_sec = one_sec_record;
           to_store_one_min = one_min_record;
           break;
    case 1:to_store_ten_min = ten_min_finish1;
           to_store_ten_sec = ten_sec_finish1;
           to_store_one_sec = one_sec_finish1;
           to_store_one_min = one_min_finish1;
           break;
    case 2:to_store_ten_min = ten_min_finish2;
           to_store_ten_sec = ten_sec_finish2;
           to_store_one_sec = one_sec_finish2;
           to_store_one_min = one_min_finish2;
            break;
    default:to_store_ten_min = ten_min_record;
           to_store_ten_sec = ten_sec_record;
           to_store_one_sec = one_sec_record;
           to_store_one_min = one_min_record;
  }
  
  retval = Flash_Erase_Sector((unsigned int *)0x4000);
  
  to_store_whole_min = (int)to_store_ten_min << 8 | to_store_one_min;
  to_store_whole_sec = (int)to_store_ten_sec << 8 | to_store_one_sec;
  retval = Flash_Write_Word((unsigned int *)0x4000,to_store_whole_min);
  retval = Flash_Write_Word((unsigned int *)0x4002,to_store_whole_sec);    
}

void populate_record(void){
  if(*(unsigned int*)0x4000 == 0xFFFF){
    //default value is 0xFFFF, nothing is stored
    ten_sec_record = 9;
    one_sec_record = 9;
    ten_min_record = 9;
    one_min_record = 9;
    is_there_record = 0;
  } else{
    //high score was recorded
    //location 0x4000 | 0x4001 = ten min | one min
    ten_min_record = (*(unsigned char*)0x4000);
    one_min_record = (*(unsigned char*)0x4001);
    ten_sec_record = (*(unsigned char*)0x4002);
    one_sec_record = (*(unsigned char*)0x4003);
    is_there_record = 1;
  
  }
  
  /*Record Init*/
  total_sec_record = ten_min_record*600 + one_min_record*60 + ten_sec_record*10 + one_sec_record;
  dot_per_sec = 50/total_sec_record;
  total_sec_current = 0;

}