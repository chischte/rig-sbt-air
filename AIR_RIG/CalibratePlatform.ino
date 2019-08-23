void CalibratePlatform()

{
  GetForceNPosition();

  digitalWrite(PWM_OUT_VALVE, HIGH); //LOW=outlet open // HIGH=outlet_closed
  digitalWrite(SLOW_UPLIFT_VALVE, HIGH); //LOW=inlet closed // HIGH=inlet_open

  while (totalForce < 100 || currentPosition > 10000)
  {
    GetForceNPosition();
  }

  digitalWrite(SLOW_UPLIFT_VALVE, LOW); //LOW=inlet closed // HIGH=inlet_open

  //-----------------------------------------------------------------------------
  min_position = currentPosition; // auto calibration of the sledges top position
  //-----------------------------------------------------------------------------

  digitalWrite(PWM_OUT_VALVE, LOW);  //low=outlet open // HIGH=outlet_closed
  Serial.println("PLATFORM CALIBRATION COMPLETED");
  delay(1000);
  Serial.print("Detected platform peak position: ");
  Serial.println(min_position);
  delay(4000);
}
