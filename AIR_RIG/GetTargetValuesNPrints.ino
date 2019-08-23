void GetTargetValuesNStoreNPrint()
{

  //*****************************************************************************
  // CALCULATE DESIRED TARGET FORCE
  //*****************************************************************************

  int arraysize = 42;
  //*****************************************************************************
  // IDEALIZED INSTRON TARGET FORCE VALUES
  // ADAPTED VALUES TO FIT STROKELENGTH
  // ESCON COMPARISON OPTIMIZED
  //*****************************************************************************
  int target_values[55] =
  {  //not defined arrays will be zero)
      10, 10, 10, 10, 15, 20, 25, 40, 60, 90, 110, 130, 160, 190, 210, 250, 260,
          260, 260, 270, 280, 300, 330, 370, 440, 520, 570, 590, 600, 600, 600,
          600, 600, 625, 700, 730, 800, 800, };
  /*
   * *****************************************************************************
   * ORIGINAL VALUES FROM EXCEL BASED ON INSTRON MEASUREMENTS
   * *****************************************************************************
   * Data from Excel Projekt\Testrig\Worstcase Hüllkurven-xlsx
   * 42 Werte, Hub-mm 0-41:
   20,   40,  70,  90, 120, 130, 140, 150, 160, 170,
   185, 200, 220, 235, 250, 260, 260, 260, 260, 260,
   260, 260, 260, 270, 280, 300, 330, 370, 440, 520,
   570, 590, 600, 600, 590, 580, 590, 625, 710, 780,
   840, 880,
   */

  current_array = map(currentPosition, min_position, max_position, 0,
      arraysize - 1); //maps the current sledge position to the force arrayvalue
  //*****************************************************************************
  // INTERPOLATION BETWEEN THE FORCE STEPS
  //*****************************************************************************
  // float mm_per_array = (max_position - min_position) / (arraysize - 1);// unit [mm/1000]
  float mm_per_array = 1000;  // unit [mm/1000]
  interpolationFactor = (currentPosition - (current_array * mm_per_array))
      / mm_per_array; // calculates the relative position in an array

  targetForce = (target_values[current_array + 1]
      - target_values[current_array]) * interpolationFactor
      + target_values[current_array];

  //*****************************************************************************
  // STORE MEASURED FORCE VALUES IN AN ARRAY
  //*****************************************************************************
  measuredValue[current_array] = totalForce;

  //*****************************************************************************
  // PRINT VALUES
  //*****************************************************************************
  int x_axis_1_spiker = 0;
  int x_axis_5_spiker = 0;
  int x_axis_10_spiker = 0;

  while (printClearance == true)
  {
    Serial.print(target_values[printCounter]);
    Serial.print(",");
    Serial.print(measuredValue[printCounter]);
    Serial.print(",");

    int spikeheight = 0;

    if (printCounter == x_axis_1_spiker)
    {
      spikeheight = 50;
      x_axis_1_spiker = printCounter + 1;
    }

    if (printCounter == x_axis_5_spiker)
    {
      spikeheight = 150;
      x_axis_5_spiker = printCounter + 5;
    }
    if (printCounter == x_axis_10_spiker)
    {
      spikeheight = 250;
      x_axis_10_spiker = printCounter + 10;
    }

    Serial.print(spikeheight);

    Serial.println(",");

    if (printALotOfTimes == 10)
    {
      printCounter++;
      if (printCounter == arraysize)
      {
        printCounter = 0;
        printClearance = false;
      }
      printALotOfTimes = 0;
    }
    printALotOfTimes++;

  }
}
