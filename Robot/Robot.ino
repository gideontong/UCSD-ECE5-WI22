#include <Wire.h>
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

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);

int M1Sp = 60;
int M2Sp = 60;

int LDR[PHOTORESISTOR_COUNT];

void readPhotoresistors() {
  int photoresistorCounter = 0;
  for (byte pin = A8; pin <= A14; pin++) {
    LDR[photoresistorCounter] = analogRead(pin);
    photoresistorCounter++;
    delay(2);
  }
}

void debugPhotoresistors() {
  for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
    Serial.print(LDR[i]);
    Serial.print(" ");
  }
  Serial.println();

  delay(200);
}

void calibratePhotoresistors() {
}

/**
 * A function that commands a specified motor to move towards a given direction at a given speed
 *
 * @param motor: a pointer to the motor we want to send movement directions to
 * @param speed: the speed of we want the motor to use
 * @param direction: the direction we want the motor to move in
 */
void runMotorAtSpeed(Adafruit_DCMotor *motor, int speed, int direction) {
  motor->setSpeed(speed);
  motor->run(direction);
}

void moveDirection(int direction) {
  switch (direction) {
    case M_FORWARD:
      runMotorAtSpeed(Motor1, M1Sp, FORWARD);
      runMotorAtSpeed(Motor2, M2Sp, FORWARD);
      break;
    case M_BACKWARD:
      runMotorAtSpeed(Motor1, M1Sp, BACKWARD);
      runMotorAtSpeed(Motor2, M2Sp, BACKWARD);
      break;
    case M_LEFT:
      runMotorAtSpeed(Motor1, M1Sp, FORWARD);
      runMotorAtSpeed(Motor2, M2Sp, RELEASE);
      break;
    case M_RIGHT:
      runMotorAtSpeed(Motor1, M1Sp, RELEASE);
      runMotorAtSpeed(Motor2, M2Sp, FORWARD);
      break;
    case M_STOP:
    default:
      runMotorAtSpeed(Motor1, M1Sp, RELEASE);
      runMotorAtSpeed(Motor2, M2Sp, RELEASE);
      break;
  }
}

void setup() {
  Serial.begin(9600);

  AFMS.begin();
  pinMode(LED_PIN, OUTPUT);
  pinMode(WHITE_PIN, OUTPUT);
  pinMode(BLACK_PIN, OUTPUT);

  Serial.println("Ready!");
}

void loop() {
  digitalWrite(WHITE_PIN, HIGH);
  digitalWrite(BLACK_PIN, HIGH);
  runMotorAtSpeed(Motor1, M1Sp, FORWARD);
  runMotorAtSpeed(Motor2, M2Sp, FORWARD);
  readPhotoresistors();
  debugPhotoresistors();
  delay(0.5);
}
