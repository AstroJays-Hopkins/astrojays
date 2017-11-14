# Astrojays Avionics

This repo contains the files for the avionics subsystem of the Astrojays Rocket.
## Sensors used:
* [Adafruit Ultimate GPS](https://www.adafruit.com/product/746)
* [Parallax Altimeter Module MS5607](https://www.parallax.com/product/29124)
* [Adafruit ADXL377 - High-G Triple-Axis Accelerometer](https://www.adafruit.com/product/1413)
* [Adafruit 9-DOF BNO055 IMU](https://www.adafruit.com/product/2472)
## Installation:
Simply download the zip and/or ```git clone``` and open the `.ino` file in the Arduino IDE.


## Pinouts
Pin|Sensor|Function|Analog/Digital
---|---|---|---
A1 | ADXL377 | x-acceleration|Analog
A2 | ADXL377 | y-acceleration|Analog
A3 | ADXL377 | z-acceleration|Analog
A4 | All but ADXL377 | SDA | Digital
A5 | All but ADXL377 | SCL | Digital
1 | - | To recovery system | Digital