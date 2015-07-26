bool pcPowerSwitchStatus = false;

void pcPowerSetup()
{
  pinMode(pcPowerPin, OUTPUT);
  digitalWrite(pcPowerPin, LOW);
}

void togglePCPowerSwitch()
{
  pcPowerSwitchStatus = !pcPowerSwitchStatus;
  digitalWrite(pcPowerPin, pcPowerSwitchStatus);
}

void setPCPowerSwitchOnMomentarily()
{
  digitalWrite(pcPowerPin, true);
  chThdSleepMilliseconds(1000); // Give enough time for PC to start
  digitalWrite(pcPowerPin, false);
}
