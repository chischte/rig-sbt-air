int toggle_on_off()
{

  //STOP TEST_RIG
  //*****************************************************************************
  if (digitalRead(stop_button) == HIGH)
  {
    machine_running = false;
    knockout_counter = 0;
    //digitalWrite(stapler_relay, LOW);

  }
  if (machine_running == false)
  {
    //SET HIGHSPEED VALVES OFF
    digitalWrite(pwm_out_valve, LOW);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
    digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
    //OPEN DECOMPRESSION VALVE
    digitalWrite(decompression_valve, LOW); //in low state the valve vents are open
    digitalWrite(green_light_pin, LOW);
    digitalWrite(click_relay, LOW);
    digitalWrite(trigger_valve, LOW);

    //digitalWrite(stapler_relay, LOW);

    cycle_state = 0;
  }

  //START TEST RIG
  //*****************************************************************************
  if (digitalRead(start_button) == HIGH)
  {
    machine_running = true;
    cycle_state = 0;
    knockout_counter = 0;
    timeout_stopwatch = millis();
    digitalWrite(stapler_relay, HIGH);

  }
  if (machine_running == true)
  {
    digitalWrite(green_light_pin, HIGH);
    digitalWrite(click_relay, HIGH);
    //digitalWrite(trigger_valve,HIGH);
  }
}
