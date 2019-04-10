int timeout_procedure()
{
  Serial.println("TIMEOUT");
  knockout_counter++;
  switch (knockout_counter)
  {
    //***************************************************************************
    case 1://run startup procedure
      //***************************************************************************
      digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
      digitalWrite(pwm_out_valve, LOW);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
      digitalWrite(slow_uplift_valve, LOW);//LOW=INLET CLOSED // HIGH=INLET_OPEN
      Serial.println("FIRST TIMEOUT - RUN STARTUP PROCEDURE");

      delay(2000);//time to push the stop button to read out the error
      run_startup_procedure();
      break;
    //***************************************************************************
    case 2://reset stapler and run startup procedure
      //***************************************************************************
      Serial.println("SECOND TIMEOUT - RESET STAPLER AND COOLING DOWN");

      digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
      digitalWrite(pwm_out_valve, LOW);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
      digitalWrite(slow_uplift_valve, LOW);//LOW=INLET CLOSED // HIGH=INLET_OPEN

      delay(20000);//cooldown

      digitalWrite(stapler_relay, LOW);
      delay(20000);
      digitalWrite(stapler_relay, HIGH);
      delay(1000);
      run_startup_procedure();
      break;
    //***************************************************************************
    case 3://reset stapler and run startup procedure
      //***************************************************************************
      Serial.println("THIRD TIMEOUT - RESET STAPLER AND COOLING DOWN");

      digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
      digitalWrite(pwm_out_valve, LOW);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
      digitalWrite(slow_uplift_valve, LOW);//LOW=INLET CLOSED // HIGH=INLET_OPEN

      delay(120000);//2 minutes cooldown

      digitalWrite(stapler_relay, LOW);
      delay(20000);
      digitalWrite(stapler_relay, HIGH);
      delay(1000);
      run_startup_procedure();
      break;
    //***************************************************************************
    case 4://turn off test rig
      //***************************************************************************
      Serial.println("FOURTH TIMEOUT - SWITCH OFF RIG");
      digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
      digitalWrite(pwm_out_valve, LOW);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
      digitalWrite(slow_uplift_valve, LOW);//LOW=INLET CLOSED // HIGH=INLET_OPEN
      digitalWrite(stapler_relay, LOW);

      machine_running = false;
      break;
  }
}
