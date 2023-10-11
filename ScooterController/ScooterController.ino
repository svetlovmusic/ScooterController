#include "ScooterController.h"

#define DEFAULT_SPEED_LIMIT 22
#define DEFAULT_SERIES_ID 0 //gt = 0, p = 1...

#include "Functions.h"

void setup() {
  int speedLimit = scooter.getFromEEPROM("speedLimit") ?: DEFAULT_SPEED_LIMIT;
  int seriesId = scooter.getFromEEPROM("seriesId") ?: DEFAULT_SERIES_ID;
  
  delay(5000);
  
  if(seriesId == 1) {
    scooter.selectSeries(ScooterController::P_SERIES);
    
    scooter.sendCommand(ScooterController::RED_SPORT_OFF, nullptr, false, true, false);                     //not blink, use serial begin, not use serial end
    scooter.sendCommand(ScooterController::SPEED_LIMIT_ON, nullptr, false, false, false);                   //not blink, not use serial begin, not use serial end
    scooter.sendCommand(ScooterController::ECO_SPEED_LIMIT, intToBytes(speedLimit), false, false, false);    //not blink, not use serial begin, not use serial end
    scooter.sendCommand(ScooterController::DRIVE_SPEED_LIMIT, intToBytes(speedLimit), false, false, false);  //not blink, not use serial begin, not use serial end
    scooter.sendCommand(ScooterController::SPORT_SPEED_LIMIT, intToBytes(speedLimit), true, false, true);    //blink, not use serial begin, use serial end
  } else {
    scooter.selectSeries(ScooterController::GT_SERIES);
    
    // By default, the last three parameters can be omitted.
    // If using a template without data modification, the second parameter can also be left out.
    // 
    scooter.sendCommand(ScooterController::ECO_SPEED_LIMIT, intToBytes(speedLimit), false, true, false);    //not blink, use serial begin, not use serial end
    scooter.sendCommand(ScooterController::SPORT_SPEED_LIMIT, intToBytes(speedLimit), false, false, false); //not blink, not use serial begin, not use serial end
    scooter.sendCommand(ScooterController::RACE_SPEED_LIMIT, intToBytes(speedLimit), true, false, true);    //blink, not use serial begin, use serial end
  }

  // example use TEMPLATE_PACKET with new data
  // byte newTemplateData[] = {0x01, 0x00};
  // scooter.sendCommand(ScooterController::TEMPLATE_PACKET, newTemplateData);
  
  // example use TEMPLATE_PACKET without new data, just send TEMPLATE_PACKET as is
  // scooter.sendCommand(ScooterController::TEMPLATE_PACKET);
  
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting...");
  
  startWiFi();
  startHttp();
}

void loop() {
  httpServer.handleClient();
  MDNS.update();
}