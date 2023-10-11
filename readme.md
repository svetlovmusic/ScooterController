# Scooter Controller Project

This project provides an ESP8266 controller for a scooter. It allows you to set the maximum speed limit for different modes (race, sport, eco) and perform other control operations.

## Features

- Set speed limit for different modes
- Save and retrieve speed limit from EEPROM
- Control the scooter's lock and lights

## Hardware Requirements

- ESP8266 board (12F, WEMOS 01, NodeMCU)
- Scooter with the appropriate communication interface ;-)

## Installation

1. Clone or download the repository.
2. Open the Arduino IDE.
3. Navigate to **Sketch > Include Library > Add .ZIP Library** and select the downloaded ZIP file.
4. Connect the ESP board to your computer.
5. Upload the sketch to the ESP board.


## The project structure:

- **ScooterController.ino**: The main Arduino sketch file that contains the `setup()` and `loop()` functions.
- **DataArrays.h**: Contains the byte arrays used for sending commands to the scooter.
- **Functions.h**: Contains various helper functions for handling HTTP requests, converting data types, and setting up WiFi and HTTP server.
- **HTMLData.h**: Contains the HTML code that is sent to the client when accessing the web interface.
- **ScooterController.cpp**: Contains the implementation of the `ScooterController` class, which handles sending commands to the scooter and managing EEPROM data.
- **ScooterController.h**: Contains the declaration of the `ScooterController` class and its member functions and variables.
- **WifiConfig.h**: Contains the Wi-Fi and Access Point configuration settings.