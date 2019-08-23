void RunMainTestCylce()
{
  switch (cycleState)
  {
  case 0: //lift up / build up startforce
    digitalWrite(TRIGGER_VALVE, LOW); //HIGH=pull //LOW=release
    digitalWrite(PWM_OUT_VALVE, HIGH); //LOW=outlet open // HIGH=outlet closed
    digitalWrite(PWM_IN_VALVE, LOW); //LOW=inlet closed // HIGH=inlet open
    GetForceNPosition();

    if (currentPosition > 1800 + min_position)

    {
      digitalWrite(SLOW_UPLIFT_VALVE, HIGH); //LOW=inlet closed // HIGH=inlet open
    }
    else
    {
      digitalWrite(SLOW_UPLIFT_VALVE, LOW); //LOW=inlet closed // HIGH=inlet open
      //Serial.println("STATE 0 OK");
      cycleState++;

    }
    break;
  case 1: //Wait till time for the new cycle has come
    timeoutStopwatch = millis(); //timeout disabled while waiting
    if (millis() > newStartTime)
    {
      // Serial.println("STATE 1 OK");
      cycleState++;
    }
    break;

  case 2: //pull trigger and start PWM Regulator
    digitalWrite(TRIGGER_VALVE, HIGH);      //HIGH=pull //LOW=release
    GetForceNPosition();
    GetTargetValuesNStoreNPrint();

    //***************************************************************************
    //GENERATE CURVE
    //***************************************************************************
    if (currentPosition > 1000 + min_position)
    //..in the beginning the air in the cylinder is just the right amount
    //..otherwise change startup force
    {
      if (totalForce < (targetForce * 1.0)) //the last value is to scale the tool stress 1=full force 0.5=50%force
      {
        digitalWrite(PWM_OUT_VALVE, HIGH); //LOW=outlet open // HIGH=outlet closed
        if (currentPosition > 25000 + min_position) //
        {
          digitalWrite(PWM_IN_VALVE, HIGH); //LOW=inlet closed // HIGH=inlet open
        }
      }
      else
      {
        digitalWrite(PWM_OUT_VALVE, LOW); //LOW=outlet open // HIGH=outlet closed
        digitalWrite(PWM_IN_VALVE, LOW); //LOW=inlet closed // HIGH=inlet open
      }
    }
    //***************************************************************************
    //DETECT CYCLE END OR ABORT IF DOWNWARD MOVEMENT STOPS
    //***************************************************************************
    if (currentPosition > (10000 + min_position)
        && currentPosition - previousPosition < 30) ///////////////////WAS 35000+minposition
    //...no more downward movement, staple cycle seems to be finished
    {
      cycleEndIndicator++;
      digitalWrite(PWM_OUT_VALVE, LOW); //LOW=outlet open // HIGH=outlet closed
      digitalWrite(PWM_IN_VALVE, LOW); //LOW=inlet closed // HIGH=inlet open
    }
    else //if there is a downward movement detected again
    {
      cycleEndIndicator = 0;
    }

    if (cycleEndIndicator == 5) //cycle state is finished
    {
      Serial.println("STATE 2 OK");
      cycleState++;
      previousPosition = 0; //reset for next staplecycle
      cycleEndIndicator = 0;
      if (currentPosition <= (32000 + min_position)) //previously used: 35000
      {
        Serial.println("Cycle aborted to early");
        RunStartupProcedure();
      }
    }
    previousPosition = currentPosition;
    break;
  case 3: //update testcycle timer, original test interval is 6x150ms pause 1x2.5s pause

    if (cycleCounter < 6)
    {
      newStartTime = millis() + 2000;      //150=original cycle
      cycleCounter++;

    }
    if (cycleCounter == 6)
    {
      newStartTime = millis() + 2500;      //2500=original cycle
      cycleCounter = 0;
      printClearance = true; //no need to deactivate, in the long pause is always enough time for this
    }
    Serial.print("TIMEOUT TIME [ms]:");
    Serial.println(millis() - timeoutStopwatch);
    timeoutStopwatch = millis();
    //printClearance = true; //deactivate for faster test rhythm
    Serial.println("STATE 3 OK");
    cycleState++;
    break;
  case 4: //print measured values
    GetTargetValuesNStoreNPrint();
    if (millis() > timeoutStopwatch + 150) //default 80 //wait a short while to let the tool go back to top
    {
      Serial.println("STATE 4 OK");
      cycleState++;
    }
    break;
  }

  if (cycleState == 5)
  {
    Serial.println("***** CYCLE COMPLETED *****");
    cycleState = 0;
    timeoutStopwatch = millis();
    knockoutCounter = 0;
  }
}
