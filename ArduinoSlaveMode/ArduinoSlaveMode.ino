#include <SPI.h>

/*SPI RELATED VARIBALES*/
boolean SSlast = LOW;         // SS last flag.
const byte led = 9;           // Slave LED digital I/O pin.
boolean ledState = HIGH;      // LED state flag.
byte rx = 0;

/*LEDs PORT*/
const int LedA = 2;
const int LedB = 3;
const int LedC = 4;
const int LedD = 5;
const int LedE = 6;
const int LedF = 7;
const int LedG = 8;
const int LedDP = 9;

/*DISPLAY COUNTER*/

int ten_min = 0;
int ten_sec = 0;
int one_min = 0;
int one_sec = 0;

//Initialize SPI slave.
void SlaveInit(void) {
  // Initialize SPI pins.
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, INPUT);
  pinMode(SS, INPUT);
  // Enable SPI as slave.
  SPCR = (1 << SPE);
}
// SPI Transfer.
byte SPItransfer(byte value) {
  SPDR = value;
  while(!(SPSR & (1<<SPIF)));
  delay(10);
  return SPDR;
}
// The setup() function runs after reset.
void setup() {
  // Initialize serial for troubleshooting.
  Serial.begin(9600);
  // Initialize slave LED pin.
  pinMode(led, OUTPUT);
  digitalWrite(led, ledState);
  // Initialize SPI Slave.
  SlaveInit();
  ledsInit();
  Serial.println("Slave Initialized");
}
// The loop function runs continuously after setup().
void loop() {
  // Slave Enabled?
  if (!digitalRead(SS)) {
    // Yes, first time?
    if (SSlast != LOW) {
      // Yes, take MISO pin.
      pinMode(MISO, OUTPUT);
      Serial.println("***Slave Enabled.");
      // Write -1 slave response code and receive master command code
      rx = SPItransfer(255);
      Serial.println("Initial -1 slave response code sent");
      Serial.println("rx:" + String(rx) + ".");
      IncrementCounter();
      //DisplayNumber(DisplayCounter);
      SSlast = LOW;
    }
  }
  else {
    // No, first time?
    if (SSlast != HIGH) {
      // Yes, release MISO pin.
      pinMode(MISO, INPUT);
      Serial.println("Slave Disabled.");
      // Update SSlast.
      SSlast = HIGH;
    }
  }
}




/*LED test*/
void ledsInit(void){
  pinMode(LedA,OUTPUT);
  pinMode(LedB,OUTPUT);
  pinMode(LedC,OUTPUT);
  pinMode(LedD,OUTPUT);
  pinMode(LedE,OUTPUT);
  pinMode(LedF,OUTPUT);
  pinMode(LedG,OUTPUT);
  pinMode(LedDP,OUTPUT);
  pinMode(A0,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  //digitalWrite(A0,HIGH);
  
  /*DEBUG*/
 // DisplayNumber(DisplayCounter);
}


/*Truth table*/
void DisplayNumber(int num){
  switch(num){
    case 0: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,HIGH); 
            digitalWrite(LedF,HIGH); 
            digitalWrite(LedG,LOW); 
            break;
    case 1: digitalWrite(LedA,LOW);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,LOW);
            digitalWrite(LedE,LOW); 
            digitalWrite(LedF,LOW); 
            digitalWrite(LedG,LOW); 
            break;
     case 2: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,LOW);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,HIGH); 
            digitalWrite(LedF,LOW); 
            digitalWrite(LedG,HIGH); 
            break;
     case 3: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,LOW); 
            digitalWrite(LedF,LOW); 
            digitalWrite(LedG,HIGH); 
            break;
     case 4: digitalWrite(LedA,LOW);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,LOW);
            digitalWrite(LedE,LOW); 
            digitalWrite(LedF,HIGH); 
            digitalWrite(LedG,HIGH); 
            break;
      case 5: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,LOW);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,LOW); 
            digitalWrite(LedF,HIGH); 
            digitalWrite(LedG,HIGH); 
            break;
      case 6: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,LOW);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,HIGH); 
            digitalWrite(LedF,HIGH); 
            digitalWrite(LedG,HIGH); 
            break;
     case 7: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,LOW);
            digitalWrite(LedE,LOW); 
            digitalWrite(LedF,LOW); 
            digitalWrite(LedG,LOW); 
            break;
      case 8: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,HIGH); 
            digitalWrite(LedF,HIGH); 
            digitalWrite(LedG,HIGH); 
            break;
     case 9: digitalWrite(LedA,HIGH);
            digitalWrite(LedB,HIGH);
            digitalWrite(LedC,HIGH);
            digitalWrite(LedD,HIGH);
            digitalWrite(LedE,LOW); 
            digitalWrite(LedF,HIGH); 
            digitalWrite(LedG,HIGH); 
            break;
               
  }  
}
void IncrementCounter(void)
{
  one_sec++;

  // Changing the Digit 
  digitalWrite(A2,LOW);
  digitalWrite(A3,HIGH);
  digitalWrite(A4,HIGH);
  digitalWrite(A5,HIGH);
  
  DisplayNumber(one_sec);
  
  if(one_sec == 10)
  {
    one_sec = 0;
    ten_sec++;
    //pattern = 0x02; // 0010

    // Changing the Digit 
    digitalWrite(A2,HIGH);
    digitalWrite(A3,LOW);
    digitalWrite(A4,HIGH);
    digitalWrite(A5,HIGH);
    
    DisplayNumber(ten_sec);
  
        
    if(ten_sec == 6)
    {
      ten_sec = 0;
      one_min++;  
      //pattern = 0x04;

      digitalWrite(A2,HIGH);
      digitalWrite(A3,HIGH);
      digitalWrite(A4,LOW);
      digitalWrite(A5,HIGH);
      
      DisplayNumber(one_min);
      if(one_min == 10)
      {
        ten_min += 1;
        one_min = 0;
        //pattern = 0x08;
         
        digitalWrite(A2,HIGH);
        digitalWrite(A3,HIGH);
        digitalWrite(A4,HIGH);
        digitalWrite(A5,LOW);
      
       DisplayNumber(ten_min);
       if(ten_min == 6)
        {
          one_sec = 0;
          one_min = 0;
          ten_sec = 0;
          ten_min = 0;
       }  
      }
     }
    }
}
  
  
 
