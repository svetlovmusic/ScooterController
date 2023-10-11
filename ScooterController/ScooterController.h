#ifndef ScooterController_h
#define ScooterController_h

#include <EEPROM.h>
#include <Arduino.h>
#include "DataArrays.h"

class ScooterController {
public:
    enum Series {
        GT_SERIES,  
        P_SERIES
    };
    
    void selectSeries(Series series);
    
    int lastSentPacketSize;
    
    enum Command {
        TEMPLATE_PACKET,
        RACE_SPEED_LIMIT,
        SPORT_SPEED_LIMIT,
        DRIVE_SPEED_LIMIT,
        ECO_SPEED_LIMIT,
        LOCK,
        LAMP_ON,
        LAMP_OFF,
        RED_SPORT_ON,
        RED_SPORT_OFF,
        SPEED_LIMIT_ON,
        SPEED_LIMIT_OFF,
        COMMAND_COUNT //number of modes, calculated automatically, should always be last in the list
    };

    ScooterController();

    byte* sendCommand(Command command, const byte* data, bool useBlinkConfirmation = true, bool useSerialBegin = true, bool useSerialEnd = true);

    void saveToEEPROM(String key, int value);

    int getFromEEPROM(String key);

private:
    Series currentSeries;
    
    byte lastSentPacket[MAX_PACKET_SIZE];
    
    struct EepromKeyAddress {
        String key;
        int address;
    };

    EepromKeyAddress eepromMappings[2];

    struct CommandData {
        const byte* packet[2]; 
        int size[2];
    } commands[COMMAND_COUNT];

    int getAddressByKey(String key);

    byte* sendPacket(const byte* templatePacket, const byte* data = nullptr, bool useBlinkConfirmation = true, bool useSerialBegin = true, bool useSerialEnd = true);

    void blinkConfirmation(int numberOfBlinks = 2, int delayTime = 50);

    uint16_t calculateChecksum(byte* packet, int packetSize);
};

#endif
