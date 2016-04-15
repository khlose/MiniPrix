# MiniPrix
ECE362 Mini Project

# Current set up

  9S12 Side
    - Port M 2 = MISO
    - Port M 3 = Slave Select Arduino 1
    - Port M 4 = MOSI
    - Port M 5 = SCK
    - Pin 4, 31 = GND
    - Pin 29 = Regulated 5 V
      
  Arduino Side
    - Pin 13 = SCK
    - Pin 12 = MISO
    - Pin 11 = MOSI
    - Pin 10 = Slave Select
    All ground connected  to Master's Ground
  

# Dev Log
Update 2016-Apr-14
  - Successfully set up SPI communication between 9S12 Master with Arduino Uno slave
  - Successfully sent out one char to slave without fail
  - Future Revision Needed ( BaudRate Division, adding more slave, map 4 digit 7 segmented display)
 
