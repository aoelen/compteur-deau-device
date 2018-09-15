# Compteur-deau code

_Tip: The landing page provides more information about the Compteur d'eau: https://aoelen.github.io/compteur-deau/_

## About Compteur d'eau
The code in this repository is written for the _Compteur d'eau_ device. This is a low-cost water quality monitoring device, built to operate within an ICT4D context in rural Africa. The device is using the LinkIt One, which is based on the Arduino platform. In order to run the code in this repository, a LinkIt One is required. The Arduino IDE can be used to compile and install the software on the LinkIt One. A dashboard to view the sensor data has been developed in PHP and can be found via: https://github.com/aoelen/compteur-deau

In order to run the code on a LinkIt One, the following libraries are needed: 
	- LinkIt One default libraries 
	- LCD library: https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight
	- OneWire library: https://github.com/leouzz/Linkit-One-OneWire

## System settings 
The settings can be changed in the `main.ino` file. 

| Type                      | Description                                                                                                                                           | Default value                                                                  |
|---------------------------|-------------------------------------------------------------------------------------------------------------------------------------------------------|--------------------------------------------------------------------------------|
| TEMPERATURE_PIN           | Digital pin on the LinkIt One where the temperature sensor is connected to                                                                            | 8                                                                              |
| PH_PIN                    | Analog pin on the LinkIt One where the pH sensor is connected to                                                                                      | A2                                                                             |
| TDS_PIN                   | Analog pin on the LinkIt One where the pH sensor is connected to                                                                                      | A1                                                                             |
| ORP_PIN                   | Analog pin on the LinkIt One where the ORP sensor is connected to                                                                                     | A0                                                                             |
| TURBIDITY_PIN             | Digital pin on the LinkIt One where the Turbidity sensor is connected to                                                                              | 2                                                                              |
| SAFETY_THRESHOLD_TDS      | The drinking water standard with respect to TDS, used for the LCD screen. When TDS is higher than threshold, a warning appears                        | 600                                                                            |
| SAFETY_THRESHOLD_PH_LOWER | The drinking water standard with respect to PH, used for the LCD screen. A warning appears when the pH is below this threshold value                  | 6.5                                                                            |
| SAFETY_THRESHOLD_PH_UPPER | The drinking water standard with respect to PH, used for the LCD screen. A warning appears when the pH is above this threshold value                  | 8.5                                                                            |
| STORE_DATA_LOCALLY        | When set to true, sensor data is saved to local storage                                                                                               | true                                                                           |
| Drv                       | If STORE_DATA_LOCALLY = true, select the location where the data should be stored. LSD = SD card, LFlash = internal 10M flash memory                  | LSD                                                                            |
| CLOUD_STORAGE_IP          | IP address or hostname of the cloud server storage                                                                                                    | 35.205.42.150                                                                  |
| CLOUD_STORAGE_PATH        | Path of the cloud server storage                                                                                                                      | /compteur-deau-prototype.php                                                   |
| SENSOR_LABELS             | The labels of the sensor values that need to be registered                                                                                            | {"temp", "ph", "tds", "orp", "battery-level", "battery-charging", "turbidity"} |
| SENSOR_VALUE_AMOUNT       | )The amount of sensor values that need to be registered (usually the amount of sensor labels provided in SENSOR_LABELS                                | 7                                                                              |
| WIFI_ENABLE               | Enable sending sensor data via WiFi to the cloud server                                                                                               | false                                                                          |
| WIFI_AP                   | WiFi access point name                                                                                                                                | -                                                                              |
| WIFI_PASSWORD             | WiFi password                                                                                                                                         | -                                                                              |
| WIFI_AUTH                 | WiFi authentication type. Choose from: LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP                                                                            | LWIFI_WPA                                                                      |
| INTERVAL_WIFI             | The interval of sending the data via WiFi. Provide the amount in milliseconds. The smaller the interval, the more battery is used                     | 900.000                                                                        |
| GPRS_ENABLE               | Enable sending sensor data via GPRS to the cloud server                                                                                               | true                                                                           |
| APN                       | Acces Point Name of GPRS provider                                                                                                                     | -                                                                              |
| LOGIN                     | Login of GPRS provider                                                                                                                                | -                                                                              |
| PASSWORD                  | Password of GPRS provider                                                                                                                             | -                                                                              |
| INTERVAL_GPRS             | The interval of sending the data via GPRS. Provide the amount in milliseconds. The smaller the interval, the more battery is used                     | 900.000                                                                        |
| INTERVAL_GPS              | The interval in which the GPS coordinates need to be reloaded. Provide the amount in milliseconds. The smaller the interval, the more battery is used | 900.000                                                                        |
