void ToggleOnOff() {

    //STOP TEST_RIG
    //*****************************************************************************
    if (digitalRead(STOP_BUTTON) == HIGH) {
        machineRunning = false;
        knockoutCounter = 0;
        //digitalWrite(STAPLER_RELAY, LOW);

    }
    if (machineRunning == false) {
        // SET HIGHSPEED VALVES OFF:
        digitalWrite(PWM_OUT_VALVE, LOW); //LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
        digitalWrite(PWM_IN_VALVE, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN
        // OPEN DECOMPRESSION VALVE:
        digitalWrite(DECOMPRESSION_VALVE, LOW); //in low state the valve vents are open
        digitalWrite(GREEN_LIGHT_PIN, LOW);
        digitalWrite(CLICK_RELAY, LOW);
        digitalWrite(TRIGGER_VALVE, LOW);

        //digitalWrite(STAPLER_RELAY, LOW);

        cycleState = 0;
    }

    // START TEST RIG
    //*****************************************************************************
    if (digitalRead(START_BUTTON) == HIGH) {
        machineRunning = true;
        cycleState = 0;
        knockoutCounter = 0;
        timeoutStopwatch = millis();
        digitalWrite(STAPLER_RELAY, HIGH);

    }
    if (machineRunning == true) {
        digitalWrite(GREEN_LIGHT_PIN, HIGH);
        digitalWrite(CLICK_RELAY, HIGH);
        //digitalWrite(TRIGGER_VALVE,HIGH);
    }
}
