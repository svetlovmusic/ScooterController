#include "ScooterController.h"

ScooterController::ScooterController() {
    eepromMappings[0] = {"speedLimit", 0};
    eepromMappings[1] = {"seriesId", 4};
    eepromMappings[2] = {"some_name_settings", 5}; 



    commands[TEMPLATE_PACKET].packet[GT_SERIES] = templatePacket;
    commands[TEMPLATE_PACKET].size[GT_SERIES] = sizeof(templatePacket);
    
    commands[RACE_SPEED_LIMIT].packet[GT_SERIES] = gt_setRaceMaxSpeed;
    commands[RACE_SPEED_LIMIT].size[GT_SERIES] = sizeof(gt_setRaceMaxSpeed);
    
    commands[SPORT_SPEED_LIMIT].packet[GT_SERIES] = gt_setSportMaxSpeed;
    commands[SPORT_SPEED_LIMIT].size[GT_SERIES] = sizeof(gt_setSportMaxSpeed);
    
    commands[ECO_SPEED_LIMIT].packet[GT_SERIES] = gt_setEcoMaxSpeed;
    commands[ECO_SPEED_LIMIT].size[GT_SERIES] = sizeof(gt_setEcoMaxSpeed);
    
    commands[LOCK].packet[GT_SERIES] = gt_setLock;
    commands[LOCK].size[GT_SERIES] = sizeof(gt_setLock);
    
    commands[LAMP_ON].packet[GT_SERIES] = gt_setLampOn;
    commands[LAMP_ON].size[GT_SERIES] = sizeof(gt_setLampOn);

    commands[LAMP_OFF].packet[GT_SERIES] = gt_setLampOff;
    commands[LAMP_OFF].size[GT_SERIES] = sizeof(gt_setLampOff);
    
    
    
    commands[TEMPLATE_PACKET].packet[P_SERIES] = templatePacket;
    commands[TEMPLATE_PACKET].size[P_SERIES] = sizeof(templatePacket);
    
    commands[SPORT_SPEED_LIMIT].packet[P_SERIES] = p_setSportMaxSpeed;
    commands[SPORT_SPEED_LIMIT].size[P_SERIES] = sizeof(p_setSportMaxSpeed);
    
    commands[DRIVE_SPEED_LIMIT].packet[P_SERIES] = p_setDriveMaxSpeed;
    commands[DRIVE_SPEED_LIMIT].size[P_SERIES] = sizeof(p_setDriveMaxSpeed);
    
    commands[ECO_SPEED_LIMIT].packet[P_SERIES] = p_setEcoMaxSpeed;
    commands[ECO_SPEED_LIMIT].size[P_SERIES] = sizeof(p_setEcoMaxSpeed);
    
    commands[RED_SPORT_ON].packet[P_SERIES] = p_setRedSportOn;
    commands[RED_SPORT_ON].size[P_SERIES] = sizeof(p_setRedSportOn);
    
    commands[RED_SPORT_OFF].packet[P_SERIES] = p_setRedSportOff;
    commands[RED_SPORT_OFF].size[P_SERIES] = sizeof(p_setRedSportOff);
    
    commands[SPEED_LIMIT_ON].packet[P_SERIES] = p_setSpeedLimitOn;
    commands[SPEED_LIMIT_ON].size[P_SERIES] = sizeof(p_setSpeedLimitOn);
    
    commands[SPEED_LIMIT_OFF].packet[P_SERIES] = p_setSpeedLimitOff;
    commands[SPEED_LIMIT_OFF].size[P_SERIES] = sizeof(p_setSpeedLimitOff);
    
    commands[LAMP_ON].packet[P_SERIES] = p_setLampOn;
    commands[LAMP_ON].size[P_SERIES] = sizeof(p_setLampOn);
    
    commands[LAMP_OFF].packet[P_SERIES] = p_setLampOff;
    commands[LAMP_OFF].size[P_SERIES] = sizeof(p_setLampOff);
}

byte* ScooterController::sendCommand(Command command, const byte* data, bool useBlinkConfirmation, bool useSerialBegin, bool useSerialEnd) {
    if (command < 0 || command >= COMMAND_COUNT) return nullptr;
    
    sendPacket(commands[command].packet[currentSeries], data, useBlinkConfirmation, useSerialBegin, useSerialEnd);

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
    const byte* setLampOn = commands[LAMP_ON].packet[currentSeries];
    const byte* setLampOff = commands[LAMP_OFF].packet[currentSeries];
    int sizeLampOn = commands[LAMP_ON].size[currentSeries];
    int sizeLampOff = commands[LAMP_OFF].size[currentSeries];
    
    for(int i = 0; i < numberOfBlinks * 2; i++) {
        Serial1.write(i % 2 == 0 ? setLampOn : setLampOff, i % 2 == 0 ? sizeLampOn : sizeLampOff);
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

void ScooterController::selectSeries(Series series) {
    currentSeries = series;
}