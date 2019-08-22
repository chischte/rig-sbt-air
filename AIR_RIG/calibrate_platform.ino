void calibrate_platform()

{
  get_force_n_position();

  digitalWrite(pwm_out_valve, HIGH); //LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
  digitalWrite(slow_uplift_valve, HIGH); //LOW=INLET CLOSED // HIGH=INLET_OPEN

  while (total_force < 100 || current_position > 10000)
  {
    get_force_n_position();
  }

  digitalWrite(slow_uplift_valve, LOW); //LOW=INLET CLOSED // HIGH=INLET_OPEN

  //*****************************************************************************
  min_position = current_position; //AUTO CALIBRATION OF THE SLEDGES TOP POSITION
  //*****************************************************************************

  digitalWrite(pwm_out_valve, LOW);  //LOW=OUTLET OPEN // HIGH=OUTLET_CLOSED
  Serial.println("PLATFORM CALIBRATION COMPLETED");
  delay(1000);
  Serial.print("Detected platform peak position: ");
  Serial.println(min_position);
  delay(4000);  //temporary delay
}
