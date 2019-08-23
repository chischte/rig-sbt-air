/*
 * *****************************************************************************
 * AIR_RIG
 * *****************************************************************************
 * Program to control the JK 1 Million Cycles Test Rig
 * *****************************************************************************
 * Michael Wettstein
 * März 2018, Zürich
 * *****************************************************************************
 * Rig Settings:
 * Motor Speed:-10000rpm
 * Mainpressure: 2.5bar (2.5bar minimum for trigger valve)
 * Vesselpressure: 2 bar
 * Open inlet throttle until force starts to overshoot, then turn back a bit.
 * *****************************************************************************
 */

#include <Controllino.h>

//*****************************************************************************
// DECLARATION OF VARIABLES / DATA TYPES
//*****************************************************************************
// boolean (true/false)
// byte (0-255)
// int   (-32,768 to 32,767) / unsigned int: 0 to 65,535
// long  (-2,147,483,648 to 2,147,483,647)
// float (6-7 Digits)
//*****************************************************************************

// KNOBS AND POTENTIOMETERS:
const byte TOGGLE_KNOB = 4;
const byte LINEAR_POT = A2;
const byte POTENTIOMETER = A4;
const byte START_BUTTON = A0;
const byte STOP_BUTTON = A1;
const byte GREEN_LIGHT_PIN = 5;
const byte CLICK_RELAY = 22;

// SENSORS:
const byte PRESSURE_SENSOR_UPPER_CHAMBER = A14;
const byte PRESSURE_SENSOR_LOWER_CHAMBER = A15;

// VALVES:
const byte HIGHSPEED_VALVE = 12;
const byte DECOMPRESSION_VALVE = 2;
const byte TRIGGER_VALVE = 3;
const byte PWM_OUT_VALVE = 6; // 400 Normliter
const byte PWM_IN_VALVE = 4;  // 200 Normliter
const byte SLOW_UPLIFT_VALVE = 7; // 400 Normliter
const byte STAPLER_RELAY = 27;

boolean machineRunning = false;
boolean printClearance = false;
boolean stepByStepMode = false;
boolean startupModeActive = true;
boolean valveCurveClearance = true;
boolean pwmInState = 0;
boolean pwmOutState = 1;
boolean highspeedMode;

byte printALotOfTimes;
byte cycleState;
byte knockoutCounter;

int current_array;

//-----------------------------------------------------------------------------
int min_position = 2222; ////in [mm/1000] //VALUE WILL BE AUTO CALIBRATED
//-----------------------------------------------------------------------------

int targetForce;
int printCounter;
int measuredValue[55];
int rawDataLowerSensor;
int rawDataUpperSensor;
int pneumaticForce; //[N]
int totalForce;
int pressureLowerChamber;
int cycleEndIndicator;
int cycleCounter;
const int highspeedValvePostion = 128;
const int maxAnalogValue = 813; // not 1024, because controllino goes up to 26.4V!

unsigned long runtimeStopwatch;
unsigned long serialPrintTimer;
unsigned long timeMarkTimer;
unsigned long pdDeltaT;
unsigned long newTime;
unsigned long timeoutStopwatch;
unsigned long newStartTime;

float interpolationFactor;
float kdFactor;
float currentPosition;
float valueLinearPot;
float max_position = min_position + 41000; // in [mm/1000] // 41mm = 42Arrays
float previousPosition;

//*****************************************************************************
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup()
{
  pinMode(STOP_BUTTON, INPUT);
  pinMode(START_BUTTON, INPUT);
  pinMode(LINEAR_POT, INPUT);
  pinMode(POTENTIOMETER, INPUT);
  pinMode(HIGHSPEED_VALVE, OUTPUT);
  pinMode(DECOMPRESSION_VALVE, OUTPUT);
  pinMode(TRIGGER_VALVE, OUTPUT);
  pinMode(GREEN_LIGHT_PIN, OUTPUT);
  pinMode(SLOW_UPLIFT_VALVE, OUTPUT);
  pinMode(PRESSURE_SENSOR_UPPER_CHAMBER, INPUT);
  pinMode(PRESSURE_SENSOR_LOWER_CHAMBER, INPUT);
  pinMode(CLICK_RELAY, OUTPUT);
  pinMode(PWM_OUT_VALVE, OUTPUT);
  pinMode(PWM_IN_VALVE, OUTPUT);
  pinMode(STAPLER_RELAY, OUTPUT);

  Serial.begin(115200); //start serial connection
  Serial.println("EXIT SETUP");
  digitalWrite(STAPLER_RELAY, HIGH);
}
//*****************************************************************************
//********************#*********#####***#####***######*************************
//********************#********#*****#*#*****#**#*****#************************
//********************#********#*****#*#*****#**######*************************
//********************#********#*****#*#*****#**#******************************
//********************#######***#####***#####***#******************************
//*****************************************************************************
void loop()
{
  if (machineRunning == false)
  {
    GetForceNPosition();
    GetTargetValuesNStoreNPrint();
  }

  //***************************************************************************
  // MAIN LOOP
  //***************************************************************************
  ToggleOnOff(); //switch machine ON/OFF

  if (machineRunning == false)
  {
    ///*
    //Serial.print("Move sledge up and set min_position value: ");
    Serial.print(0);
    Serial.print(",");
    Serial.print(44000);
    Serial.print(",");
    Serial.println(currentPosition, 0);
    delay(20);
    //*/
  }

  if (machineRunning == true)
  {

    if (startupModeActive == true)
    {
      RunStartupProcedure();
    }

    RunMainTestCylce();

    //***************************************************************************
    // TIMEOUT STOPWATCH - RESET TOOL - AUTO SHUTDOWN
    //***************************************************************************

    if (millis() - timeoutStopwatch > 3500)
    {
      TimeoutProcedure();
    }
  }

  //***************************************************************************
  // STOPWATCH TWO READ THE LENGTH OF A PROGRAMCYCLE
  //***************************************************************************
  /*
   long runtime = micros() - runtimeStopwatch;
   Serial.println(runtime);
   //delay(300);
   runtimeStopwatch = micros();
   */
}
