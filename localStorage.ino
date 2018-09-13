// Libraries for accessing local storage 
#include <LFlash.h>
#include <LSD.h>
#include <LStorage.h>

void setupLocalStorage() {
  if (!STORE_DATA_LOCALLY) {
    return;
  }
  
  Serial.print("Init SD card...");
  pinMode(10, OUTPUT); // Set SPI pin to output, just to be sure 
  Drv.begin();
  
  Serial.println("done");
}

void storeData() {
  if (!STORE_DATA_LOCALLY) {
    return;
  }

  LFile dataFile = Drv.open("log.txt", FILE_WRITE);
  
  if (dataFile) {
    String line;
    
    for(int j=0; j<SENSOR_VALUE_AMOUNT; j++) { // Loop for creating one line of all sensor data
      line += SENSOR_LABELS[j] + "; " + sensorValues[j] + ";";
    }
    dataFile.println(line);
    dataFile.close();
  }
}
