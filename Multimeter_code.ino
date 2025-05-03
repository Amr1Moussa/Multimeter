#include <LiquidCrystal.h>

// LCD setup
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Analog pins
const int pinVoltageDivider = A5; // For resistance
const int pinCurrent = A1;
const int pinVoltage = A2;
const int pinCapacitance = A0;    // For capacitance

// DIP switch pins
const int swCurrent = 9;
const int swVoltage = 10;
const int swResistance = 11;
const int swCapacitance = 12;

// Constants
const float Vin = 5.0;
const float knownResistor = 950.0;
const float shuntResistor = 0.5;
const float resistorRatio = 1000.0 / 11000.0;
const int chargePin = 13;
const int dischargePin = 8;
const int rValue = 10000; // 10kΩ resistor for capacitance
const int numReadings = 10;

void setup() {
  Serial.begin(9600);

  pinMode(pinVoltageDivider, INPUT);
  pinMode(pinCurrent, INPUT);
  pinMode(pinVoltage, INPUT);
  pinMode(pinCapacitance, INPUT);

  pinMode(swCurrent, INPUT);
  pinMode(swVoltage, INPUT);
  pinMode(swResistance, INPUT);
  pinMode(swCapacitance, INPUT);

  pinMode(chargePin, OUTPUT);
  pinMode(dischargePin, OUTPUT);
  digitalWrite(chargePin, LOW);
  digitalWrite(dischargePin, LOW);

  lcd.begin(16, 2);
  lcd.print("Multimeter Ready");
  delay(2000);
}

void loop() {
  float Vout = 0, R_unknown = -1, current_mA = 0, voltage = 0;

  for (int i = 0; i < numReadings; i++) {
    Vout += analogRead(pinVoltageDivider) * (Vin / 1023.0);
    float Vshunt = analogRead(pinCurrent) * (Vin / 1023.0);
    current_mA += (Vshunt / shuntResistor) * 1000.0;
    voltage += analogRead(pinVoltage) * (Vin / 1023.0) / resistorRatio;
  }

  Vout /= numReadings;
  current_mA /= numReadings;
  voltage /= numReadings;

  if (Vout > 0 && Vout < Vin) {
    R_unknown = ((Vin - Vout) * knownResistor) / Vout;
  }

  lcd.clear();
  lcd.setCursor(0, 0);

  if (digitalRead(swCurrent) == HIGH) {
    lcd.print("Current:");
    lcd.setCursor(0, 1);
    lcd.print(current_mA * .23, 1);
    lcd.print(" mA");
    Serial.print("Current: ");
    Serial.println(current_mA * 0.9, 1);
  } else if (digitalRead(swVoltage) == HIGH) {
    lcd.print("Voltage:");
    lcd.setCursor(0, 1);
    lcd.print(voltage * 1.03, 2);
    lcd.print(" V");
    Serial.print("Voltage: ");
    Serial.println(voltage * 1.059, 2);
  } else if (digitalRead(swResistance) == HIGH) {
    lcd.print("Resistance:");
    lcd.setCursor(0, 1);
    if (R_unknown > 0 && R_unknown < 1e6) {
      if (R_unknown > 1050) {
        lcd.print(R_unknown / 1000, 2);
        lcd.print(" k ohm");
        Serial.print("Resistance: ");
        Serial.println(R_unknown / 1000, 2);
      } else {
        lcd.print(R_unknown, 0);
        lcd.print(" Ohm");
        Serial.print("Resistance: ");
        Serial.println(R_unknown, 0);
      }
    } else {
      lcd.print("Out of range");
      Serial.println("Resistance: Out of range");
    }
  } else if (digitalRead(swCapacitance) == HIGH) {
    lcd.print("Capacitance:");
    lcd.setCursor(0, 1);

    digitalWrite(chargePin, LOW);
    digitalWrite(dischargePin, HIGH);
    delay(500);
    digitalWrite(dischargePin, LOW);

    digitalWrite(chargePin, HIGH);
    unsigned long startTime = micros();
    while (analogRead(pinCapacitance) < 648) {}
    unsigned long elapsedTime = micros() - startTime;
    digitalWrite(chargePin, LOW);

    float capacitance = (float)elapsedTime / rValue * 1e6;

    float avgCap = 0;
    for (int i = 0; i < 3; i++) {
      avgCap += capacitance;
      delay(50);
    }
    avgCap /= 3000000;

    lcd.print(avgCap, 2);
    lcd.print(" uF");
    Serial.print("Capacitance: ");
    Serial.println(avgCap, 2);
  } else {
    lcd.print("Select Mode:");
    lcd.setCursor(0, 1);
    lcd.print("1I  2V  3R  4C");
    Serial.println("Select Mode: 1=I, 2=V, 3=R, 4=C");
  }
  delay(1000);
}