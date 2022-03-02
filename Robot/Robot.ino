/* Photoresistor Code UCSD ECE 5 Lab 4 */
/* Variables for Light Sensors*/

#define totalPhotoResistors 7 // Defining how many photoresistors are used, modify this if more or less are used


#include <Wire.h>
#include <Adafruit_MotorShield.h>

#define LED_PIN 13

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

// Initialize Photo Resistor Array
int LDR[totalPhotoResistors]; // Since there are 7 photoresistors, we will number them from 0 to 6 (recall array index start with 0)

// ************************************************************************************************* //
/**
 * Function to read photo resistors
 */
void ReadPhotoResistors() {
  int photoResistorCounter = 0;
  // looping through analog pins A8 to A14 and storing their values into our LDR array
  for (byte pin = A8; pin <= A14; pin++) {
    LDR[photoResistorCounter] = analogRead(pin);
    photoResistorCounter++;
    delay(2);
  }
}

// ************************************************************************************************* //
/**
 * Function to print values of interest
 */
void Print() {
  for (int i = 0; i < totalPhotoResistors; i++) {
    // Printing the photo resistor reading values one by one
    Serial.print(LDR[i]);
    Serial.print(" ");
  }
  Serial.println(); // This just prints a new line, allowing the next set of readings to be on a new line

  delay(200); //just here to slow down the output for easier reading if desired
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

void setup()
{
  Serial.begin(9600);

  AFMS.begin();
  pinMode(LED_PIN, OUTPUT);

  Serial.println("Ready!");
}

void loop()
{
    ReadPhotoResistors(); // Read photoresistors and map to 0-100 based on calibration

  Print(); // Print values to serial monitor
}
