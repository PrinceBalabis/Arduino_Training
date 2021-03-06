/*

   Before running this sketch you must configure ESP-05 to connect to your network at boot
   Also it must run at 38400 baud
   Set WiFi Mode to only STA
      AT+CWMODE=1
   Join Access Point
      AT+CWJAP="Router","kungarike"
   Disable watchdog
      AT+CSYSWDTDISABLE
   Set to 38400 baud
      AT+CIOBAUD=38400
   Restart to save settings
      AT+RST

 * * Make sure the ESP-05 has at least 200-300 mA!!!! (by using separate power supply)
   Runs at 3.3V

   If there is still no answer from ESP-05, unplug and replug the ESP-05 onto the breadboard to fully reset it,
   (Might just work to reconnect the 5V cable to reset it)
   or just unplug the cable from the power supply(it doesnt work to use the switch button for some reason, the cable has to be pulled out.)
   Make sure you connect the 200-300mA power supply to boot the ESP-05 first before connecting Arduino to Computer
   Also give some seconds before connecting the Arduino to Computer, to give the ESP-05 some time to boot

   How to Send a GET HTTP Request from a simple internet browser:
   Enter this in browser to send command 1
   http://princehome.mooo.com:9500?c1
*/
// If a thread weirdly crashes then increase the stack value

#include <NilRTOS.h>
#include <NilSerial.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <HomeNetwork.h>
#include "config.h"
#include <SoftwareSerial.h>

RF24 radio(RF24_PIN_CE, RF24_PIN_CSN);
RF24Network network(radio);
HomeNetwork homeNetwork(radio, network);

#define Serial NilSerial

SoftwareSerial esp8266(2, 3);

bool executionerIdle = 0;
int32_t answer = -999;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Home Network Webserver Node"));

  esp8266.begin(38400); // your esp's baud rate might be different

  SPI.begin(); // SPI is used by homeNetwork

  // Initialize Home Network
  homeNetwork.setDebug(true); // Enable debug on home Network Library
  homeNetwork.begin(NODEID, &executeHomeNetworkCommand);
  homeNetwork.setNetworkUpdateTime(HOME_SETTING_TIME_NETWORKAUTOUPDATE);

  Serial.println(F("Basic system booted up! Starting RTOS..."));

  nilSysBegin(); // Start Nil RTOS.
}

void loop() {
  //printStackInfo(); // Print stack information
}

void printStackInfo() {
  nilPrintStackSizes(&Serial);
  nilPrintUnusedStack(&Serial);
  Serial.println();

  // Delay for one second.
  // Must not sleep in loop so use nilThdDelayMilliseconds().
  // Arduino delay() can also be used in loop().
  nilThdDelayMilliseconds(1000);
}


