static msg_t Thread2(void *arg) {
  Serial.print(F("\n\n--------------------------------------------------------------- \nExample SERVER for Prince Home Network \nThis node ID: "));
  Serial.println(nodeID);

  chThdSleepMilliseconds(4000); // Give some time for HomeNetwork thread to start

  while (1) {
    // Wait for message to receive
    Serial.println(F("--------------Server Program Starting----------------- \nIdle and waiting for client"));
    while (msgContent == -1) {
      chThdSleepMilliseconds(50); // Check every 50ms if a message is received
    }

    if (msgContent == exampleData) {
      // Send return-message back to client
      uint8_t msgSent = homeNetwork.write(msgSender, exampleResponceData, msgTypeResponse);
      Serial.print("Client has a question \nSending response back: ");
      Serial.println(exampleResponceData);

      if (msgSent) {
        Serial.println(F("Message sent"));
      } else if (!msgSent) {
        Serial.println(F("ERROR!: Failed to send message to parent."));
      } else if (msgSent == 2) {
        Serial.println(F("OBS! FINALLY SENT MESSAGE AFTER SEVERAL RETRIES. BAD SIGNAL WITH PARENT!"));
      }
    }

    //Clear variables to reset
    msgSender = -1;
    msgContent = -1;
    Serial.println(F("---------------Server Program Done-------------------\n"));

    chThdSleepMilliseconds(100); // Redo this send program every 200 ms
  }

  return 0;
}

void homeNetworkMessageReceived() {
  int16_t msgSenderReceived;
  int32_t msgReceived;
  unsigned char msgTypeReceived = 'Z';
  msgSenderReceived = homeNetwork.read(&msgReceived, &msgTypeReceived);

  // Put code in this if-statement which should occur when a message is received
  if (msgReceived != -1) {
    msgSender = msgSenderReceived;
    msgContent = msgReceived;
    msgType = msgTypeReceived;
    Serial.print(F("---------------Received Data Raw------------------- \nRaw Message sender: "));
    Serial.println(msgSender);
    Serial.println(F("Raw Type content: "));
    Serial.write(msgType);
    Serial.print(F("\nRaw Message content: "));
    Serial.println(msgContent);
    Serial.println(F("-----------------------------------------------"));
  }
}
