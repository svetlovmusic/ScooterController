#include "ScooterController.h"

ScooterController::ScooterController() {
    eepromMappings[0] = {"speedLimit", 0};
    eepromMappings[1] = {"brightness", 4}; //example. how to add new data address to eeprom

    commands[RACE_SPEED_LIMIT] = {setRaceMaxSpeed, sizeof(setRaceMaxSpeed)};
    commands[SPORT_SPEED_LIMIT] = {setSportMaxSpeed, sizeof(setSportMaxSpeed)};
    commands[ECO_SPEED_LIMIT] = {setEcoMaxSpeed, sizeof(setEcoMaxSpeed)};
    commands[LOCK] = {setLock, sizeof(setLock)};
    commands[LAMP_ON] = {setLampOn, sizeof(setLampOn)};
    commands[LAMP_OFF] = {setLampOff, sizeof(setLampOff)};
}

byte* ScooterController::sendCommand(Command command, const byte* data, bool useBlinkConfirmation, bool useSerialBegin, bool useSerialEnd) {
    if (command < 0 || command >= COMMAND_COUNT) return nullptr;

    sendPacket(commands[command].packet, data, useBlinkConfirmation, useSerialBegin, useSerialEnd);

    return lastSentPacket;
}

byte* ScooterController::sendPacket(const byte* templatePacket, const byte* data, bool useBlinkConfirmation, bool useSerialBegin, bool useSerialEnd) {
    int dataSize = (data == nullptr) ? 0 : templatePacket[2];           // Size of the data from the template or 0 if data is not provided
    int packetSize = 9 + dataSize;                                      // 9 represents the size of all fields before data and the checksum

    // Copy the template into the packet to be sent
    memcpy(lastSentPacket, templatePacket, packetSize - 2);

    if(data != nullptr) {
        memcpy(&lastSentPacket[7], data, dataSize); 
    }

    // Calculate and update the checksum
    uint16_t checksum = calculateChecksum(lastSentPacket, packetSize - 2); 
    lastSentPacket[packetSize - 2] = checksum & 0xFF;
    lastSentPacket[packetSize - 1] = (checksum >> 8) & 0xFF;

    // Send the packet
    if(useSerialBegin) Serial1.begin(115200, SERIAL_8N1);
    
    delay(100);
    Serial1.write(lastSentPacket, packetSize);
    delay(100);
    
    if(useBlinkConfirmation) blinkConfirmation();
    if(useSerialEnd) Serial1.end();

    lastSentPacketSize = packetSize;
    return lastSentPacket;
}

void ScooterController::saveToEEPROM(String key, int value) {
    int addr = getAddressByKey(key);
    if (addr != -1) {
        EEPROM.begin(512);
        EEPROM.put(addr, value);
        EEPROM.commit();
        EEPROM.end();
    }
}

int ScooterController::getFromEEPROM(String key) {
    int value = 0;
    int addr = getAddressByKey(key);
    if (addr != -1) {
        EEPROM.begin(512);
        EEPROM.get(addr, value);
        EEPROM.end();
    }
    return value;
}

int ScooterController::getAddressByKey(String key) {
    for (int i = 0; i < sizeof(eepromMappings) / sizeof(EepromKeyAddress); ++i) {
        if (eepromMappings[i].key == key) {
            return eepromMappings[i].address;
        }
    }
    return -1;
}

void ScooterController::blinkConfirmation(int numberOfBlinks, int delayTime) {
    for(int i = 0; i < numberOfBlinks * 2; i++) {
        Serial1.write(i % 2 == 0 ? setLampOn : setLampOff, sizeof(setLampOn));
        delay(delayTime);
    }
}

uint16_t ScooterController::calculateChecksum(byte* packet, int packetSize) {
    uint16_t checksum = 0;
    for (int i = 2; i < packetSize - 2; i++) {
        checksum += packet[i];
    }
    checksum = ~checksum;
    return checksum;
}
