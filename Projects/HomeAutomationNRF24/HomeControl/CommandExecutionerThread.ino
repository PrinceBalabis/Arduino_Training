/**
 ** CommandExecutioner thread
 ** Executes commands
 **/

static bool commandOrigin = 0;
static int32_t commandToExecute = 0;

// Declare a semaphore with an inital counter value of zero.
SEMAPHORE_DECL(cmdExSem, 0);

NIL_WORKING_AREA(commandExecutioner, 150); //65 bytes works great
NIL_THREAD(CommandExecutioner, arg)
{
  Serial.println(F("Started CommandExecutioner thread"));

  while (1)
  {
    // Wait for signal from either HNListenThread or Keypad Thread to continue
    nilSemWait(&cmdExSem);
    
    switch (commandOrigin) {
      case COMMANDEXECUTIONER_MSGORIGIN_HOMENETWORK: // If the command is from Home Network
        switch (commandToExecute) {
          case HOME_HOMECONTROL_CMD_PC_ON:
            setPCPowerSwitchOnMomentarily();
            Serial.println(F("Turned PC on"));
            break;
          case HOME_HOMECONTROL_CMD_SPEAKER_SWITCH_TOGGLE:
            toggleAudioSwitch();
            Serial.println(F("Toggled speaker switch "));
            break;
          case HOME_HOMECONTROL_CMD_SPEAKER_SWITCH_SPEAKER:
            setAudioSwitchSpeaker();
            Serial.println(F("Set speaker switch to speaker"));
            break;
          case HOME_HOMECONTROL_CMD_SPEAKER_SWITCH_HEADSET:
            setAudioSwitchHeadset();
            Serial.println(F("Set speaker switch to headset"));
            break;
        }
        break;
      case COMMANDEXECUTIONER_MSGORIGIN_KEYPAD: // If the command is from local origin(keypad)
        switch (commandToExecute) {
          case BUTTON_MAINLIGHTS_TOGGLE:
            sendTWICommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_MAINLIGHTS_TOGGLE);
            Serial.println(F("Toggling Main Lights"));
            break;
          case BUTTON_PAINTINGLIGHTS_TOGGLE:
            sendTWICommand(HOME_LIGHTS433POWER_ID, HOME_LIGHTS433POWER_CMD_PAINTINGLIGHTS_TOGGLE);
            Serial.println(F("Toggling Painting Lights"));
            break;
          case BUTTON_SPEAKER_POWER:
            sendTWICommand(HOME_SPEAKER_ID, HOME_SPEAKER_CMD_POWER_TOGGLE);
            Serial.print(F("Toggling Speaker Power"));
            break;
          case BUTTON_SPEAKER_VOLUME_UP:
            sendTWICommand(HOME_SPEAKER_ID, HOME_SPEAKER_CMD_VOLUME_UP);
            Serial.print(F("Increasing Speaker Volume"));
            break;
          case BUTTON_SPEAKER_VOLUME_DOWN:
            sendTWICommand(HOME_SPEAKER_ID, HOME_SPEAKER_CMD_VOLUME_DOWN);
            Serial.print(F("Decreasing Speaker Volume"));
            break;
          case BUTTON_SPEAKER_MUTE:
            sendTWICommand(HOME_SPEAKER_ID, HOME_SPEAKER_CMD_MUTE_TOGGLE);
            Serial.print(F("Toggling Speaker Mute"));
            break;
          case BUTTON_SPEAKER_MODE:
            toggleAudioSwitch();
            Serial.println(F("Toggling Speaker Mode"));
            break;
          case BUTTON_PC_POWER:
            togglePCPowerSwitch();
            Serial.println(F("Toggling PC Power Switch"));
            break;
          case BUTTON_PC_SLEEP:
            //            sent = homeNetwork.sendCommand(HOME_WEBSERVER_ID, HOME_WEBSERVER_CMD_MONITOR_DISABLE);
            Serial.print(F("Disabling Monitors"));
            break;
        }
    }
  }
}

/*
   Run this function to enable CommandExecutionerThread to run
*/
void executeCommand(int32_t _commandToExecute, bool _commandOrigin) {
  commandToExecute = _commandToExecute;
  commandOrigin = _commandOrigin;
  nilSemSignal(&cmdExSem);
}
