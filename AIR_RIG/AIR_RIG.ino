/*
 * *****************************************************************************
 * AIR_RIG
 * *****************************************************************************
 * Program to control the JK 1 Million Cycles Test Rig
 * *****************************************************************************
 * Michael Wettstein
 * März 2018, Zürich
 * *****************************************************************************
 */

//*****************************************************************************
//PRE-SETUP SECTION / PIN LAYOUT
//*****************************************************************************
#include <Controllino.h> /* Usage of CONTROLLINO library allows you to use CONTROLLINO_xx aliases in your sketch. */
//*****************************************************************************
//KNOBS AND POTENTIOMETERS
//*****************************************************************************
#define toggle_knob 4
#define linear_pot A2
#define potentiometer A4
#define start_button A0
#define stop_button A1
#define green_light_pin 5
#define click_relay 22
//*****************************************************************************
//SENSORS
//*****************************************************************************
#define pressure_sensor_upper_chamber A14
#define pressure_sensor_lower_chamber A15
//*****************************************************************************
//VALVES
//*****************************************************************************
#define highspeed_valve 12
#define decompression_valve 2
#define trigger_valve 3
//define locking_valve 4
#define pwm_out_valve 6 //400 Normliter
#define pwm_in_valve 4  //200 Normliter
#define slow_uplift_valve 7 //400 Normliter
#define stapler_relay 27
//*****************************************************************************
//DECLARATION OF VARIABLES / DATA TYPES
//*****************************************************************************
//boolean (true/false)
//byte (0-255)
//int   (-32,768 to 32,767) / unsigned int: 0 to 65,535
//long  (-2,147,483,648 to 2,147,483,647)
//float (6-7 Digits)
//*****************************************************************************
boolean machine_running = false;
boolean printclearance = false;
boolean step_by_step_mode = false;
boolean startupmode_active = true;
boolean valve_curveclearance = true;
boolean pwm_in_state = 0;
boolean pwm_out_state = 1;
boolean highspeed_mode;

byte print_a_lot_of_times;
byte cycle_state;
byte knockout_counter;

int current_array;

//§#§@#§@#§@#§@#§@#§@#§@#§@#§@#
//@#§@#§@#§@#§@#§@#§@#§@#
//@#§@#§@#§@#§@#§@#

int min_position = 2222; ////in [mm/1000] //VALUE WILL BE AUTO CALIBRATED

//@#§@#§@#§@#§@#§@#
//@#§@#§@#§@#§@#§@#§@#§@#
//@#§@#§@#§@#§@#§@#§@#§@#§@#§@#

int target_force;
int printcounter;
int highspeed_valve_position = 128;
int max_analog_value = 813; //not 1024, because controllino goes up to 26.4V!
int measured_values[55];
int rawdata_lower_sensor;
int rawdata_upper_sensor;
int pneumatic_force; //[N]
int kp_max = 1500; //[RPM /(°/10)] use this value to scale the regulator potentiometer to desired range
int kp_factor;
int total_force;
int pressure_lower_chamber;
int cycle_end_indicator;
int cycle_counter;

unsigned long runtime_stopwatch;
unsigned long serialprinttimer;
unsigned long previoustime;
unsigned long timemarktimer;
unsigned long pd_delta_t;
unsigned long newtime;
unsigned long timeout_stopwatch;
unsigned long new_starttime;

float interpolation_factor;
float exact_position;
float kd_factor;
float current_position;
float value_linear_pot;
float force_error;
float previous_force_error;
float force_error_speed;
float max_position = min_position + 41000; //in [mm/1000] // 41mm = 42Arrays
float previous_position;

//*****************************************************************************
//******************######**#######*#######*#******#*######********************
//*****************#********#**********#****#******#*#*****#*******************
//******************####****#####******#****#******#*######********************
//***********************#**#**********#****#******#*#*************************
//*****************######***######*****#*****######**#*************************
//*****************************************************************************
void setup()
{
  //***************************************************************************
  pinMode(stop_button, INPUT);
  pinMode(start_button, INPUT);
  pinMode(linear_pot, INPUT);
  pinMode(potentiometer, INPUT);
  pinMode(highspeed_valve, OUTPUT);
  pinMode(decompression_valve, OUTPUT);
  pinMode(trigger_valve, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(slow_uplift_valve, OUTPUT);
  pinMode(pressure_sensor_upper_chamber, INPUT);
  pinMode(pressure_sensor_lower_chamber, INPUT);
  pinMode(click_relay, OUTPUT);
  pinMode(pwm_out_valve, OUTPUT);
  pinMode(pwm_in_valve, OUTPUT);
  pinMode(stapler_relay, OUTPUT);
  //***************************************************************************
  Serial.begin(115200); //start serial connection
  Serial.println("EXIT SETUP");
  digitalWrite(stapler_relay, HIGH);
}
//*****************************************************************************
//*****************************************************************************
//********************#*********#####***#####***######*************************
//********************#********#*****#*#*****#**#*****#************************
//********************#********#*****#*#*****#**######*************************
//********************#********#*****#*#*****#**#******************************
//********************#######***#####***#####***#******************************
//*****************************************************************************
//*****************************************************************************
void loop()
{

  //§#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#
  //§#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#
  //§#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#
  //Settings: Motor Speed:-10000rpm
  //Mainpressure: 2.5bar (2.5bar minimum for trigger valve)
  //Vesselpressure: 2 bar
  //open inlet throttle until force starts to overshoot, then turn back a bit
  //§#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#
  //§#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#
  //§#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#§@#

  if (machine_running == false)
  {
    get_force_n_position();
    get_target_values_n_store_n_print();
  }

  //***************************************************************************
  //ACTIVATE / DEACTIVATE SERIAL PRINTS FOR MONITORING AND DEBUGGING
  //***************************************************************************
  //serial_prints();

  //***************************************************************************
  //MAIN LOOP
  //***************************************************************************
  toggle_on_off(); //switch machine ON/OFF

  if (machine_running == false)
  {
    ///*
    //Serial.print("Move sledge up and set min_position value: ");
    Serial.print(0);
    Serial.print(",");
    Serial.print(44000);
    Serial.print(",");
    Serial.println(current_position, 0);
    delay(20);
    //*/
  }

  if (machine_running == true)
  {

    if (startupmode_active == true)
    {
      run_startup_procedure();
    }

    run_main_test_cycle();

    //***************************************************************************
    //TIMEOUT STOPWATCH - RESET TOOL - AUTO SHUTDOWN
    //***************************************************************************

    if (millis() - timeout_stopwatch > 3500)
    {
      timeout_procedure();
    }
  }

  //***************************************************************************
  //STOPWATCH TWO READ THE LENGTH OF A PROGRAMCYCLE
  //***************************************************************************
  /*
   long runtime = micros() - runtime_stopwatch;
   Serial.println(runtime);
   //delay(300);
   runtime_stopwatch = micros();
   */
  //***************************************************************************
}
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
