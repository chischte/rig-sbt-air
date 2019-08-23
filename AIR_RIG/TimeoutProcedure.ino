void TimeoutProcedure()
{
  Serial.println("TIMEOUT");
  knockoutCounter++;
  switch (knockoutCounter)
  {
  case 1: // run startup procedure
    digitalWrite(PWM_IN_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open
    digitalWrite(PWM_OUT_VALVE, LOW); // LOW=outlet open // HIGH=outlet closed
    digitalWrite(SLOW_UPLIFT_VALVE, LOW); //LOW=inlet closed // HIGH=inlet open
    Serial.println("FIRST TIMEOUT - RUN STARTUP PROCEDURE");

    delay(2000); // time to push the stop button to read out the error
    RunStartupProcedure();
    break;
  case 2: // reset stapler and run startup procedure
    Serial.println("SECOND TIMEOUT - RESET STAPLER AND COOLING DOWN");

    digitalWrite(PWM_IN_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open
    digitalWrite(PWM_OUT_VALVE, LOW); // LOW=outlet open // HIGH=outlet closed
    digitalWrite(SLOW_UPLIFT_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open

    delay(20000); // cooldown

    digitalWrite(STAPLER_RELAY, LOW);
    delay(20000);
    digitalWrite(STAPLER_RELAY, HIGH);
    delay(1000);
    RunStartupProcedure();
    break;
  case 3: // reset stapler and run startup procedure
    Serial.println("THIRD TIMEOUT - RESET STAPLER AND COOLING DOWN");

    digitalWrite(PWM_IN_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open
    digitalWrite(PWM_OUT_VALVE, LOW); // LOW=outlet open // HIGH=outlet closed
    digitalWrite(SLOW_UPLIFT_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open

    delay(120000); // 2 minutes cooldown

    digitalWrite(STAPLER_RELAY, LOW);
    delay(20000);
    digitalWrite(STAPLER_RELAY, HIGH);
    delay(1000);
    RunStartupProcedure();
    break;
  case 4: // turn off test rig
    Serial.println("FOURTH TIMEOUT - SWITCH OFF RIG");
    digitalWrite(PWM_IN_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open
    digitalWrite(PWM_OUT_VALVE, LOW); // LOW=outlet open // HIGH=outlet closed
    digitalWrite(SLOW_UPLIFT_VALVE, LOW); // LOW=inlet closed // HIGH=inlet open
    digitalWrite(STAPLER_RELAY, LOW);

    machineRunning = false;
    break;
  }
}
