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