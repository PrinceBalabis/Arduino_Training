/**
 ** CommandExecutioner thread
 ** Executes commands
 **/

static uint8_t commandOrigin = 0;
static int32_t commandToExecute = 0;
static uint16_t nodeToSendTo = 0;

// Declare a semaphore with an inital counter value of zero.
SEMAPHORE_DECL(cmdExSem, 0);

NIL_WORKING_AREA(commandExecutioner, 180); // bytes works great
NIL_THREAD(CommandExecutioner, arg)
{
  Serial.println(F("Started CommandExecutioner thread"));

  while (1)
  {
    // Wait for signal to run
    nilSemWait(&cmdExSem);

    switch (commandOrigin) {
      case COMMANDEXECUTIONER_MSGORIGIN_INTERNET_MACRO: // Macro Command that came from the internet
        switch (commandToExecute) {
          case QSN_MAINLIGHTS_STATUS:

            break;
          case CMD_APARTMENT_SHUTDOWN:
            Serial.println(F("Shut-down Apartment Command!"));
            shutdownApartment();
            break;
          case CMD_APARTMENT_STARTUP:
            Serial.println(F("Start-up Apartment Command!"));
            startupApartment();
            break;
          case CMD_APARTMENT_TOGGLE:
            Serial.println(F("Toggle Apartment Command!"));
            if (askApartmentStatus()) {
              shutdownApartment();
            } else {
              startupApartment();
            }
            break;
          case CMD_APARTMENT_MOOD_TOGGLE:
            Serial.println(F("Apartment Mood Toggle Command!"));
            toggleApartmentMood();
            break;
        }
        break;
      case COMMANDEXECUTIONER_MSGORIGIN_INTERNET: // Normal Command that came from the internet
        Serial.println(F("Command sent to HomeNetwork!"));
        homeNetwork.sendCommand(nodeToSendTo, commandToExecute);
        break;
      case COMMANDEXECUTIONER_MSGORIGIN_HOMENETWORK: // Command that came from HomeNetwork
        switch (commandToExecute) {
          case HOME_WEBSERVER_CMD_PC_SLEEP:
            callbackCommand = HOME_WEBSERVER_CMD_PC_SLEEP;
            Serial.println(F("Send GET-command to Sleep PC"));
            break;
          case HOME_WEBSERVER_CMD_SPOTIFY_CHILL:

            break;
          case HOME_WEBSERVER_CMD_SPOTIFY_DINNER:

            break;
          case HOME_WEBSERVER_CMD_SPOTIFY_WORKOUT:

            break;
          case HOME_WEBSERVER_CMD_SPOTIFY_WORK:

            break;
            //          case HOME_WEBSERVER_CMD_MONITOR_DISABLE:
            //            if (!sendGetRequest(1, 192, 168, 1, 2, 5060)) {
            //              Serial.println(F("Couldnt connect to server!"));
            //            } else {
            //              Serial.println(F("Disabled Monitors!"));
            //            }
            //            break;
            //          case HOME_WEBSERVER_CMD_MONITOR_ENABLE:
            //            if (!sendGetRequest(2, 192, 168, 1, 2, 5060)) {
            //              Serial.println(F("Couldnt connect to server!"));
            //            } else {
            //              Serial.println(F("Enabled Monitors!"));
            //            }
            //            break;
            //          case HOME_WEBSERVER_CMD_SPOTIFY_CHILL:
            //            if (!sendGetRequest(3, 192, 168, 1, 2, 5060)) {
            //              Serial.println(F("Couldnt connect to server!"));
            //            } else {
            //              Serial.println(F("Started Spotify Chill Playlist!"));
            //            }
            //            break;
            //          case HOME_WEBSERVER_CMD_SPOTIFY_DINNER:
            //            if (!sendGetRequest(4, 192, 168, 1, 2, 5060)) {
            //              Serial.println(F("Couldnt connect to server!"));
            //            } else {
            //              Serial.println(F("Started Spotify Dinner Playlist!"));
            //            }
            //            break;
            //          case HOME_WEBSERVER_CMD_SPOTIFY_WORKOUT:
            //            if (!sendGetRequest(5, 192, 168, 1, 2, 5060)) {
            //              Serial.println(F("Couldnt connect to server!"));
            //            } else {
            //              Serial.println(F("Started Spotify Workout Playlist!"));
            //            }
            //            break;
            //          case HOME_WEBSERVER_CMD_SPOTIFY_WORK:
            //            if (!sendGetRequest(6, 192, 168, 1, 2, 5060)) {
            //              Serial.println(F("Couldnt connect to server!"));
            //            } else {
            //              Serial.println(F("Started Spotify Work Playlist!"));
            //            }
            //            break;
        }
        break;
    }
  }
}

/*
   Run this function to enable CommandExecutionerThread to run
*/
void executeCommandFromHomeNetwork(uint8_t _commandToExecute) {
  commandToExecute = _commandToExecute;
  commandOrigin = COMMANDEXECUTIONER_MSGORIGIN_HOMENETWORK;
  nilSemSignal(&cmdExSem);
}

/*
   Run this function to enable CommandExecutionerThread to run
*/
void executeCommandFromInternetMacro(uint16_t _commandToExecute) {
  nodeToSendTo = 0;
  commandToExecute = _commandToExecute;
  commandOrigin = COMMANDEXECUTIONER_MSGORIGIN_INTERNET_MACRO;
  nilSemSignal(&cmdExSem);
}

/*
   Run this function to enable CommandExecutionerThread to run
*/
void executeCommandFromInternet(uint16_t _nodeToSendTo, uint16_t _commandToExecute) {
  nodeToSendTo = _nodeToSendTo;
  commandToExecute = _commandToExecute;
  commandOrigin = COMMANDEXECUTIONER_MSGORIGIN_INTERNET;
  nilSemSignal(&cmdExSem);
}

void shutdownApartment() {
  homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_MAINLIGHTS_OFF);
  Serial.println(F("Main lights shut down!"));
  homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_PAINTINGLIGHTS_OFF);
  Serial.println(F("Painting lights shut down!"));
  //  homeNetwork.sendCommand(HOME_SPEAKER_ID, HOME_SPEAKER_CMD_POWER_OFF);
  //  Serial.println(F("Speaker shut down!"));
  //homeNetwork.sendCommand(HOME_PC_ID, HOME_PC_CMD_MONITORS_DISABLE);
  //  Serial.println(F("PC Sleeping!"));
}

void startupApartment() {
  homeNetwork.sendCommand(HOME_HOMECONTROL_ID, HOME_HOMECONTROL_CMD_PC_ON); // Wake PC
  Serial.println(F("Woke PC!"));

  homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_MAINLIGHTS_ON); // Turn on main lights!
  Serial.println(F("Main lights turned on"));

  //nilThdSleepMilliseconds(4000); // Give some time for PC to wake before doing any more PC controls
  //homeNetwork.sendCommand(HOME_PC_ID, HOME_PC_CMD_SPOTIFY_PLAYLIST_WORKOUT); // Start Workout Playlist!!

  //nilThdSleepMilliseconds(5000); // Give some time for Spotify to start playlist
  //  nilThdSleepMilliseconds(1000);
  //  homeNetwork.sendCommand(HOME_SPEAKER_ID, HOME_SPEAKER_CMD_POWER_ON); // Turn on speaker!
  //  Serial.println(F("Speaker turned on"));
}

// Returns true if a source is on, returns false if all is off
bool askApartmentStatus() {

  bool status = false;

  status = getMainLightsStatus();
  if (status) {
    Serial.println(F("Apartment seems online, turning everything off.."));
    return true;
  }

  status = getPaintingLightsStatus();
  if (status) {
    Serial.println(F("Apartment seems online, turning everything off.."));
    return true;
  }

  //  status = getspeakerPowerSwitchStatus();
  //  if (status) {
  //    Serial.println(F("Apartment seems online, turning everything off.."));
  //    return true;
  //  }

  Serial.println(F("Apartment seems offline, turning everything on.."));
  return false;
}

//boolean getspeakerPowerSwitchStatus() {
//  int32_t status;
//  if (!homeNetwork.sendQuestion(HOME_SPEAKER_ID, HOME_SPEAKER_QSN_POWER_STATUS, &status, 1000)) {
//    Serial.println(F("Couldn't get an answer from Speaker Node!"));
//    return false;
//  }
//  else if (status) {
//    Serial.println(F("Speaker is on"));
//    return true;
//  } else {
//    Serial.println(F("Speaker is off"));
//  }
//  return false;
//}

boolean getPaintingLightsStatus() {
  int32_t status;
  if (!homeNetwork.sendQuestion(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_QSN_PAINTINGLIGHTS_STATUS, &status, 200)) {
    Serial.println(F("Couldn't get an answer from Main Lights Node!"));
    return false;
  }
  else if (status) {
    Serial.println(F("Painting lights are on"));
    return true;
  } else {
    Serial.println(F("Painting lights are off"));
  }
  return false;
}

boolean getMainLightsStatus() {
  int32_t status;
  if (!homeNetwork.sendQuestion(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_QSN_MAINLIGHTS_STATUS, &status, 200)) {
    Serial.println(F("Couldn't get an answer from Main Lights Node!"));
    return false;
  }
  else if (status) {
    Serial.println(F("Main lights are on"));
    return true;
  } else {
    Serial.println(F("Main lights are off"));
  }
  return false;
}

void toggleApartmentMood() {
  int32_t status;

  if (!homeNetwork.sendQuestion(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_QSN_MAINLIGHTS_STATUS, &status, 200)) {
    Serial.print(F("Couldn't get an answer from Main Lights & 433MHz controller Node!->"));
  }
  else if (status) { // Main lights are on, toggle off main lights and paintinglights on
    Serial.println(F("Main lights are on!"));
    homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_MAINLIGHTS_OFF);
    homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_PAINTINGLIGHTS_ON);
  }
  else if (!status) {// Main lights are off, check paintinglights status
    homeNetwork.sendQuestion(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_QSN_PAINTINGLIGHTS_STATUS, &status, 200);
    if (status) { // Paintinglights are on, toggle on main lights and paintinglights off
      Serial.println(F("Painting lights are on!"));
      homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_MAINLIGHTS_ON);
      homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_PAINTINGLIGHTS_OFF);
    } else {
      // If both Main lights and painting lights are off, toggle painting lights on
      Serial.println(F("Both Main lights and painting lights are off, toggle painting lights on"));
      homeNetwork.sendCommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_PAINTINGLIGHTS_ON);
    }
  }
}

