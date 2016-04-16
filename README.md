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
  

# Dev Log
Update 2016-Apr-14 <br />
  - Successfully set up SPI communication between 9S12 Master with Arduino Uno slave <br />
  - Successfully sent out one char to slave without fail <br />
  - Future Revision Needed ( BaudRate Division, adding more slave, map 4 digit 7 segmented display) <br />
 
