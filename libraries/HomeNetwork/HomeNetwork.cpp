/**
*  HomeNetwork.cpp
*/

#include "HomeNetwork.h"

HomeNetwork::HomeNetwork( RF24& _radio, RF24Network& _network ): radio(_radio), network(_network)
{
}

void HomeNetwork::begin(uint16_t nodeID)
{
  radio.begin(); // Initialize radio
  network.begin(homeNetwork_channel, nodeID); // Start mesh Network
  radio.setRetries(homeNetwork_retryDelay, homeNetwork_retryTimes);
  radio.setPALevel(homeNetwork_powerAmplifierLevel);
  radio.setDataRate(homeNetwork_dataRate);
  network.txTimeout = 200;
}

void HomeNetwork::update(void (* pmsgReceivedF)(uint16_t,unsigned char,int32_t))
{
  if(!autoUpdatePaused){
    network.update(); // Check the network regularly for the entire network to function properly
    if(network.available())
    {
      int32_t msgReceived;
      unsigned char msgTypeReceived;
      uint16_t msgSenderReceived = read(&msgReceived, &msgTypeReceived);
      pmsgReceivedF(msgSenderReceived, msgTypeReceived, msgReceived);
      Serial.println(F("sdfgsdfgsdfgdfg"));
    }
  }
  chThdSleepMilliseconds(homeNetwork_autoUpdateTime);  //Give other threads some time to run
}

/**
* write
* This function sends the message to a receiver, both which are set in parameter
**/
uint8_t HomeNetwork::write(uint16_t msgReceiver, int32_t msgContent, unsigned char msgType)
{
  autoUpdatePaused = true; // Pause listening for messages
  chThdSleepMilliseconds(100); // Needed for stability, give autoupdate time to pause

  // Set receiver of message
  RF24NetworkHeader header(msgReceiver, msgType);

  // Send message to server, keep trying untill server confirms receiver gets the message
  bool msgSent = network.write(header, &msgContent, sizeof(msgContent));

  autoUpdatePaused = false; // Continue listening for messages
  if (msgSent) {
    return 1;
  }
  else {
    return 0;
  }
}

/**
* writeQuestion
* This function sends the message to a receiver, both which are set in parameter
* Gets a response back
**/
uint8_t HomeNetwork::writeQuestion(uint16_t msgReceiver, int32_t msgContent, int32_t *pmsgResponse)
{
  autoUpdatePaused = true; // Pause listening for messages
  bool answerTimeout = false;
  uint16_t msgSenderReceived = -1;
  int32_t msgReceived = 0;
  unsigned char msgTypeReceived = 'Z';
  chThdSleepMilliseconds(200); // Needed for stability, give autoupdate time to pause
  write(msgReceiver, msgContent, msgTypeAsk); // Send question

  //How long to wait for the answer
  unsigned long started_waiting_at = millis();

  while ((msgSenderReceived != msgReceiver || msgTypeReceived != msgTypeResponse) && !answerTimeout) {
    network.update(); // Check the network regularly for the entire network to function properly
    if(network.available())
    {
      msgSenderReceived = read(&msgReceived, &msgTypeReceived);
    }
    if (millis() - started_waiting_at > homeNetwork_timeoutAnswerTime && msgSenderReceived == -1) {
      answerTimeout = true;
    }
    chThdSleepMilliseconds(20); // Check every few ms if message is received
  }
  *pmsgResponse = msgReceived;
  autoUpdatePaused = false; // Continue listening for messages
  return !answerTimeout;
}

/**
*  read
*  This function reads the message and stores it to the variable sent in parameter
* returns the senders ID.int - returns -1 if read was unsuccesful
*/
uint16_t HomeNetwork::read(int32_t *pmsgReceived, unsigned char *pmsgType) {
  if (network.available()) {
    // Save sender node ID of received message
    RF24NetworkHeader peekHeader;
    network.peek(peekHeader);
    uint16_t msgSender = peekHeader.from_node;
    *pmsgType = peekHeader.type;

    // Save received message content
    RF24NetworkHeader readHeader;
    network.read(readHeader, pmsgReceived, sizeof(int32_t)); // Read message and store to msgReceived variable

    return msgSender;
  }
  else {
    *pmsgReceived = -1;
    return -1;
  }
}

uint8_t HomeNetwork::askExampleDataToExampleServer(int32_t *pmsgResponse) {
  return writeQuestion(nodeExampleServer, cmdExampleCommand, pmsgResponse);
}

uint8_t HomeNetwork::toggleMainLights() {
  return write(nodeMainLights, cmdToggleLights, msgTypeCommand);
}

// uint8_t HomeNetwork::setMainLightsOn() {
//   msgNode = mainLights;
//   msgContent = 02;
// }

// uint8_t HomeNetwork::setMainLightsOff() {
//   msgNode = mainLights;
//   msgContent = 03;
// }

// uint8_t HomeNetwork::togglePaintingLights() {
//   msgNode = centralHomeControl;
//   msgContent = 11;
// }

// uint8_t HomeNetwork::setPaintingLightsOn() {
//   msgNode = centralHomeControl;
//   msgContent = 9;
// }

// uint8_t HomeNetwork::setPaintingLightsOff() {
//   msgNode = centralHomeControl;
//   msgContent = 10;
// }

// uint8_t HomeNetwork::toggleSpeakerPower() {
//   msgNode = centralHomeControl;
//   msgContent = 01;
// }

// uint8_t HomeNetwork::setSpeakerPowerOn() {
//   msgNode = centralHomeControl;
//   msgContent = 02;
// }

// uint8_t HomeNetwork::setSpeakerPowerOff() {
//   msgNode = centralHomeControl;
//   msgContent = 03;
// }

// uint8_t HomeNetwork::shutdownAll() {
//   setMainLightsOff();
//   setPaintingLightsOff();
//   setSpeakerPowerOff();
// }

// uint8_t HomeNetwork::enterSleepMode() {
//   shutdownAll();
// }

// uint8_t HomeNetwork::leavingApartment() {
//   shutdownAll();
// }

// uint8_t HomeNetwork::exitSleepMode() {
//   setMainLightsOn();
//   setSpeakerPowerOn();
// }

// uint8_t HomeNetwork::setPartyMode() {
//   setPaintingLightsOn();
//   setMainLightsOff();
//   chThdSleepMilliseconds(20);
//   setSpeakerPowerOn();
// }
