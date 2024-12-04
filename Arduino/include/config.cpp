#include <Arduino.h>

//Register keys
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

#define MEASUREMENT_INTERVAL_S 30    // Interval between measurements in seconds
#define N_MEAS 10                   // Number of measurements
#define OUT_OF_RANGE_COEFF 0.2      // Coefficient for out of range values; 1-OUT_OF_RANGE_COEFF < value < 1+OUT_OF_RANGE_COEFF


typedef struct {
  int16_t values[N_MEAS];
  uint16_t average;
  uint16_t tresholds[2];
  uint8_t percentage;
} measured_value;

measured_value humidity1  = {{0}, 0, {0, 1023}, 0};
measured_value humidity2  = {{0}, 0, {0, 1023}, 0};
measured_value waterlevel = {{0}, 0, {0, 1023}, 0};

