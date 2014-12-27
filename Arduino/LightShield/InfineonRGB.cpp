  // Infineon RGB_LED_Shield_Master_Tester
 // by Michelle Chia
 // Refactored to Arduino Style by Mohan Palanisamy
  // Demonstrates I2C communication with the RGB LED Shield for safe configuration. Tested at 48Vin, 6V forward voltage LED. LED current up to 350mA.
  // Modified 13 August 2014 
  // Modified 20 Decemeber 2014
 
 #include "InfineonRGB.h"
 #include <Wire.h>
 
 
 InfineonRGB::InfineonRGB()
 {
 
 }
 
 boolean InfineonRGB::begin(void)
 {
        I2CDMX (ADDRESS, DMXOFF); // disable DMX
      write2bytes (ADDRESS, FADERATE, 0x0000); // Immediate fade
        write2bytes (ADDRESS, DIMMINGLEVEL, 4095); // FFF=100% brightness level
 }
 void InfineonRGB::setRed(unsigned int redIntensity)
 {
   write2bytes (ADDRESS, INTENSITY_RED, redIntensity);
 }
 
 void InfineonRGB::setGreen(unsigned int greebIntensity)
 {
   write2bytes (ADDRESS, INTENSITY_GREEN, greebIntensity);
 }
 
 void InfineonRGB::setBlue(unsigned int blueIntensity)
 {
   write2bytes (ADDRESS, INTENSITY_BLUE, blueIntensity);
 }
 
 void InfineonRGB::setBrightnessLevel(unsigned int percent)
 {
   write2bytes(ADDRESS, DIMMINGLEVEL, 4095 * percent /100);
 }
 
 void InfineonRGB::setRGB(unsigned int r,unsigned int g,unsigned int b)
 {
   write6bytes(ADDRESS,INTENSITY_RGB,r,g,b);
 }
 
 
 /*
  Parameters (IN): int Address - Address of RGB LED Shield, Default 0x15E
                   int Command - Defined I2C Commands i.e. INTENSITY_RED, INTENSITY_GREEN, INTENSITY_BLUE
                   unsigned int Data - 16bit data to be written to slave
  Parameters (OUT): None
  Return Value: None
  Description: This function will write 2 bytes of word to the I2C bus line
  */
  void InfineonRGB::write2bytes (int Address, int Command, unsigned int Data)
  {
    unsigned int upperByte, lowerByte; // Separate 4 byte data into 2 byte values
    lowerByte = Data;
    upperByte = Data >> 8;
    
    unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
    unsigned int upperSLAD = Address >> 8;
    upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write
    
    Wire.beginTransmission(byte(upperSLAD)); // Start I2C transmission
    Wire.write(byte(lowerSLAD)); // address lower 8 bits of i2c address
    Wire.write(byte(Command)); // write command
    Wire.write(byte(upperByte)); // write data
    Wire.write(byte(lowerByte));
    Wire.endTransmission(true);
  } 
  
    /*
  Parameters (IN): int Address - Address of RGB LED Shield, Default 0x15E
                   int Command - Defined I2C Commands i.e. INTENSITY_RGB, CURRENT_RGB
                   unsigned int DataOne, unsigned int DataTwo, unsigned int DataThree - Three 16bit data to be written to slave
  Parameters (OUT): None
  Return Value: None
  Description: This function will write 6 bytes of word to the I2C bus line
  */
  
  void InfineonRGB::write6bytes (unsigned int Address, unsigned int Command, unsigned int DataOne, unsigned int DataTwo, unsigned int DataThree) // DataOne: Red, DataTwo: Green, DataThree: Blue
  {
    unsigned int upperByte, lowerByte; // Split each Data parameter into upper and lower 8 bytes because I2C format sends 8 bytes of data each time
    lowerByte = DataOne;
    upperByte = DataOne >> 8;
    
    unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF);
    unsigned int upperSLAD = Address >> 8;
    upperSLAD |= 0x79; // First 5 bits 11110 and last bit '1' for a write
    
    Wire.beginTransmission(byte(upperSLAD)); // Red
    Wire.write(byte(lowerSLAD));
    Wire.write(byte(Command));
    Wire.write(byte(upperByte));
    Wire.write(byte(lowerByte));
    lowerByte = DataTwo;
    upperByte = DataTwo >> 8;
    Wire.write(byte(upperByte));
    Wire.write(byte(lowerByte));
    lowerByte = DataThree;
    upperByte = DataThree >> 8;
    Wire.write(byte(upperByte));
    Wire.write(byte(lowerByte));
    Wire.endTransmission(true);
      
  }
  
  void InfineonRGB::I2CDMX (unsigned int Address, unsigned int Command) // Switch off / on the DMX
  { 
    unsigned int lowerSLAD = (unsigned int) (Address & 0x00FF); // Putting address into correct format
    unsigned int upperSLAD = Address >> 8;
    upperSLAD |= 0x79;
    
    Wire.beginTransmission(byte(upperSLAD)); // Start I2C transmission
    Wire.write(byte(lowerSLAD));
    Wire.write(byte(Command)); 
    Wire.endTransmission(true);
  
  }
 
