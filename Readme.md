# Interface lab 2020

This repo documents best hardware practices using Unity and Arduino. These examples were developed to explain useful ways that Arduino and Unity can communicate with each other.

We used Arduino Nano 33 IoT, and instructions, blueprints and video tutorials are hosted at https://medium.com/interface-lab

#


## Arduino Code
**Prototype 0** From Arduino to Unity, using one digital input

**Prototype 1** From Arduino to Unity, using many sensors

**Prototype 2** From Unity to Arduino

**Prototype 3** Arduino and Unity talking to each other (Full Duplex)


#

## Unity package

You should use the option **Assets / Import Package...** in order to decompress and install this file.

#

## Notes

The serial communication needs to control the flow of information. One option is to use the Unity package to request an update, what showed to be stable and reliable on Windows computers. Optionally, you can try to use a delay in Arduino as in the example provided in Prototype 0.

**Software used:** Unity version 2019.3.14f1, [Ardity] (https://github.com/dwilches/Ardity)

**OS tested:** macOS Mojave 10.14.6 and Windows 10 Pro 64bits

**Microcontrollers tested:** Arduino UNO, Arduino Nano IOT 33, ESP8266 NodeMCU, ESP32

**Hardware tested:** IMU BNO-055, Neopixels WS2812, servoMotors, LEDs, Analog Potentiometers
