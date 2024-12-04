#include <Arduino.h>
#include <Wire.h>

#include <config.cpp>

static const int NUM_REGISTERS = 256;
int reg = 0;
byte regbuffer[NUM_REGISTERS];

// time from last measurement
unsigned long lastLoop = MEASUREMENT_INTERVAL_S;

int16_t humidity1[N_MEAS] = {0};
uint16_t humidity1Avg = 0;
int16_t humidity2[N_MEAS] = {0};
uint16_t humidity2Avg = 0;
int16_t waterlevel[N_MEAS] = {0};
uint16_t waterlevelAvg = 0;

void receiveEvent(int len);
void sendEvent();

/**
 * Calculate average of data
 *
 * @param data Data to calculate average
 * @param type Type of average: 0 - all data, 1 - only > -1
 */

uint16_t calculateAverage(int16_t data[], uint8_t type);


void setup() {
  Wire.begin(0x50);              // join I2C bus with address #8
  Wire.onReceive(receiveEvent);  // register event
  Wire.onRequest(sendEvent);

#ifdef DEBUG
  Serial.begin(115200);  // start serial for output
#endif
}

void loop() {
  if (millis() - lastLoop > MEASUREMENT_INTERVAL_S * 1000) {
    // Read sensors
    for (uint8_t i = 0; i < N_MEAS; i++) {
      humidity1[i] = analogRead(HUMIDITY1_PIN);
      humidity2[i] = analogRead(HUMIDITY2_PIN);
      waterlevel[i] = analogRead(WATERLEVEL_PIN);
      delay(10);
    }

    // Calculate average from all measurements
    humidity1Avg = calculateAverage(humidity1, 0);
    humidity2Avg = calculateAverage(humidity2, 0);
    waterlevelAvg = calculateAverage(waterlevel, 0);

    // check if some value is out of range
    // set value to -1 if it is out of range
    for (uint8_t i = 0; i < N_MEAS; i++) {
      if (humidity1[i] > humidity1Avg * 1.2 ||
          humidity1[i] < humidity1Avg * 0.8) {
        humidity1[i] = -1;
      }
      
      if (humidity2[i] > humidity2Avg * 1.2 ||
          humidity2[i] < humidity2Avg * 0.8) {
        humidity2[i] = -1;
      }

      if (waterlevel[i] > waterlevelAvg * 1.2 ||
          waterlevel[i] < waterlevelAvg * 0.8) {
        waterlevel[i] = -1;
      }
    }

    // Calculate average again, but do not include out of range values
    humidity1Avg = calculateAverage(humidity1, 1);
    humidity2Avg = calculateAverage(humidity2, 1);
    waterlevelAvg = calculateAverage(waterlevel, 1);

    #ifdef DEBUG
      Serial.print("Humidity1Avg: " + String(humidity1Avg) + ", 0x");
      Serial.println(humidity1Avg, HEX);
      Serial.print("Humidity2Avg: " + String(humidity2Avg) + ", 0x");
      Serial.println(humidity2Avg, HEX);
      Serial.print("WaterlevelAvg: " + String(waterlevelAvg) + ", 0x");
      Serial.println(waterlevelAvg, HEX);
    #endif
    

    lastLoop = millis();
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int len) {
  if (len == 1) {  // One Byte Data received -> Read Request Address
    reg = Wire.read();
    Serial.print("Read Request Address: 0x");
    Serial.println(reg, HEX);
  } else {
    reg = 0;
    byte rx = Wire.read();
    delayMicroseconds(20);
    while (Wire.available() > 0) {
      rx %= sizeof(regbuffer);
      regbuffer[rx] =
          Wire.read();  // pull in the latest byte of data and process it
      if (rx == CONFIG_REG)
        Serial.println("To reg buff CONFIG_REG was written: " +
                       String(regbuffer[CONFIG_REG]));
      rx++;
    }
  }
}

void sendEvent() {
  uint16_t c = 0;
  delayMicroseconds(20);
  if (reg == HUMIDITY1_REG) {
    c = humidity1Avg;
  } else if (reg == HUMIDITY2_REG) {
    c = humidity2Avg;
  } else if (reg == WATERLEVEL_REG) {
    c = waterlevelAvg;
  }
  // c = 0x05;
  Wire.write(c);
}

uint16_t calculateAverage(int16_t data[], uint8_t type) {
  uint16_t avg = 0;
  uint16_t n_data = 0;

  for (uint8_t i = 0; i < (N_MEAS); i++) {
    if (type == 1 && data[i] > 0) { // if type == 1, calculate average of data > -1
      avg += data[i];
      n_data++;
    } else if (type == 0) {   // if type == 0, calculate average of all data
      avg += data[i];
      n_data++;
    }
  }
  avg /= n_data;

  return avg;
}


