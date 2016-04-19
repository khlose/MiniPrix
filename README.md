# MiniPrix
ECE362 Mini Project

# Current set up

  9S12 Side <br />
    - Port M 2 = MISO <br />
    - Port M 3 = Slave Select Arduino 1 <br />
    - Port M 4 = MOSI <br />
    - Port M 5 = SCK <br />
    - Pin 4, 31 = GND <br />
    - Pin 29 = Regulated 5 V <br />
      
  Arduino Side <br />
    - Digital Pin 13 = SCK <br />
    - Digital Pin 12 = MISO <br />
    - Digital Pin 11 = MOSI <br />
    - Digital Pin 10 = Slave Select1 <br />
    - Analog in A0 = Slave Select2 <br />
    - Analog in A1 = Slave Select3 <br />
    All ground connected  to Master's Ground <br />
  4 digits 7 segment LEDs <br />
    - A 13 => Arduino 2 <br />
    - B 9 => Arduino 3 <br />
    - C 4 => Arduino 4 <br />
    - D 2 => Arduino 5 <br />
    - E 1 => Arduino 6 <br />
    - F 12 => Arduino 7 <br />
    - G 5 => Arduino 8 <br />
    - dp 3 => Arduino 9 <br />
    - Common 14 (digit 4)=> Arduino A5<br />
    - Common 11 (digit 3)=> Arduino A4<br />
    - Common 7 (Colon)=> GND <br />
    - Common 10 (digit 2)=> Arduino A3<br />
    - Common 6 (digit 1)=> Arduino A2<br />

# Dev Log
Update 2016-Apr-18 <br />
  - Moved SPI transaction from user-defined function to a proper ISR. <br />
  - Each character shifted in represents on/off status of each segment for a digit, so there will be a total of 4 chars shifted in per sec <br />
  - Using left, right push buttons on board for now to signal start/pause the timer <br />

Update 2016-Apr-16 <br />
  - TIM ISR is now finished. 9S12 will now send a character over to Slave ATmega every 1 sec
  - Future changes needed: might want to change data transferring rate to 10 ms, not 1 sec
 
Update 2016-Apr-14 <br />
  - Successfully set up SPI communication between 9S12 Master with Arduino Uno slave <br />
  - Successfully sent out one char to slave without fail <br />
  - Future Revision Needed ( BaudRate Division, adding more slave, map 4 digit 7 segmented display) <br />
 
