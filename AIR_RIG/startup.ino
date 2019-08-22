void run_startup_procedure()
{
  Serial.println("RUNNING STARTUP ROUTINE");
  toggle_on_off(); //SWITCH MACHINE ON/OFF
  digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
  digitalWrite(pwm_out_valve, LOW); //LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
  digitalWrite(slow_uplift_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
  delay(1000); //temporary
  Serial.println("RUNNING STARTUP LOOP");
  delay(1000); //temporary

  //***************************************************************************

  Serial.println("RESET TOOL BY DISCONNECTING POWER SUPPLY");
  digitalWrite(stapler_relay, LOW);
  delay(2000);
  digitalWrite(stapler_relay, HIGH);
  delay(1000);
  //***************************************************************************

  Serial.println("WAKE UP TOOL");
  digitalWrite(trigger_valve, LOW);  //HIGH=pull //LOW=release
  delay(300);
  digitalWrite(trigger_valve, HIGH);  //HIGH=pull //LOW=release
  delay(500);
  digitalWrite(trigger_valve, LOW);  //HIGH=pull //LOW=release
  delay(3000);
  toggle_on_off(); //SWITCH MACHINE ON/OFF

  //***************************************************************************

  Serial.println("SLEDGE CALIBRATION");

  digitalWrite(trigger_valve, HIGH); //HIGH=pull //LOW=release
  delay(6000);
  digitalWrite(trigger_valve, LOW); //HIGH=pull //LOW=release
  delay(300);
  toggle_on_off(); //SWITCH MACHINE ON/OFF

  //***************************************************************************

  Serial.println("PLATFORM CALIBRATION");
  delay(500); //temporary debug
  calibrate_platform();
  toggle_on_off(); //SWITCH MACHINE ON/OFF

  //***************************************************************************

  Serial.println("PUSH DOWN PLATFORM");

  digitalWrite(trigger_valve, HIGH); //HIGH=pull //LOW=release
  delay(300);
  digitalWrite(trigger_valve, LOW); //HIGH=pull //LOW=release
  delay(1000);
  toggle_on_off(); //SWITCH MACHINE ON/OFF

  //***************************************************************************

  timeout_stopwatch = millis();
  cycle_state = 0;
  startupmode_active = false;

  Serial.println("EXIT STARTUP ROUTINE LOOP");
  delay(1000);
  timeout_stopwatch = millis();
}
