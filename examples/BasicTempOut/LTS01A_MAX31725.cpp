#include "LTS01A_MAX31725.h"
#include "Wire.h"


MAX31725::MAX31725(){
  Serial.print("Starte leeren Konstruktor"); 
  _i2cAddress =  MAX31725_I2C_ADDRESS;
}

void MAX31725::begin(int sdapin ,int sclpin){
   _i2cAddress =  MAX31725_I2C_ADDRESS;
  Wire.begin(sdapin, sclpin); 
  }
  
void MAX31725::begin(uint8_t i2cAddress, int sdapin ,int sclpin){
   _i2cAddress =  i2cAddress;
  Wire.begin(sdapin, sclpin); 
  } 

float MAX31725::read_temperature(){
    Wire.beginTransmission(_i2cAddress);   
    Wire.write(0x00);              
    Wire.endTransmission();
    Wire.requestFrom(_i2cAddress, 2);     
    byte msb = Wire.read();        
    //Serial.println(msb, BIN);
    byte lsb = Wire.read();        
    //Serial.println(lsb, BIN);
    float temp = msb;
    int16_t t = msb << 8 | lsb;  
    temp = t * 0.00390625;       
    return temp;
}


// Funktion 2 für THYST in Grad Celsius lesen  
float MAX31725::read_thyst(){
  Wire.beginTransmission(_i2cAddress);   
  Wire.write(0x02);              
  Wire.endTransmission();        
  Wire.requestFrom(_i2cAddress, 2);      
  byte msb = Wire.read();        
  //Serial.println(msb, BIN);
  byte lsb = Wire.read();        
  //Serial.println(lsb, BIN);     
  float thyst = msb;
  int16_t t = msb << 8 | lsb;   
  thyst = t * 0.00390625;        
  return thyst;
}

// Funktion 3 für TOS in Grad Celsius lesen  
float MAX31725::read_tos(){
  Wire.beginTransmission(_i2cAddress);   
  Wire.write(0x03);             
  Wire.endTransmission();        
  Wire.requestFrom(_i2cAddress, 2);    
  byte msb = Wire.read();        
  //Serial.println(msb, BIN);
  byte lsb = Wire.read();        
  //Serial.println(lsb, BIN);     
  float tos = msb;
  int16_t t = msb << 8 | lsb;  
  tos = t * 0.00390625;      
  return tos;
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// THYST und TOS setzen 
//

// Funktion 4 für THYST in Grad Celsius setzen
void MAX31725::write_thyst(byte msb, byte lsb){
  Wire.beginTransmission(_i2cAddress);   
  Wire.write(0x02);              
  Wire.write(msb);              
  Wire.write(lsb);              
  Wire.endTransmission();        
}

// Funktion 5 für TOS in Grad Celsius setzen
void MAX31725::write_tos( byte msb, byte lsb){
  Wire.beginTransmission(_i2cAddress);  
  Wire.write(0x03);              
  Wire.write(msb);             
  Wire.write(lsb);              
  Wire.endTransmission();         
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Config Register printen, lesen, setzen  
//

//Funktion 6 für Config Register Byte lesen 
byte MAX31725::get_config(){
  Wire.beginTransmission(_i2cAddress);    
  Wire.write(0x01); 
  Wire.endTransmission();        
  Wire.requestFrom(_i2cAddress, 1);
  byte data = Wire.read();
  return data; 
}

// Funktion 7 für Config Register printen 
void MAX31725::print_config(){
  Wire.beginTransmission(_i2cAddress);   
  Wire.write(0x01); 
  Wire.endTransmission();        
  Wire.requestFrom(_i2cAddress, 1);
  byte data = Wire.read();

  Serial.printf("Config in Bit:");
  Serial.print(data, BIN);
  Serial.printf("\n");
  if(data - 127 > 0 ){ Serial.println("1 One-Shot: aktiv"); data = data - 128;} 
    else { Serial.println("0 One-Shot: deaktiv"); }
  if(data - 63 > 0 ){ Serial.println("1 Timeout: deaktiv"); data = data - 64;} 
    else { Serial.println("0 Timeout: aktiv"); }
  if(data - 31 > 0 ){ Serial.println("1 Data Format: Extended"); data = data - 32;} 
    else { Serial.println("0 Data Format: Standard"); }
  if(data - 15 > 0 ){ Serial.println("1 Fault Queue[1]: 1"); data = data - 16;} 
    else { Serial.println("0 Fault Queue[1]: 0"); }
  if(data - 7 > 0 ){ Serial.println("1 Fault Queue[0]: 1"); data = data - 8;} 
    else { Serial.println("0 Fault Queue[0]: 0"); }   
  if(data - 3 > 0 ){ Serial.println("1 OS Polarity: Active High"); data = data - 4;} 
    else { Serial.println("0 OS Polarity: Active Low"); } 
  if(data - 1 > 0 ){ Serial.println("1 Interrupt Mode"); data = data - 2;} 
    else { Serial.println("0 Comparator Mode"); }
  if(data == 1 ){ Serial.println("1 Shutdown Mode: Aktiv"); data = data - 1;} 
    else { Serial.println("0 Shutdown Mode: Deaktiv"); }       
}

// Funktion 8 für Config Register Byte setzen
void MAX31725::write_config( byte data){
  Wire.beginTransmission(_i2cAddress);    
  Wire.write(0x01);              
  Wire.write(data);
  Wire.endTransmission();        
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration OneShot lesen, setzen    
//

//Funktion 9 für Config One Shot lesen 
bool MAX31725::getConfig_OneShot(){
  byte data =  get_config();
  if(data - 127 > 0 ){ return true;} 
    else { return false; }       
}

//Funktion 10 für Config One Shot setzen
void MAX31725::setConfig_OneShot(bool active){
  bool configOneShot = getConfig_OneShot(); 
  if( configOneShot == active){}
  else {
      byte data = get_config();
      if(configOneShot == false && active == true){data=data+128;}
      else {data=data-128;} 
      write_config(data);  
    }       
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration Timeout lesen, setzen    
//

//Funktion 11 für Config Timeout lesen  
bool MAX31725::getConfig_Timeout(){
  byte data =  get_config();
   if(data - 127 > 0 ){data = data - 128;} else {}
   
   if(data - 63 > 0 ){return false;} 
    else { return true;}        
}

//Funktion 12 für Config Timeout setzen
void MAX31725::setConfig_Timeout(bool enabled){
  bool configTimeout = getConfig_Timeout(); 
  if( configTimeout == enabled){}
  else {
      byte data = get_config();
      if(configTimeout == false && enabled == true){data=data-64;}
      else {data=data+64;} 
      write_config(data);  
    }       
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration ExtendedDataFormat lesen, setzen    
//

//Funktion 13 für Config ExtendedDataFormat lesen
bool MAX31725::getConfig_ExtendedDataFormat(){
  byte data =  get_config();
   if(data - 127 > 0 ){data = data - 128;} else {}
   if(data - 63 > 0 ){data = data - 64;} else {}

     if(data - 31 > 0 ){return true;} 
    else { return false;}           
}

//Funktion 14 für Config ExtendedDataFormat setzen
void MAX31725::setConfig_ExtendedDataFormat(bool enabled){
  bool configExtended = getConfig_ExtendedDataFormat(); 
  if( configExtended == enabled){}
  else {
      byte data = get_config();
      if(configExtended == false && enabled == true){data=data+32;}
      else {data=data-32;} 
      write_config(data);  
    }       
}


////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration FaultsQueue lesen, setzen    
//

//Funktion 15 für Config FaultsQueue lesen 
int MAX31725::getConfig_FaultsQueue(){
  byte data =  get_config();
   if(data - 127 > 0 ){data = data - 128;} else {}
   if(data - 63 > 0 ){data = data - 64;} else {}
   if(data - 31 > 0 ){data = data - 32;} else {}

   boolean faultQueue1; 
     if(data - 15 > 0 ){faultQueue1 = true; data = data - 16;} 
    else { faultQueue1 = false;}
   boolean faultQueue0; 
     if(data - 7 > 0 ){faultQueue0 = true;} 
    else { faultQueue0 = false;}

    if(faultQueue1 == faultQueue0 ){ if(faultQueue1 == true){return 6;} else {return 1;}}
    else { if(faultQueue1 == true){return 4;} else {return 2;} }  
}

//Funktion 16 für Config FaultsQueue setzen
void MAX31725::setConfig_FaultsQueue(int faults){
  if( (faults > 6 && faults < 1) || faults == 3 || faults == 5 ) {Serial.printf("\n Err: Use defined Fault Queue \n"); return;}
  int configFaults = getConfig_FaultsQueue(); 
  if(configFaults == faults){}
  else {
      int zw; 
      if(configFaults==1)zw = 0;
      if(configFaults==2)zw = 8;
      if(configFaults==4)zw = 16;
      if(configFaults==6)zw = 24; 
      
      byte data = get_config();
      write_config(data - zw); 
      
      if(faults == 1){ write_config(data + 0); }
      if(faults == 2){ write_config(data + 8); }
      if(faults == 4){ write_config(data + 16); }
      if(faults == 6){ write_config(data + 24); }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration OSPolarity lesen, setzen    
//

//Funktion 17 für Config OSPolarity lesen  
bool MAX31725::getConfig_OSPolarity(){
  byte data =  get_config();
   if(data - 127 > 0 ){data = data - 128;} else {}
   if(data - 63 > 0 ){data = data - 64;} else {}
   if(data - 31 > 0 ){data = data - 32;} else {}
   if(data - 15 > 0 ){data = data - 16;} else {}
   if(data - 7 > 0 ){data = data - 8;} else {}
  
    if(data - 3 > 0 ){ return true;} 
    else { return false; }       
}

//Funktion 18 für Config OSPolarity setzen 
void MAX31725::setConfig_OSPolarity(bool active_high){
  bool configOSPolarity = getConfig_OSPolarity(); 
  if( configOSPolarity == active_high){}
  else {
      byte data = get_config();
      if(configOSPolarity == false && active_high == true){data=data+4;}
      else {data=data-4;} 
      write_config(data);  
    }       
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration InterruptMode lesen, setzen    
//

//Funktion 19 für Config InterruptMode lesen 
bool MAX31725::getConfig_InterruptMode(){
  byte data =  get_config();
   if(data - 127 > 0 ){data = data - 128;} else {}
   if(data - 63 > 0 ){data = data - 64;} else {}
   if(data - 31 > 0 ){data = data - 32;} else {}
   if(data - 15 > 0 ){data = data - 16;} else {}
   if(data - 7 > 0 ){data = data - 8;} else {}
   if(data - 3 > 0 ){data = data - 4;} else {}
    if(data - 1 > 0 ){ return true;} 
    else { return false; }       
}

//Funktion 20 für Config InterruptMode setzen
void MAX31725::setConfig_InterruptMode(bool interrupt_active){
  bool configInterruptMode = getConfig_InterruptMode(); 
  if( configInterruptMode == interrupt_active){}
  else {
      byte data = get_config();
      if(configInterruptMode == false && interrupt_active == true){data=data+2;}
      else {data=data-2;} 
      write_config(data);  
    }       
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Konfiguration ShutdownMode lesen, setzen    
//

//Funktion 21 für Config ShutdownMode lesen 
bool MAX31725::getConfig_ShutdownMode(){
  byte data =  get_config();
   if(data - 127 > 0 ){data = data - 128;} else {}
   if(data - 63 > 0 ){data = data - 64;} else {}
   if(data - 31 > 0 ){data = data - 32;} else {}
   if(data - 15 > 0 ){data = data - 16;} else {}
   if(data - 7 > 0 ){data = data - 8;} else {}
   if(data - 3 > 0 ){data = data - 4;} else {}
   if(data - 1 > 0 ){data = data - 2;} else {}
    if(data == 1 ){ return true;} 
    else { return false; }       
}

//Funktion 21 für Config ShutdownMode setzen 
void MAX31725::setConfig_ShutdownMode(bool shutdown_active){
  bool configShutdownMode = getConfig_ShutdownMode(); 
  if( configShutdownMode == shutdown_active){}
  else {
      byte data = get_config();
      if(configShutdownMode == false && shutdown_active == true){data=data+1;}
      else {data=data-1;} 
      write_config(data);  
    }       
}
