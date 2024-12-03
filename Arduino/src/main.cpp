#include <Arduino.h>
#include <Wire.h>

//Register keys
//#define CONTROL_REG       0x03 // Control how eeprom is used
#define CONFIG_REG        0x01  // Configuration of the device
#define HUMIDITY1_REG     0x10  // Humidity 1 data
#define HUMIDITY2_REG     0x11  // Humidity 2 data
#define WATERLEVEL_REG    0x20  // Water level data
#define WATERVALVE_REG    0x30  // Water valve data

// Pin definitions
#define HUMIDITY1_PIN     A0
#define HUMIDITY2_PIN     A1
#define WATERLEVEL_PIN    A2
#define WATERVALVE_PIN    3


static const int NUM_REGISTERS=256;
int reg = 0;
byte regbuffer[NUM_REGISTERS];


void receiveEvent(int len);
void sendEvent();

void setup() {
  Wire.begin(0x50);                // join I2C bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(sendEvent);

  #ifdef DEBUG 
    Serial.begin(115200);           // start serial for output
  #endif
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int len) {

  if(len == 1){ // One Byte Data received -> Read Request Address
    reg = Wire.read();
    Serial.print("Read Request Address: 0x");
    Serial.println(reg, HEX);
  } else {
    reg = 0;
    byte rx = Wire.read();
    delayMicroseconds(20);
    while(Wire.available() > 0){
      rx %= sizeof(regbuffer);
      regbuffer[rx] = Wire.read();  //pull in the latest byte of data and process it
      if (rx == CONFIG_REG)
        Serial.println("To reg buff CONFIG_REG was written: " + String(regbuffer[CONFIG_REG]));
      rx++;
    }
  }
  
}


void sendEvent(){
  int p = reg % sizeof(regbuffer); 
  byte c;
  delayMicroseconds(20);
  if (reg == HUMIDITY1_REG){
    c = 0x05;
  } else if(reg == HUMIDITY2_REG) {
    c = 0x77;
  }
  
  //c = 0x05;
  Wire.write(c);
}
