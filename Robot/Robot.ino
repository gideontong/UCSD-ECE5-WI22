#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_MotorShield.h>

#define LED_PIN 13
#define BLACK_PIN 22
#define WHITE_PIN 24
#define PHOTORESISTOR_COUNT 7
#define CALIBRATION_COUNT 15
#define M_FORWARD 0
#define M_BACKWARD 1
#define M_LEFT 2
#define M_RIGHT 3
#define M_STOP 4

#define CALIBRATEEVERYTIME 1
#define DETECTFALLOFFBOARD 1  // Detects robot falling off the board and immediately stops all motors
#define PRINTALLDATA 1
#define NOMINALSPEED 30       // This is the base speed for both motors, can also be increased by using potentiometers

// Potentiometer Pins
int LDR_Pin[] = { A8, A9, A10, A11, A12, A13, A14 };
int led_Pins[] = { 13, 2, 3, 4, 5, 6, 7, 8 };
const int S_pin = A0;  // Pin connected to Speed potentiometer
const int P_pin = A1;  // Pin connected to P term potentiometer
const int I_pin = A2;  // Pin connected to I term potentiometer
const int D_pin = A3;  // Pin connected to D term potentiometer

//Variables for PID Calculations
int MxRead, MxIndex, CriteriaForMax;
int leftHighestPR, highestPResistor, rightHighestPR;
float AveRead, WeightedAve;

int M1SpeedtoMotor, M2SpeedtoMotor;

// For Motor Control
int Turn = 1, M1P = 0, M2P = 0;
float error, lasterror = 0, sumerror = 0;
float kP = 1, kI = 1, kD = 1;
int rogueRobotCount = 0;

int SpRead = 0;  // speed increase
int kPRead = 0;  // proportional gain
int kIRead = 0;  // integral gain
int kDRead = 0;  // derivative gain

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Motor1 = AFMS.getMotor(2);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(1);

int M1Sp = 60;
int M2Sp = 60;

int LDR[PHOTORESISTOR_COUNT];

float Mn[20];
float Mx[20];
float LDRf[20];
int rawPResistorData[20];
int totalPhotoResistors = sizeof(LDR_Pin) / sizeof(LDR_Pin[0]);
int numLEDs = sizeof(led_Pins) / sizeof(led_Pins[0]);

void setup() {
  Serial.begin(9600);

  AFMS.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);
  pinMode(BLACK_PIN, OUTPUT);

  calibratePhotoresistors();
  RunMotors();
  sanityCheck();

  Serial.println("Ready!");
}

void loop() {
  ReadPhotoResistors();  // Read photoresistors
  SpRead = 1;
  kPRead = 8;
  kIRead = 0;
  kDRead = 0;

  CalcError();  // Calculates error

  PID_Turn();  // PID Control and Output to motors to turn

  if (DETECTFALLOFFBOARD && determineRogueRobot())
    haltMotors();  // Only if DETECTFALLOFFBOARD is enabled AND Robot is off the track will we halt the motors.
  else
    RunMotors();  // Runs motors

  if (PRINTALLDATA)  // If PRINTALLDATA Enabled, Print all the data
    debugAll();
}
