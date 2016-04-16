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
      
  Arduino Side
    - Pin 13 = SCK <br />
    - Pin 12 = MISO <br />
    - Pin 11 = MOSI <br />
    - Pin 10 = Slave Select <br />
    All ground connected  to Master's Ground <br />
  4 digits 7 segment LEDs <br />
    - A 13 => ATmega 2 <br />
    - B 9 => ATmega 3 <br />
    - C 4 => ATmega 7 <br />
    - D 2 => ATmega 8 <br />
    - E 1 => ATmega 9 <br />
    - F 12 => ATmega 7 <br />
    - G 5 => ATmega 8 <br />
    - dp 3 => ATmega 9 <br />
    - Common 14 11 7 10 6 => ATmega GND <br />
    

# Dev Log
Update 2016-Apr-16 <br />
  - TIM ISR is now finished. 9S12 will now send a character over to Slave ATmega every 1 sec
  - Future changes needed: might want to change data transferring rate to 10 ms, not 1 sec
 
Update 2016-Apr-14 <br />
  - Successfully set up SPI communication between 9S12 Master with Arduino Uno slave <br />
  - Successfully sent out one char to slave without fail <br />
  - Future Revision Needed ( BaudRate Division, adding more slave, map 4 digit 7 segmented display) <br />
 
