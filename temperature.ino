// Source (partially): https://github.com/leouzz/Linkit-One-OneWire
#include <OneWire.h>

OneWire ds(TEMPERATURE_PIN);

float temperature = 0;

float getTempFromSensor(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];
  //Serial.println(addr[8]);
  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}

float fixTempSensor(float temp) {
  if(temp > 10 && temp < 100) { // Temperature between a realistic value (to filter out wrong values due to sensor errors) 
    return temp;
  } else {
    return 0;
  }
}

float getTemp() {
  float temperatureNew = getTempFromSensor();
  float fixTemp = fixTempSensor(temperatureNew);

  if (fixTemp != 0) { 
    temperature = fixTemp; // Only update the temperature if the sensor provides a realistic value
  }

  return temperature;
}

