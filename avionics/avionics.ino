#include <Adafruit_GPS.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "IntersemaBaro.h"


#define _DRA  8

int acc[3] = {A1,A2,A3};

// Altitude differential array
int alt_diff[_DRA] = {0,0,0,0,0,0,0,0};
// Differential index
int di;
// what's the name of the hardware serial port?
#define GPSSerial Serial

// Connect to the GPS on the hardware port
Adafruit_GPS GPS(&GPSSerial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO  true

 Intersema::BaroPressure_MS5607B baro(true);
Adafruit_BNO055 bno = Adafruit_BNO055(55);
int initial_altitude = 0;
int old_alt = 0;


int rec_pin = 1;
void setup()  
{

  
  Serial.begin(115200);
  
  
  /* 
   *
   *  INITIALIZE SENSORS
   *  
   */
  
  // IMU
  if(!bno.begin())
  {
    Serial.print("No IMU detected");
    while(1);
  }

  // GPS
  GPS.begin(9600);
  // You can adjust which sentences to have the module emit, below
  // Default is RMC + GGA
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // Default is 1 Hz update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  if (!GPS.LOCUS_StartLogger())
  {
    Serial.print("No GPS detected or logging failure");
    while(1);
  }
  
  // ALTIMETER
 

  // Get initial altitude reading
  // 100 samples over 1s
  initial_altitude = avgAlt(100);
  old_alt = initial_altitude;
  int di = 0;


  // Intialize pins
  pinMode(rec_pin, OUTPUT);
}

void loop()                     // run over and over again
{
  if (GPSSerial.available()) {
    char c = GPSSerial.read();
    Serial.write(c);
  }

   /* Get a new sensor event */
  sensors_event_t event;
  bno.getEvent(&event);

  
  Serial.print(F("Orientation: "));
  Serial.print((float)event.orientation.x);
  Serial.print(F(" "));
  Serial.print((float)event.orientation.y);
  Serial.print(F(" "));
  Serial.print((float)event.orientation.z);
  Serial.println(F(""));

  
  long new_alt = baro.getHeightCentiMeters();   // <<<<<< CHANGED
  // Cast to int to save space 
  int f_alt = (int)((float)(new_alt) / 30.48);

  // Calculate height differential (velocity)
  // Positive if going up, negative if going down

  alt_diff[di] = f_alt - old_alt;
  
  if(check_alt_diff()){
    digitalWrite(rec_pin,HIGH);
  }
}

// Make sure that velocity has been negative
bool check_alt_diff()
{
  for(int i =0; i < _DRA; i++){
    if(alt_diff[di] >= 0) return false;
  }
  return true;
}

long avgAlt(int samples)
{
  long sampleSum = 0;
  for(int i=0; i < samples; i++)
  {
    sampleSum += (long)((float) baro.getHeightCentiMeters() / 30.48);
    delay(10);
  }
  return sampleSum/samples;
}
  


