/*
  ---------------------------------------------------------------
  This is the config tab.
  All tweakable settings are here.
  It must be named "A_" in the start of the tab-name or else it won't compile
  ---------------------------------------------------------------
*/

/*
  ---------------------------------------------------------------
  HTTP/Wi-Fi settings
  ---------------------------------------------------------------
*/
const char* ssid = "Router";
const char* password = "kungarike";
const uint16_t serverPort = 6548; // Port for this server, so we can access it from the internet
const uint8_t wifiConnectTimeout = 5; // The amount of seconds to wait until giving up connecting to Wi-Fi

/*
  ---------------------------------------------------------------
  Pin-outs
  ---------------------------------------------------------------
*/
const int ledPinESP =  D4;// LED on ESP on pin D4(Must call D4 in code)
const int ledPinBoard =  D0; // LED on board on pin D0(Must call D0 in code)
const int ceilingLightsPin =  D1; // pin to the relay which controls the ceilinglights
const int leftWallSwitchPin =  D5; // pin connected to the left wall switch
const int rightWallSwitchPin =  D2; // pin connected to the right wall switch
// (Milliseconds)The amount of time to wait untill accepting another buttonpress
// 400ms seeems to be the sweet spot for the NodeMCU
const unsigned long interruptDebounce = 400;
const int mhz433Pin = D7; // pin to the 433MHz transmitter

/*
  ---------------------------------------------------------------
   Execution frequency in milliseconds(does not mean sample time and does not account for execution times).
   So if a task has an exeution frequency of 10ms, and the task takes 5ms to execute one loop of its code,
   then the total sample time is 15ms(10+5).
  ---------------------------------------------------------------
*/
const uint32_t serverUpdateExeutionFrequency = 500; // How often to check if a new client has connected
//const uint32_t wallSwitchUpdateExecutionFrequency = 100; // How often to check if the wall switches has been pressed

/*
  ---------------------------------------------------------------
  433 MHz codes
  ---------------------------------------------------------------
*/
const uint8_t paintingLights_mhz433Code = 1; // Code for the paintinglights around the apartment
const uint8_t deskLights_mhz433Code = 2; // Code for the desklight

