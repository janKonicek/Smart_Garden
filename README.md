# Smart Garden

A simple sketch of this project.
![How it works](Docs/images/image.png)

## Arduino
Arduino works as I2C slave and measures soil moisture using two analogue sensors, water level (analog) and can also control water valve (digital output). 

Pins, registers, tresholds can be easily changed in [config.cpp](/Arduino/include/config.cpp).

## Nordic
Currently I am using nRF52840 devkit. The board is reading data from arduino every 30 seconds. 

It also have several zigbee clusters:
 - basic
 - indentify
 - on_off (water valve)
 - analog_output (water level)
 - soil_moisture (modified cluster with two values)


## Zigbee2mqtt
[garden.js](/Zigbee2mqtt/garden.js) is a converter of this device for Zigbee2mqtt. 


## Video
[Video](https://photos.app.goo.gl/bV1H2QtREhb4gFk89)

## References
[mak3r - I2C Arduino Slave](https://github.com/mak3r/i2c-arduino-slave/tree/main)

[Zigbee2MQTT](https://www.zigbee2mqtt.io)

[nobodyguy - Zigbee air quality sensor](https://github.com/nobodyguy/zigbee_air_quality_monitor_firmware/)

[rbaron - b-parasite (Soil moisture sensor)](https://github.com/rbaron/b-parasite/)
