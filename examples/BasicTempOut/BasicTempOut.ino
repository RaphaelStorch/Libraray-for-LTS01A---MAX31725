//LibTest.ino
#include "LTS01A_MAX31725.h"

const int sdapin = 23;  
const int sclpin = 19; 

MAX31725 MAX31725; 

void setup() {
  Serial.begin(115200);
  Serial.printf("Starte MAX31725 Library Test\n");
  delay(50); 
  MAX31725.begin(0x48, sdapin, sclpin); 
}


void loop() {
  float temperatur;
  temperatur = MAX31725.read_temperature();
  Serial.printf("Temp:%.8f°C\n", temperatur);
  
  delay(2000);
  
  //max31725.print_config();
}
