/*
 * V17
 * Timeout Stoppuhr von den Zyklusdelayzeiten entkoppelt
 * Schlittenkalibrierung und Startup routine umstrukturiert und optimiert
 * Zusätzliche Zwischenschritt Serialprints eingefügt
 * 
 * V16
 * autokalibrierung von "min_position"
 * Rückwärtsschlag vermeiden bei Abort des Zyklus dur Toolausschaltung
 * 
 * V15
 * Programmstruktur optimiert (Unterprogramme erstellt)
 * Fehlersuche Stahlschlitten
 * 
 * V14 
 * Startup ablauf beinhaltet ein und ausschalten des Geräts
 * Testzyklus Rythmus korrigiert
 * Geschwindigkeit optimiert
 *
 * HISTORY
 * V12 kleinere Anpassungen
 * 
 * V11 Regelung funktioniert auch bei voller Geschwindigkeit!
 * Entfernen der Steuerung aus dem Programm
 * 
 * V10 - Wahlweise Steuerung oder Regelung
 * 
 * V09 - Steuerung
 * valve_curve() Funktion zum komfortablen erzeugen der Steuerkurve programmiert
 * funktioniert noch nicht.
 *  
 * V08 - Steuerung
 * besserer Printout mit skalierung der x achse
 * 
 * V07 - Steuerung
 * Motor auf 10000RPM eingestellt
 * Wechsel von Regelung (V06) auf Steuerung der Ventile, je nach Position
 * Testzyklus Rhytmus implementiert (6Zyklen mit 150ms Zwischenpause, dann 2.5s pause
 * 
 * V06  - Regelung
 * Minimaler tiefpunkt für Zyklusende von 37000 auf 35000 reduziert
 * Wichtig: "cycle_end_indicator" implementiert - erst ein wiederhohltes detektieren des Tiefpunktes 
 * beendet den Zyklus. Analoge Messfehler des Linearpotentiometers werden so ignoriert.
 *  * Version scheint sehr stabil zu laufen. 
 * Motorspeed 2500 läuft gut
 * Maximaldruck 2.6 Bar funktioniert gut
 *  
 */
 
int serial_prints()
{

  //*****************************************************************************
  //*****************************************************************************
  //SERIAL PRINT SECTION
  //*****************************************************************************
  //*****************************************************************************

  if (millis() - serialprinttimer > 1)
  {
    Serial.flush();

    Serial.print(0);
    Serial.print(",");
    Serial.print(+40);
    Serial.print(",");
    //Serial.print(128);
    Serial.print(",");

    Serial.print(current_position/1000, 2);
    //Serial.print(",");
    //Serial.print(interpolation_factor,2);
    //Serial.print(",");
    //Serial.print(current_array);
    //Serial.print(",");
    //Serial.print(target_force);
    //Serial.print(",");
    //Serial.print(highspeed_valve_position);
    //Serial.print(",");
    //Serial.print(pressure_lower_chamber);
    //Serial.print(",");
    //Serial.print(analogRead(pressure_sensor_lower_chamber));
    //Serial.print(",");
    //Serial.print(analogRead(linear_pot));
    //Serial.print(",");
    //Serial.print(total_force);
    //Serial.print(",");
    //Serial.print(pneumatic_force);
    //Serial.print(",");

    serialprinttimer = millis();
    /*
        //ADD A VISUAL "TIMEMARKSPIKE" TO TE SERIALPRINTER
        int timemarkrate = 1000; //AFTER THIS TIME A SPIKE APEARS
        if (millis() > timemarktimer)
        {
          Serial.print(50);
          timemarktimer = millis() + timemarkrate;
        }
        else
        {
          Serial.print (0);
        }
    */
    Serial.println(",");
    Serial.flush();
  }
}
