#include <SPI.h>

char buf[100] = {0x3F,0x3F,0x3F,0x3F};
volatile byte pos;
volatile bool process_it;

const int LedA = 2;
const int LedB = 3;
const int LedC = 4;
const int LedD = 5;
const int LedE = 6;
const int LedF = 7;
const int LedG = 8;
//const int LedDP = 9;
const int Dig1 = A2;
const int Dig2 = A3;
const int Dig3 = A4;
const int Dig4 = A5;



void setup(void)
{
  Serial.begin(115200);   // debugging

              // turn on SPI in slave mode
  SPCR |= bit(SPE);

  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);

  // get ready for an interrupt
  pos = 0;   // buffer empty
  process_it = false;

  // now turn on interrupts
  SPI.attachInterrupt();
  LedInit();

}  // end of setup


   // SPI interrupt routine
ISR(SPI_STC_vect)
{
  byte c = SPDR;  // grab byte from SPI Data Register

          // add to buffer if room
  if (pos < sizeof buf)
  {
    buf[pos++] = c;

    // example: newline means time to process buffer
    if (c == '\n')
      process_it = true;

  }  // end of room available
}  // end of interrupt routine SPI_STC_vect

   // main loop - wait for flag set in interrupt routine
void loop(void)
{
  if (process_it)
  {
    //buf[pos] = 0;
    //Serial.println(buf);
    pos = 0;
    process_it = false;
  }  // end of flag set
  RefreshDisplay();
}  // end of loop




void DisplayNumber(char num) {
  switch (num) {
  case 0x3F: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, HIGH);
    digitalWrite(LedF, HIGH);
    digitalWrite(LedG, LOW);
    break;
  case 0x06: digitalWrite(LedA, LOW);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, LOW);
    digitalWrite(LedE, LOW);
    digitalWrite(LedF, LOW);
    digitalWrite(LedG, LOW);
    break;
  case 0x5B: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, LOW);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, HIGH);
    digitalWrite(LedF, LOW);
    digitalWrite(LedG, HIGH);
    break;
  case 0x4F: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, LOW);
    digitalWrite(LedF, LOW);
    digitalWrite(LedG, HIGH);
    break;
  case 0x66: digitalWrite(LedA, LOW);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, LOW);
    digitalWrite(LedE, LOW);
    digitalWrite(LedF, HIGH);
    digitalWrite(LedG, HIGH);
    break;
  case 0x6D: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, LOW);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, LOW);
    digitalWrite(LedF, HIGH);
    digitalWrite(LedG, HIGH);
    break;
  case 0x7D: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, LOW);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, HIGH);
    digitalWrite(LedF, HIGH);
    digitalWrite(LedG, HIGH);
    break;
  case 0x07: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, LOW);
    digitalWrite(LedE, LOW);
    digitalWrite(LedF, LOW);
    digitalWrite(LedG, LOW);
    break;
  case 0x7F: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, HIGH);
    digitalWrite(LedF, HIGH);
    digitalWrite(LedG, HIGH);
    break;
  case 0x6F: digitalWrite(LedA, HIGH);
    digitalWrite(LedB, HIGH);
    digitalWrite(LedC, HIGH);
    digitalWrite(LedD, HIGH);
    digitalWrite(LedE, LOW);
    digitalWrite(LedF, HIGH);
    digitalWrite(LedG, HIGH);
    break;

  }
}



void RefreshDisplay() {

  PORTD &= 0b00000011;
  PORTB &= 0b11111110;
  digitalWrite(Dig1, LOW);
  digitalWrite(Dig2, HIGH);
  digitalWrite(Dig3, HIGH);
  digitalWrite(Dig4, HIGH);
  PORTD |= (buf[0] << 2);
  PORTB |= (buf[0] >> 6);
  delay(1);
  PORTD &= 0b00000011;
  PORTB &= 0b11111110;
  digitalWrite(Dig1, HIGH);
  digitalWrite(Dig2, LOW);
  digitalWrite(Dig3, HIGH);
  digitalWrite(Dig4, HIGH);
  PORTD |= (buf[1] << 2);
  PORTB |= (buf[1] >> 6);
  delay(1);
  PORTD &= 0b00000011;
  PORTB &= 0b11111110;
  digitalWrite(Dig1, HIGH);
  digitalWrite(Dig2, HIGH);
  digitalWrite(Dig3, LOW);
  digitalWrite(Dig4, HIGH);
  PORTD |= (buf[2] << 2);
  PORTB |= (buf[2] >> 6);
  delay(1);
  PORTD &= 0b00000011;
  PORTB &= 0b11111110;
  digitalWrite(Dig1, HIGH);
  digitalWrite(Dig2, HIGH);
  digitalWrite(Dig3, HIGH);
  digitalWrite(Dig4, LOW);
  PORTD |= (buf[3] << 2);
  PORTB |= (buf[3] >> 6);
  delay(1);

}



void LedInit() {
  pinMode(LedA, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(LedC, OUTPUT);
  pinMode(LedD, OUTPUT);
  pinMode(LedE, OUTPUT);
  pinMode(LedF, OUTPUT);
  pinMode(LedG, OUTPUT);
  //pinMode(LedDP, OUTPUT);
  pinMode(Dig1, OUTPUT);
  pinMode(Dig2, OUTPUT);
  pinMode(Dig3, OUTPUT);
  pinMode(Dig4, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(Dig1, LOW);
  digitalWrite(Dig2, LOW);
  digitalWrite(Dig3, LOW);
  digitalWrite(Dig4, LOW);
  digitalWrite(9, HIGH);
}

