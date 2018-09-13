// Code partially from: https://docs.labs.mediatek.com/resource/linkit-one/en/tutorials/car-tracker

#include <LGSM.h>
#include <LGPS.h>
#include <LGPRS.h>
#include <LGPRSClient.h>
#include "vmthread.h"
#include <LTask.h>

//Endpoint settings
//char server[] = "35.205.42.150";
//char path[] = "/compteur-deau-prototype.php";
//int port = 80; // HTTP

//GPRS settings
LGPRSClient client;

//GPS
gpsSentenceInfoStruct info;
char buff[256];
char lat[100];
char lng[100];
char dil[25];
float val_gps;
float satNumber = 0;

unsigned long previousMillisGPRS = 0;        // will store last time executed 
unsigned long previousMillisGPS = 0;        // will store last time executed

bool setupGPRS() {
  LGPS.powerOn();
  delay(3000);
  int gprs_retry = 0;

  Serial.println("Start");
  
  while(!LGPRS.attachGPRS(APN,LOGIN,PASSWORD)) {
      if(gprs_retry == 120) {
        Serial.println("Connection failed, continue without GPRS..."); // No connection established 
        return false;
      }
      delay(1000);
      gprs_retry += 1;
    }
   Serial.println("Connected to network");
 
  Serial.println("Starting up succeeded");
  Serial.println("Going into Loop");
  Serial.println("\n");
  return true;
}

void loopGPRS() {
   unsigned long currentMillis = millis();

   // Only execute within the specified interval
   if (currentMillis - previousMillisGPS >= INTERVAL_GPS) { 
      previousMillisGPS = currentMillis;
      LTask.remoteCall(createThGPS, NULL);
   }

  if (currentMillis - previousMillisGPRS >= INTERVAL_GPRS) {
    previousMillisGPRS = currentMillis;
    
    Serial.println("--Sending Data--");
    if (client.connect(CLOUD_STORAGE_IP, 80))
    {
      Serial.println("Connected to endpoint, sending GET request...");
      Serial.println("\n");
      
      client.print("GET " + (String)CLOUD_STORAGE_PATH);

      // Make a HTTP request, make query string of all sensor data
      String queryString = generateSensorQueryString();
      
      client.print(queryString);
      
      // Location
      client.print("&x=");
      client.print(lat);
      client.print("&y=");
      client.print(lng);
      
      // GPS
      client.print("&gps=");
      client.print(val_gps);
      
      // Satellites
      client.print("&ns=");
      client.print(satNumber);
      
      client.println(" HTTP/1.1");
      client.print("Host: ");
      client.println(CLOUD_STORAGE_IP);
      client.println("Connection: Close");
      client.println(); 
    } 
  }
}

boolean createThGPS(void* userdata) {
    vm_thread_create(thread_GPS, NULL, 0);
    return true;
}

VMINT32 thread_GPS(VM_THREAD_HANDLE thread_handle, void* user_data) {
    LGPS.getData(&info);
    //Serial.println((char*)info.GPGGA); 
    parseGPGGA((const char*)info.GPGGA);
    return 0;
}



float conv_coords(float in_coords)
{
//Initialize the location.
float f = in_coords;
// Get the first two digits by turning f into an integer, then doing an integer divide by 100;
// firsttowdigits should be 77 at this point.
int firsttwodigits = ((int)f)/100; //This assumes that f < 10000.
//float nexttwodigits = f – (float)(firsttwodigits*100);
float a = f-(float)(firsttwodigits*100);
float theFinalAnswer = (float)(firsttwodigits + a/60.0);
return theFinalAnswer;
}

void parseGPGGA(const char* GPGGAstr)
{
  /* Refer to http://www.gpsinformation.org/dale/nmea.htm#GGA
   * Sample data: $GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47
   * Where:
   *  GGA          Global Positioning System Fix Data
   *  123519       Fix taken at 12:35:19 UTC
   *  4807.038,N   Latitude 48 deg 07.038' N
   *  01131.000,E  Longitude 11 deg 31.000' E
   *  1            Fix quality: 0 = invalid
   *                            1 = GPS fix (SPS)
   *                            2 = DGPS fix
   *                            3 = PPS fix
   *                            4 = Real Time Kinematic
   *                            5 = Float RTK
   *                            6 = estimated (dead reckoning) (2.3 feature)
   *                            7 = Manual input mode
   *                            8 = Simulation mode
   *  08           Number of satellites being tracked
   *  0.9          Horizontal dilution of position
   *  545.4,M      Altitude, Meters, above mean sea level
   *  46.9,M       Height of geoid (mean sea level) above WGS84
   *                   ellipsoid
   *  (empty field) time in seconds since last DGPS update
   *  (empty field) DGPS station ID number
   *  *47          the checksum data, always begins with *
   */
  double latitude;
  double longitude;
  double dilution;
  int tmp, hour, minute, second, num ;
  if(GPGGAstr[0] == '$')
  {
    tmp = getComma(1, GPGGAstr);
    hour     = (GPGGAstr[tmp + 0] - '0') * 10 + (GPGGAstr[tmp + 1] - '0');
    minute   = (GPGGAstr[tmp + 2] - '0') * 10 + (GPGGAstr[tmp + 3] - '0');
    second    = (GPGGAstr[tmp + 4] - '0') * 10 + (GPGGAstr[tmp + 5] - '0');
    
    sprintf(buff, "UTC timer %2d-%2d-%2d", hour, minute, second);

    int currentHour = hour;
    int currentMinute = minute;
    int currentSecond = second;
    tmp = getComma(2, GPGGAstr);
    latitude = getDoubleNumber(&GPGGAstr[tmp]);
    tmp = getComma(4, GPGGAstr);
    longitude = getDoubleNumber(&GPGGAstr[tmp]);
    sprintf(buff, "latitude = %10.10f, longitude = %10.10f", latitude, longitude);

    sprintf(lat, "%10.10f", conv_coords(latitude));
    sprintf(lng, "%10.10f", conv_coords(longitude));
    tmp = getComma(8, GPGGAstr);
    dilution = getDoubleNumber(&GPGGAstr[tmp]);
    val_gps = dilution;
    sprintf(dil, "%.2f", dilution);

    
   
    
    tmp = getComma(7, GPGGAstr);
    num = getIntNumber(&GPGGAstr[tmp]);    
    sprintf(buff, "satellites number = %d", num);
    satNumber = (float) num;
    //Serial.println(satNumber);
    //Serial.println(buff);
  }
  else
  {

  }
}

static unsigned char getComma(unsigned char num,const char *str)
{
  unsigned char i,j = 0;
  int len=strlen(str);
  for(i = 0;i < len;i ++)
  {
     if(str[i] == ',')
      j++;
     if(j == num)
      return i + 1; 
  }
  return 0; 
}

static double getDoubleNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atof(buf);
  return rev; 
}

static double getIntNumber(const char *s)
{
  char buf[10];
  unsigned char i;
  double rev;
  
  i=getComma(1, s);
  i = i - 1;
  strncpy(buf, s, i);
  buf[i] = 0;
  rev=atoi(buf);
  return rev; 
}

