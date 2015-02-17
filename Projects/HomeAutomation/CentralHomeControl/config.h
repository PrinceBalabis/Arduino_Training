/*
*  This is the config file.
 *  Here you can configure Buttons assigns, IR codes, 
 *  RF24 codes & component pins.
 *  Also you can enable/disable serial
 */

/*
*  ------------------------------------
 *  RF24 Mesh Node
 *  ------------------------------------
 */
/**   Addresses/Topology                           Node Numbers  (To simplify address assignment in this demonstration)
 *           00                  - Master Node         ( 0 )
 *         01  02                - 1st Level children ( 1,2 )
 *  32 22 12    15 25 35 45    - 2nd Level children (7,5,3-4,6,8)
 */
// Map of RF24 nodes in the apartment
const uint16_t mainLights = 00; // Main Light Node
const uint16_t centralHomeControl = 01; // Centra Home Control Node
const uint16_t raspberryPi = 02; // Raspberry Pi Node
const uint16_t voiceRecog = 03; // VoiceRecog Node

// This is the nodeID of this Arduino.
// WARNING! DONT FORGET TO SET UNIQUE NODE ID IN config.h FOR EACH NODE!
const uint16_t nodeID = centralHomeControl;

// Commands this node has to offer
const int32_t speakerPowerToggle = 1;
const int32_t speakerPowerOn = 2;
const int32_t speakerPowerOff = 3;
const int32_t speakerVolumeUp = 4;
const int32_t speakerVolumeDown = 5;
const int32_t speakerMuteToggle = 6;
const int32_t speakerMuteOn = 7;
const int32_t speakerMuteOff = 8;
const int32_t diningTableOn = 9;
const int32_t diningTableOff = 10;
const int32_t diningTableToggle = 11;

/*
*  ------------------------------------
 *  Keypad Button reservations
 *  ------------------------------------
 */
// NextBase
const uint8_t nBPowerButton = 4;
const uint8_t nBUpVolButton = 3;
const uint8_t nBDownVolButton = 2;
const uint8_t nBMuteButton = 1;
// Lights
const uint8_t lightMainButton = 8;
const uint8_t lightDiningTableButton = 7;
// PC swtich
const uint8_t pcPowerButton = 12;
/*
*  ------------------------------------
 *  IR Codes
 *  ------------------------------------
 */
// NEXTBASE IR codes
const unsigned long nBIRPower = 0xFFC23D;
const unsigned long nBIRMute = 0xFF02FD;
const unsigned long nBIRUpVolume = 0xFFF00F;
const unsigned long nBIRDownVolume = 0xFF708F;

/*
*  ------------------------------------
 *  433 MHz Switches codes
 *  ------------------------------------
 */
const uint8_t speakerUnit = 2;
const uint8_t lightDiningTable = 1;

/*
*  ------------------------------------
 *  Components pins
 *  ------------------------------------
 */
// PC Power pin pin
const int pcPowerPin = 7;

// 433 MHz switch transmitter pin
const int switch433TransmitterPin = 6;
