#include "ScooterController.h"
#include "Functions.h"

#define DEFAULT_SPEED_LIMIT 22


void setup() {
  int speedLimit = scooter.getFromEEPROM("speedLimit") ?: DEFAULT_SPEED_LIMIT;

  delay(5000);
  
  // By default, the last three parameters can be omitted.
  // If using a template without data modification, the second parameter can also be left out.
  // 
  scooter.sendCommand(ScooterController::ECO_SPEED_LIMIT, intToBytes(speedLimit), false, true, false);    //not blink, use serial begin, not use serial end
  scooter.sendCommand(ScooterController::SPORT_SPEED_LIMIT, intToBytes(speedLimit), false, false, false); //not blink, not use serial begin, not use serial end
  scooter.sendCommand(ScooterController::RACE_SPEED_LIMIT, intToBytes(speedLimit), true, false, true);    //blink, not use serial begin, use serial end

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
