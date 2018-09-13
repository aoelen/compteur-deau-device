/*
  Web client

 This sketch connects to a website 
 using Wi-Fi functionality on MediaTek LinkIt platform.

 Change the macro WIFI_AP, WIFI_PASSWORD, WIFI_AUTH and SITE_URL accordingly.

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe
 modified 20 Aug 2014
 by MediaTek Inc.
 */
 
#include <LWiFi.h>
#include <LWiFiClient.h>

LWiFiClient c;
unsigned long previousMillisWifi = 0; // Will store last time executed

void setupWifi()
{
  if (!WIFI_ENABLE) {
    return;
  }

  LWiFi.begin();

  Serial.print("Connecting to AP...");
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(1000);
  }
  Serial.println("done");
}

void loopWifi() {
  if (!WIFI_ENABLE) {
    return;
  }

  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillisWifi>= INTERVAL_WIFI) {
    previousMillisWifi = currentMillis;
    
    Serial.println("send HTTP GET request");
    c.print("GET " + (String)CLOUD_STORAGE_PATH);

    String queryString = generateSensorQueryString();
    
    c.print(queryString);
    
    c.println(" HTTP/1.1");
    c.println("Host: " CLOUD_STORAGE_IP);
    c.println("Connection: close");
    c.println();
    c.stop();
    delay(1000);
  
    Serial.println("Connecting to Website");
    while (0 == c.connect(CLOUD_STORAGE_IP, 80))
    {
      Serial.println("Re-Connecting to Website");
      delay(1000);
    }
  }
}
