void readPhotoresistors() {
  int photoresistorCounter = 0;
  for (byte pin = A8; pin <= A14; pin++) {
    LDR[photoresistorCounter] = analogRead(pin);
    photoresistorCounter++;
    delay(2);
  }
}
