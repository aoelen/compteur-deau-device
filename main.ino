// Compteur d'eau code for LinkIt One

#include <LBattery.h> // Battery library, needed for getting battery level

// Settings
// Pins
#define TEMPERATURE_PIN 8
#define PH_PIN A2
#define TDS_PIN A1
#define ORP_PIN A0
#define TURBIDITY_PIN 2

// Drinking water standards
#define SAFETY_THRESHOLD_TDS 600
#define SAFETY_THRESHOLD_PH_LOWER 6.5
#define SAFETY_THRESHOLD_PH_UPPER 8.5

// Local storage
#define STORE_DATA_LOCALLY true      
#define Drv LSD // Storage location; LSD = SD card, LFlash = internal 10M flash memory

// Cloud storage 
#define CLOUD_STORAGE_IP "35.205.42.150"
#define CLOUD_STORAGE_PATH "/compteur-deau-prototype.php"

const String SENSOR_LABELS[] = {"temp", "ph", "tds", "orp", "battery-level", "battery-charging", "turbidity"};
const byte SENSOR_VALUE_AMOUNT = 7;

// WiFi settings
#define WIFI_ENABLE false
#define WIFI_AP "flits air b"
#define WIFI_PASSWORD "" 
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
const long INTERVAL_WIFI= 1000 * 60 * 1; // Interval (ms) in which data should be sent via WiFi

//GPRS settings
#define GPRS_ENABLE false
const char * APN = "multimedia.lebara.nl";
const char * LOGIN  ="";
const char * PASSWORD = "";
const long INTERVAL_GPRS= 1000 * 60 * 1; // Interval (ms) in which data should be sent via GPRS

//GPS settings
const long INTERVAL_GPS = 1000 * 60 * 1; // Interval (ms) in which GPS coordinates need to be reloaded 

// Global variables
float sensorValues[SENSOR_VALUE_AMOUNT];
bool GPRSConnection;
int loopAmount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting...");

  setupLCD();
  setupLocalStorage();
  setupWifi();
  setupTurbidity();
  
  if (GPRS_ENABLE) {
    GPRSConnection = setupGPRS();
  }
}

void loop() {
  loopAmount++;

  // Get all sensor values
  float tempValue = getTemp();
  float phValue = loopPH();
  float tdsValue = loopTDS();
  int orpValue = loopOrp();
  int turbidityValue = loopTurbidity();
  int batteryLevel = LBattery.level();
  int batteryCharging = LBattery.isCharging();

  // Put them in an array for storing it later
  sensorValues[0] = tempValue;
  sensorValues[1] = phValue;
  sensorValues[2] = tdsValue;
  sensorValues[3] = orpValue;
  sensorValues[4] = batteryLevel;
  sensorValues[5] = batteryCharging;
  sensorValues[6] = turbidityValue;

  // Execture loops for components 
  storeData();
  loopLCD();
  loopWifi();

  if (GPRSConnection) { 
    loopGPRS();
  }

  // Serial print only for debugging 
  for(int j=0; j<SENSOR_VALUE_AMOUNT; j++) {
      Serial.print(SENSOR_LABELS[j] + ": " + sensorValues[j] + ";");
  }
  Serial.println();
  
  // Give the sensors some time to initialize
  if (loopAmount > 200) { 
    delay(1000 * 60 * 15);
  } 
}
