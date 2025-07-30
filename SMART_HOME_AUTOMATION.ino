//DHARANI CREATIONS

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD address may be 0x3F or 0x27

// Sensor Pins
const int smokeAOPin = A0;     // MQ2 Analog
const int smokeDOPin = 8;      // MQ2 Digital
const int ldrSensor = A1;
const int flameSensor = 2;
const int rainSensor = 3;

// Output Pins
const int buzzer = 4;
const int lightLED = 5;
const int alertLED = 6;

void setup() {
  // Inputs
  pinMode(smokeAOPin, INPUT);
  pinMode(smokeDOPin, INPUT);
  pinMode(ldrSensor, INPUT);
  pinMode(flameSensor, INPUT);
  pinMode(rainSensor, INPUT);

  // Outputs
  pinMode(buzzer, OUTPUT);
  pinMode(lightLED, OUTPUT);
  pinMode(alertLED, OUTPUT);

  lcd.init(); 
  lcd.backlight();
  
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print(" Smart Home ");
  lcd.setCursor(0, 1);
  lcd.print(" Initializing ");
  delay(2000);
  lcd.clear();
}

void loop() {
  int smokeValue = analogRead(smokeAOPin);       // MQ2 analog
  bool gasDetected = digitalRead(smokeDOPin) == LOW; // MQ2 digital
  int ldrValue = analogRead(ldrSensor);          
  bool flameDetected = digitalRead(flameSensor) == LOW;
  bool rainDetected = digitalRead(rainSensor) == LOW;

  Serial.print("Smoke: "); Serial.print(smokeValue);
  Serial.print(" | Gas DO: "); Serial.print(gasDetected);
  Serial.print(" | LDR: "); Serial.print(ldrValue);
  Serial.print(" | Flame: "); Serial.print(flameDetected);
  Serial.print(" | Rain: "); Serial.println(rainDetected);

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smoke:");
  lcd.print(smokeValue);

  lcd.setCursor(9, 0);
  lcd.print(ldrValue < 500 ? "Light" : "Dark");

  lcd.setCursor(0, 1);
  if (flameDetected) {
    lcd.print("FLAME ALERT!");
  } else if (gasDetected || smokeValue > 400) {
    lcd.print("Gas/Smoke Detected");
  } else if (rainDetected) {
    lcd.print("Rain Warning");
  } else {
    lcd.print("All Normal");
  }

  // Alerts
  if (gasDetected || smokeValue > 200 || flameDetected) {
    digitalWrite(buzzer, HIGH);
    digitalWrite(alertLED, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
    digitalWrite(alertLED, LOW);
  }

  // Light control using LDR
  if (ldrValue < 500) {
    digitalWrite(lightLED, LOW); // Turn on white LED if dark
  } else {
    digitalWrite(lightLED, HIGH);  // Turn off in daylight
  }

  delay(1000);
}
