 // Infineon RGB_LED_Shield_Master_Tester
 // by Michelle Chia
 // Refactored to Arduino Style by Mohan Palanisamy
  // Demonstrates I2C communication with the RGB LED Shield for safe configuration. Tested at 48Vin, 6V forward voltage LED. LED current up to 350mA.
  // Modified 13 August 2014 
  // Modified 20 Decemeber 2014
  #include <Arduino.h>
  
  #define ADDRESS                    0x15EUL
  
  #define INTENSITY_RED              0x11U
  #define INTENSITY_GREEN            0x12U
  #define INTENSITY_BLUE	     0x13U
  #define INTENSITY_RGB              0x14U
  #define CURRENT_RED                0x21U
  #define CURRENT_GREEN              0x22U
  #define CURRENT_BLUE               0x23U
  #define CURRENT_RGB                0x24U
  #define DMXOFF		     0x30U
  #define DMXON             	     0x31U
  #define DMXSLOT		     0x32U
  #define DMX8BIT                    0x33U
  #define DMX16BIT                   0x34U
  #define OFFTIME_RED                0x41U
  #define OFFTIME_GREEN              0x42U
  #define OFFTIME_BLUE		     0x43U
  #define WALKTIME                   0x50U
  #define DIMMINGLEVEL               0x60U
  #define FADERATE                   0x61U
  #define _CHANGE_ADDRESS            0x70U
 
  #define READ_INTENSITY_RED         0x81U
  #define READ_INTENSITY_GREEN       0x82U
  #define READ_INTENSITY_BLUE        0x83U
  #define READ_CURRENT_RED           0x84U
  #define READ_CURRENT_GREEN         0x85U
  #define READ_CURRENT_BLUE          0x86U
  #define READ_OFFTIME_RED           0x87U
  #define READ_OFFTIME_GREEN         0x88U
  #define READ_OFFTIME_BLUE          0x89U
  #define READ_WALKTIME              0x8AU
  #define READ_DIMMINGLEVEL          0x8BU
  #define READ_FADERATE              0x8CU
  #define DIRECTACCESS_READ          0x90U // read twice
  #define DIRECTACCESS_MOVE   	     0x91U
  #define DIRECTACCESS_AND           0x92U
  #define DIRECTACCESS_OR            0x93U
  
  #define SAVEPARAMETERS             0xA0U
  
  #define BCCUMODID                  0x50030008U
  #define CHIPID                     0x40010004U
  #define REDINTS                    0x500300A0U // BCCU_CH5
  #define REDINT                     0x500300A4U
  #define BLUEINTS                   0x50030078U 
  #define STARTWALK                  0x50030018U
  

 class InfineonRGB {
   public:
      InfineonRGB(void);
      
      boolean begin(void);

      //Set Colors
      void setRed(unsigned int r);
      void setGreen(unsigned int g);
      void setBlue(unsigned int b);
      void setRGB(unsigned int r,unsigned int g,unsigned int b);
      void setBrightnessLevel(unsigned int percent);
      
    private: 
      void write2bytes (int Address, int Command, unsigned int Data);
      void write6bytes (unsigned int Address, unsigned int Command, unsigned int DataOne, unsigned int DataTwo, unsigned int DataThree) ;// DataOne: Red, DataTwo: Green, DataThree: Blue
      void I2CDMX(unsigned int Address, unsigned int Command);
  };
