void calibratePhotoresistors() {
  int numberOfMeasurements = 15;

  // Black
  Serial.println("\nNow calibrating black.");
  digitalWrite(BLACK_PIN, HIGH);
  digitalWrite(WHITE_PIN, LOW);
  delay(3000);
  CalibrateHelper(numberOfMeasurements, true);
  
  // White
  Serial.println("Now calibrating white.");
  digitalWrite(BLACK_PIN, LOW);
  digitalWrite(WHITE_PIN, HIGH);
  delay(2000);
  CalibrateHelper(numberOfMeasurements, false);

  digitalWrite(WHITE_PIN, LOW);

  if (!CALIBRATEEVERYTIME)
    EEPROMCalibration();

  Serial.print("Black Vals:  ");
  for (int i = 0; i < totalPhotoResistors; i++)
    Serial.print(String(Mx[i]) + " ");
  Serial.println();

  Serial.print("White Vals:  ");
  for (int i = 0; i < totalPhotoResistors; i++)
    Serial.print(String(Mn[i]) + " ");
  Serial.println();

  Serial.print("Delta Vals:  ");
  for (int i = 0; i < totalPhotoResistors; i++)
    Serial.print(String(Mx[i] - Mn[i]) + " ");
  Serial.println();

  digitalWrite(WHITE_PIN, HIGH);
  digitalWrite(BLACK_PIN, HIGH);
}

void EEPROMCalibration() {
  if (abs(Mn[3] - Mx[3]) > 50) {
    Serial.println("Saving to memory...");
    for (int i = 0; i < totalPhotoResistors*2; i++) {
      if (i < totalPhotoResistors)
        EEPROM.write(i, Mx[i] / 4);
      else
        EEPROM.write(i, Mn[i - totalPhotoResistors] / 4);
      delay(200);
    }
  }
  else {
    Serial.println("Reading from memory...");
    for (int i = 0; i < totalPhotoResistors*2; i++) {
      if (i < totalPhotoResistors)
        Mx[i] = (EEPROM.read(i) * 4);
      else
        Mn[i - totalPhotoResistors] = (EEPROM.read(i) * 4);
      delay(125);
    }
  }
}

void CalibrateHelper(int numberOfMeasurements, boolean ifCalibratingBlack) {
  for (int i = 0; i < numberOfMeasurements; i++) {
    for (int pin = 0; pin < totalPhotoResistors; pin++) {
      LDRf[pin] = LDRf[pin] + (float)analogRead(LDR_Pin[pin]);
      delay(2);
    }
    Serial.print(". ");
  }
  for (int pin = 0; pin < totalPhotoResistors; pin++) {
    if (ifCalibratingBlack) {                                   // updating cooresponding array based on if we are calibrating black or white
      Mx[pin] = round(LDRf[pin] / (float)numberOfMeasurements); // take average and store for black
    }
    else {
      Mn[pin] = round(LDRf[pin] / (float)numberOfMeasurements); // take average and store for white
    }
    LDRf[pin] = 0.0;
  }

  Serial.println(" Done!");
}
