//LibTest.ino
#include "LTS01A_MAX31725.h"

const int sdapin = 23;  // default 21;
const int sclpin = 19;  // default 22;

MAX31725 max31725; 

void setup() {
  Serial.begin(115200);
  Serial.printf("Starte MAX31725 Library Test\n");
  delay(50); 
  max31725.begin(0x48, sdapin, sclpin); 
}


void loop() {
  float temperatur;
  temperatur = max31725.read_temperature();
  Serial.printf("\n Temp:%.8f°C\n\n", temperatur);
  delay(1000);

  float thyst;
  thyst = max31725.read_thyst();
  Serial.printf("Thyst Temp:%.8f°C\n\n", thyst);
  delay(1000);
  
  float tos;
  tos = max31725.read_tos();
  Serial.printf("TOS Temp:%.8f°C\n\n", tos);
  delay(1000);

  max31725.write_thyst(80, 0); 
  max31725.write_tos(85, 0); 

  max31725.print_config();
  
  //max31725.write_config(0);

  bool cm1 = max31725.getConfig_OneShot();
  max31725.setConfig_OneShot(true); 

  bool cm2 = max31725.getConfig_Timeout();
  max31725.setConfig_Timeout(false);

  bool cm3 = max31725.getConfig_ExtendedDataFormat();
  max31725.setConfig_ExtendedDataFormat(false);

  bool cm4 = max31725.getConfig_FaultsQueue();
  max31725.setConfig_FaultsQueue( 6);   

  bool cm5 = max31725.getConfig_OSPolarity();
  max31725.setConfig_OSPolarity(true);
  
  bool cm6 = max31725.getConfig_InterruptMode();
  max31725.setConfig_InterruptMode( true); 

  bool cm7 = max31725.getConfig_ShutdownMode();
  max31725.setConfig_ShutdownMode(false); 
   
}
