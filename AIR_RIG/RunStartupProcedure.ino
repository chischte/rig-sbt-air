void RunStartupProcedure()
{
  Serial.println("RUNNING STARTUP ROUTINE");
  ToggleOnOff(); // SWITCH MACHINE ON/OFF
  digitalWrite(PWM_IN_VALVE, LOW); // LOW=inlet closed // HIGH=inlet_open
  digitalWrite(PWM_OUT_VALVE, LOW); // LOW=outlet open // HIGH=outlet_closed
  digitalWrite(SLOW_UPLIFT_VALVE, LOW); // LOW=inlet closed // HIGH=inlet_open
  delay(1000);
  Serial.println("RUNNING STARTUP LOOP");
  delay(1000);
  //***************************************************************************

  Serial.println("RESET TOOL BY DISCONNECTING POWER SUPPLY");
  digitalWrite(STAPLER_RELAY, LOW);
  delay(2000);
  digitalWrite(STAPLER_RELAY, HIGH);
  delay(1000);
  //***************************************************************************

  Serial.println("WAKE UP TOOL");
  digitalWrite(TRIGGER_VALVE, LOW);  // HIGH=pull // LOW=release
  delay(300);
  digitalWrite(TRIGGER_VALVE, HIGH);  // HIGH=pull // LOW=release
  delay(500);
  digitalWrite(TRIGGER_VALVE, LOW);  // HIGH=pull // LOW=release
  delay(3000);
  ToggleOnOff(); // switch machine on/off

  //***************************************************************************

  Serial.println("SLEDGE CALIBRATION");

  digitalWrite(TRIGGER_VALVE, HIGH); // HIGH=pull // LOW=release
  delay(6000);
  digitalWrite(TRIGGER_VALVE, LOW); // HIGH=pull // LOW=release
  delay(300);
  ToggleOnOff(); // switch machine on/off

  //***************************************************************************

  Serial.println("PLATFORM CALIBRATION");
  delay(500);
  CalibratePlatform();
  ToggleOnOff(); //switch machine on/off

  //***************************************************************************

  Serial.println("PUSH DOWN PLATFORM");

  digitalWrite(TRIGGER_VALVE, HIGH); // HIGH=pull // LOW=release
  delay(300);
  digitalWrite(TRIGGER_VALVE, LOW); // HIGH=pull // LOW=release
  delay(1000);
  ToggleOnOff(); // switch machine on/off

  //***************************************************************************

  timeoutStopwatch = millis();
  cycleState = 0;
  startupModeActive = false;

  Serial.println("EXIT STARTUP ROUTINE LOOP");
  delay(1000);
  timeoutStopwatch = millis();
}
