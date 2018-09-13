#include "rgb_lcd.h"

rgb_lcd lcd;
unsigned long previousMillisLCD = 0; // Will store last time executed
const long intervalLCD= 5000; // Time in ms to switch between screens/parameters on LCD
bool showFirstScreen = true;
String tdsError;
String phError;

void setupLCD() {
  lcd.begin(16, 2);
  lcd.setRGB(0, 255, 0); // Default color is green
  
  //LCD show start message
  Serial.print("Start LCD");
  lcd.setCursor(0, 0);
  lcd.print("Demarrer...");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");
}

void loopLCD() {
  float tempValue = sensorValues[0];
  float phValue = sensorValues[1];
  int tdsValue = sensorValues[2];
  int orpValue = sensorValues[3];
  
  // Switch between pH/Temp and TDS/Turbid view on LCD
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillisLCD >= intervalLCD) {
    previousMillisLCD = currentMillis;
    lcd.clear();
    
    if (showFirstScreen) {
      lcd.setCursor(0, 0);
      lcd.print("pH:" + String(phValue) + phError);
      
      lcd.setCursor(0, 1);
      lcd.print("Temp:" + String(tempValue));
      lcd.print((char)223);
      lcd.print("C");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("TDS:" + String(tdsValue) + tdsError);
      
      lcd.setCursor(0, 1);
      lcd.print("ORP:" + String(orpValue));
    }
    
    if (showFirstScreen) {
      showFirstScreen = false;

      // Set the background color to red or green depending on drinking water safety ranges
      if (phValue < SAFETY_THRESHOLD_PH_LOWER || phValue > SAFETY_THRESHOLD_PH_UPPER) {
        lcd.setRGB(255, 0, 0); // Red
        phError = " (!)";
      } else {
        lcd.setRGB(0, 255, 0); // Green
        phError = "";
      }
    } else {
      showFirstScreen = true;

      if (tdsValue > SAFETY_THRESHOLD_TDS) {
        lcd.setRGB(255, 0, 0); // Red
        tdsError = " (!)";
      } else {
        lcd.setRGB(0, 255, 0); // Green
        tdsError = "";
      }
    }
  }
}

