#ifndef MAX31725_h
#define MAX31725_h

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgramm.h"
#endif

#define MAX31725_I2C_ADDRESS    0x48

class MAX31725
{
private:     
    int _i2cAddress;

public:
    // Konstruktor
    MAX31725();

    //Methoden
    void begin(int sdapin, int sclpin); 
    void begin(uint8_t i2cAddress, int sdapin, int sclpin); 
    
    float read_temperature();
    float read_thyst();
    float read_tos();

    void write_thyst( byte msb, byte lsb);
    void write_tos(byte msb, byte lsb);

    byte get_config();
    void print_config();
    void write_config(byte data);

    bool getConfig_OneShot();
    void setConfig_OneShot(bool active);

    bool getConfig_Timeout();
    void setConfig_Timeout(bool enabled);

    bool getConfig_ExtendedDataFormat();
    void setConfig_ExtendedDataFormat( bool enabled);

    int getConfig_FaultsQueue();
    void setConfig_FaultsQueue(int faults);

    bool getConfig_OSPolarity();
    void setConfig_OSPolarity( bool active_high);

    bool getConfig_InterruptMode();
    void setConfig_InterruptMode( bool interrupt_active);

    bool getConfig_ShutdownMode();
    void setConfig_ShutdownMode(bool shutdown_active);
};
#endif
