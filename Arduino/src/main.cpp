#include <Arduino.h>
#include <Wire.h>

#include <config.cpp>

static const int NUM_REGISTERS = 256;
int reg = 0;
byte regbuffer[NUM_REGISTERS];

// time from last measurement
unsigned long lastLoop = MEASUREMENT_INTERVAL_S;

void receiveEvent(int len);
void sendEvent();

/**
 * Calculate average of data
 *
 * @param data Data to calculate average
 * @param type Type of average: 0 - all data, 1 - only > -1
 * @return Average of data
 */
uint16_t calculateAverage(int16_t data[], uint8_t type);

/**
 * Calculate percentage of value
 * 
 * @param value Value to calculate percentage
 * @param tresholds Tresholds for 100% and 0%
 * @return Percentage of value
 */
uint8_t calculatePercentage(uint16_t value, uint16_t tresholds[]);


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
      humidity1.values[i] = analogRead(HUMIDITY1_PIN);
      humidity2.values[i] = analogRead(HUMIDITY2_PIN);
      waterlevel.values[i] = analogRead(WATERLEVEL_PIN);
      delay(10);
    }

    // Calculate average from all measurements
    humidity1.average = calculateAverage(humidity1.values, 0);
    humidity2.average = calculateAverage(humidity2.values, 0);
    waterlevel.average = calculateAverage(waterlevel.values, 0);

    // check if some value is out of range
    // set value to -1 if it is out of range
    for (uint8_t i = 0; i < N_MEAS; i++) {
      if (humidity1.values[i] > humidity1.average * 1.2 ||
          humidity1.values[i] < humidity1.average * 0.8) {
        humidity1.values[i] = -1;
      }

      if (humidity2.values[i] > humidity2.average * 1.2 ||
          humidity2.values[i] < humidity2.average * 0.8) {
        humidity2.values[i] = -1;
      }

      if (waterlevel.values[i] > waterlevel.average * 1.2 ||
          waterlevel.values[i] < waterlevel.average * 0.8) {
        waterlevel.values[i] = -1;
      }
    }

    // Calculate average again, but do not include out of range values
    humidity1.average = calculateAverage(humidity1.values, 1);
    humidity2.average = calculateAverage(humidity2.values, 1);
    waterlevel.average = calculateAverage(waterlevel.values, 1);

    // Calculate percentage of values
    humidity1.percentage = calculatePercentage(humidity1.average, humidity1.tresholds);
    humidity2.percentage = calculatePercentage(humidity2.average, humidity2.tresholds);
    waterlevel.percentage = calculatePercentage(waterlevel.average, waterlevel.tresholds);

    // Print values
    #ifdef DEBUG
      Serial.println("===== MEASUREMENTS =====");
      Serial.print("Humidity1Avg:  " + String(humidity1.average)  + ", Percentage: " + String(humidity1.percentage)  + ", 0x");
      Serial.println(humidity1.percentage, HEX);
      Serial.print("Humidity2Avg:  " + String(humidity2.average)  + ", Percentage: " + String(humidity2.percentage)  + ", 0x");
      Serial.println(humidity2.percentage, HEX);
      Serial.print("WaterlevelAvg: " + String(waterlevel.average) + ", Percentage: " + String(waterlevel.percentage) + ", 0x");
      Serial.println(waterlevel.percentage, HEX);
      Serial.println("=========================");
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
  uint16_t res = 0;
  delayMicroseconds(20);
  if (reg == HUMIDITY1_REG) {
    res = humidity1.percentage;
  } else if (reg == HUMIDITY2_REG) {
    res = humidity2.percentage;
  } else if (reg == WATERLEVEL_REG) {
    res = waterlevel.percentage;
  }
  Wire.write(res);
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

uint8_t calculatePercentage(uint16_t value, uint16_t tresholds[]) {
  float percentage = 0;
  if (value < tresholds[0]) {
    percentage = 0;
  } else if (value > tresholds[1]) {
    percentage = 100;
  } else {
    percentage = float(value - tresholds[0]) / float(1023 - tresholds[0]) * 100; // percentage * 100
  }

  return uint8_t(percentage);
}


