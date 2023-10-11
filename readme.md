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

## Adding a New Command with Data to the Scooter Controller (example not contain real data)

To add a new command that allows the user to control the brightness of the display, follow these steps:

1. Open the `DataArrays.h` file and add a new constant byte array for the command packet. For example:

   ```cpp
   const byte setBrightness[] = {0x5A, 0xA5, 0x02, 0x3E, 0x20, 0x03, 0xAB, 0x16, 0x00, 0xFF, 0xFF};
   ```

2. Open the `ScooterController.h` file and add a new entry in the `Command` enumeration for the new command. For example:

   ```cpp
   enum Command {
	   RACE_SPEED_LIMIT,
	   SPORT_SPEED_LIMIT,
	   ECO_SPEED_LIMIT,
	   LOCK,
	   LAMP_ON,
	   LAMP_OFF,
	   SET_BRIGHTNESS, // New command for setting brightness
	   COMMAND_COUNT
   };
   ```

3. In the `ScooterController` constructor, add the new command to the `commands` array:

   ```cpp
   commands[SET_BRIGHTNESS] = {setBrightness, sizeof(setBrightness)};
   ```

4. In the `handleSetBrightness` function in `Functions.h`, add a new handler for this command. For example:

   ```cpp
   void handleSetBrightness() {
	 String brightness = httpServer.arg("brightness");
	 int brightnessValue = brightness.toInt();
	 
	 byte* packet = scooter.sendCommand(ScooterController::SET_BRIGHTNESS, intToBytes(brightnessValue));
   
	 if (packet) {
	   String packetStr = byteArrayToString(packet, scooter.lastSentPacketSize);
	   httpServer.send(200, "text/plain", "Brightness set to: " + brightness + " Packet: " + packetStr);
	 } else {
	   httpServer.send(200, "text/plain", "Brightness set to: " + brightness + " Packet: error");
	 }
   }
   ```

5. In the `setup` function in `scooter_controller.ino`, add a new HTTP handler for the `/set_brightness` endpoint:

   ```cpp
   httpServer.on("/set_brightness", handleSetBrightness);
   ```

6. Build and upload the updated code to your ESP8266 board.

Now, users can send a POST request to `/set_brightness` with the desired brightness value, and the scooter controller will send the corresponding command packet to control the brightness of the display.

You can also enhance the web interface of the program to control the display brightness from it.

## Send a custom command to control the scooter using `scooter.sendCommand()`

You need to provide the following parameters:

- `command`: The desired command from the `ScooterController::Command` enum. This specifies the type of command to be sent.
- `data`: (optional) An array of bytes containing the data to be sent along with the command. The format and size of the data depend on the specific command you are sending. If no data is required for the command, you can pass `nullptr`.
- `useBlinkConfirmation`: (optional) A boolean value indicating whether to use blink confirmation. Blink confirmation is a visual feedback on the scooter that the command has been received. The default value is `true`.
- `useSerialBegin`: (optional) A boolean value indicating whether to initialize the Serial communication before sending the command. The default value is `true`.
- `useSerialEnd`: (optional) A boolean value indicating whether to terminate the Serial communication after sending the command. The default value is `true`.

If you have a template packet and you want to modify the data within it before sending, you can set the template packet to the byte array templatePacket (DataArrays.h), and pass the new data as the `data` parameter.
Here's an example of how to assemble a new data array and send it along with the template packet:

```cpp
//set new data to templatePacket in `DataArrays.h`
const byte templatePacket[] = {0x5A, 0xA5, 0x02, 0x3E, 0x20, 0x03, 0x8C, 0x00, 0x00, 0xFF, 0xFF};

// Send the custom command with the modified data
byte newTemplateData[] = {0x01, 0x00};
scooter.sendCommand(ScooterController::TEMPLATE_PACKET, newTemplateData);

  
// example use TEMPLATE_PACKET without new data, just send TEMPLATE_PACKET as is
// scooter.sendCommand(ScooterController::TEMPLATE_PACKET);
```

In this example, we have a template packet with a length of 11 bytes. The data portion of the template starts at index 8. We then create a new byte array `newTemplateData` with two bytes of data. This newTemplateData modifies the template packet before sending.
The data size of the packet is calculated automatically. If the template indicates that the size is 2 bytes, it means you can modify exactly 2 bytes in this packet by passing them.

## How to understand what's in the packet

Example packet: 5A A5 02 3E 20 03 81 12 00 09 FF

`5A A5` - Preambula
`02` - Data size
`3E 20` - Route
`03` - Data write code
`81` - Data Index
`12 00` - Data
`09 FF` - Checksum

## Adding a New Command with Data to the Scooter Controller and use it in project (example not contain real data)

Steps:

1. Open the `DataArrays.h` file and add a new constant byte array for the command packet. For example:

   ```cpp
   const byte newPacket[] = {0x5A, 0xA5, 0x02, 0x3E, 0x20, 0x03, 0xAB, 0x16, 0x00, 0xFF, 0xFF};
   ```

2. Open the `ScooterController.h` file and add a new entry in the `Command` enumeration for the new command. For example:

   ```cpp
   enum Command {
	   RACE_SPEED_LIMIT,
	   SPORT_SPEED_LIMIT,
	   ECO_SPEED_LIMIT,
	   LOCK,
	   LAMP_ON,
	   LAMP_OFF,
	   NEW_PACKET, // New command
	   COMMAND_COUNT
   };
   ```

3. In the `ScooterController` constructor, add the new command to the `commands` array:

   ```cpp
   commands[NEW_PACKET] = {newPacket, sizeof(newPacket)};
   ```

4. Send NEW_PACKET:

   ```cpp
	scooter.sendCommand(ScooterController::NEW_PACKET);
   ```





## The project structure:

- **ScooterController.ino**: The main Arduino sketch file that contains the `setup()` and `loop()` functions.
- **DataArrays.h**: Contains the byte arrays used for sending commands to the scooter.
- **Functions.h**: Contains various helper functions for handling HTTP requests, converting data types, and setting up WiFi and HTTP server.
- **HTMLData.h**: Contains the HTML code that is sent to the client when accessing the web interface.
- **ScooterController.cpp**: Contains the implementation of the `ScooterController` class, which handles sending commands to the scooter and managing EEPROM data.
- **ScooterController.h**: Contains the declaration of the `ScooterController` class and its member functions and variables.
- **WifiConfig.h**: Contains the Wi-Fi and Access Point configuration settings.