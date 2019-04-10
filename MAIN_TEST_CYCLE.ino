int run_main_test_cycle()
{
  switch (cycle_state)
  {
    //***************************************************************************
    case 0://lift up / build up startforce
      //***************************************************************************
      digitalWrite(trigger_valve, LOW);//HIGH=pull //LOW=release
      digitalWrite(pwm_out_valve, HIGH);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
      digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
      get_force_n_position();

      if (current_position > 1800 + min_position)
     
      {
        digitalWrite(slow_uplift_valve, HIGH);//LOW=INLET CLOSED // HIGH=INLET_OPEN
      }
      else
      {
        digitalWrite(slow_uplift_valve, LOW);//LOW=INLET CLOSED // HIGH=INLET_OPEN
        //delay(500);
        //Serial.println("STATE 0 OK");
        cycle_state++;

      }
      break;
    //***************************************************************************
    case 1://Wait till time for the new cycle has come
      //***************************************************************************
      timeout_stopwatch = millis(); //timeout disabled while waiting
      if (millis() > new_starttime)
      {
        // Serial.println("STATE 1 OK");
        cycle_state++;
      }
      break;

    //***************************************************************************
    case 2://pull trigger and start PWM Regulator
      //***************************************************************************
      digitalWrite(trigger_valve, HIGH);//HIGH=pull //LOW=release
      get_force_n_position();
      get_target_values_n_store_n_print();

      //GENERATE CURVE
      //***************************************************************************
      if (current_position > 1000 + min_position)
        //..in the beginning the air in the cylinder is just the right amount
        //..otherwise change startup force
      {
        if (total_force < (target_force * 1.0))//the last value is to scale the tool stress 1=full force 0.5=50%force
        {
          digitalWrite(pwm_out_valve, HIGH); //LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
          if (current_position > 25000 + min_position) //
          {
            digitalWrite(pwm_in_valve, HIGH); //LOW=INLET CLOSED // HIGH=INLET_OPEN
          }
        }
        else
        {
          digitalWrite(pwm_out_valve, LOW); //LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
          digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
        }
      }

      //DETECT CYCLE END OR ABORT IF DOWNWARDMOMEVEMENT STOPS
      //***************************************************************************
      if (current_position > (10000 + min_position) && current_position - previous_position < 30) ///////////////////WAS 35000+minposition
        //...no more downward movement, staple cycle seems to be finished
      {
        cycle_end_indicator++;
        digitalWrite(pwm_out_valve, LOW);//LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
        digitalWrite(pwm_in_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
      }
      else //IF THERES A DOWNWARDMOMEMENT DETECTED AGAIN
      {
        cycle_end_indicator = 0;
      }

      if (cycle_end_indicator == 5)//cycle state is finished
      {
         Serial.println("STATE 2 OK");
        cycle_state++;
        previous_position = 0; //reset for next staplecycle
        cycle_end_indicator = 0;
        if (current_position <= (32000 + min_position)) //default(35000)...CYCLE HAS BEEN ABORTED TOO EARLY!
        {
          Serial.println("Cycle aborted to early");
          run_startup_procedure();
        }
      }
      previous_position = current_position;
      break;
    //***************************************************************************
    case 3://update testcycle timer //original testintervall is 6x150ms pause 1x2.5s pause
      //***************************************************************************

      if (cycle_counter < 6)
      {
        new_starttime = millis() + 2000;//150=original cycle
        cycle_counter++;

      }
      if (cycle_counter == 6)
      {
        new_starttime = millis() + 2500;//2500=original cycle
        cycle_counter = 0;
        printclearance = true;//no need to deactivate, in the long pause is always enough time for this
      }
      Serial.print("TIMEOUT TIME [ms]:");
      Serial.println(millis() - timeout_stopwatch);
      timeout_stopwatch = millis();
      //printclearance = true; //deactivate for faster test rhythm
      Serial.println("STATE 3 OK");
      cycle_state++;
      break;
    //***************************************************************************
    case 4://print measured values
      //***************************************************************************
      get_target_values_n_store_n_print();
      if (millis() > timeout_stopwatch + 150) //default 80//wait a short while to let the tool go back to top
      {
         Serial.println("STATE 4 OK");
        cycle_state++;
      }
      break;
      //***************************************************************************
  }

  if (cycle_state == 5)
  {
    Serial.println("***** CYCLE COMPLETED *****");
    cycle_state = 0;
    timeout_stopwatch = millis();
    knockout_counter = 0;
  }
}
