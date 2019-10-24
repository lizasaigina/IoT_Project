// This code is for using in Waspmote to  send temperature values to the server in the ThingSpeak cloud


// Put your libraries here (#include ...)
#include <WaspWIFI_PRO.h>
#include <WaspSensorCities_PRO.h>
#include <WaspSensorGas_v30.h>
#include <WaspFrame.h>


// choose socket (SELECT USER'S SOCKET)
///////////////////////////////////////
uint8_t socket = SOCKET0;
///////////////////////////////////////
// WiFi AP settings (CHANGE TO USER'S AP)
///////////////////////////////////////
//char ESSID[] = "iPhone BEST";
//char PASSW[] = "1111111111";
///////////////////////////////////////

bmeCitiesSensor  bme(SOCKET_A);

float temperature;  // Stores the temperature in ºC


char type[] = "http"; // "http" or "https"
char host[] = "api.thingspeak.com";
char port[] = "80";
char link[] = "update?api_key=WD4TB6GJTU357N5W&";

char tempbuffer[100];
///////////////////////////////////////


uint8_t error;
uint8_t status;
unsigned long previous;


void setup()
{
//  USB.println(F("Start program"));
//  USB.println(F("***************************************"));
//  USB.println(F("Once the module is set with one or more"));
//  USB.println(F("AP settings, it attempts to join the AP"));
//  USB.println(F("automatically once it is powered on"));
//  USB.println(F("Refer to example 'WIFI_PRO_01' to configure"));
//  USB.println(F("the WiFi module with proper settings"));
//  USB.println(F("***************************************"));

  USB.ON();
  USB.println(F("Temperature, humidity and pressure sensor example"));


  ///////////////////////////////////////////
  // 1. Turn on the board and the SOCKET
  ///////////////////////////////////////////

  // Switch ON and configure the Gases Board
  Gases.ON();

}



void loop()
{

  char body[150];

  ///////////////////////////////////////////
  // 1. Turn on the sensor
  ///////////////////////////////////////////

  bme.ON();


  ///////////////////////////////////////////
  // 2. Read sensors
  ///////////////////////////////////////////

  temperature = bme.getTemperature();

  strcpy(body, link);
  Utils.float2String (temperature, tempbuffer, 3);

  strcat(body, "field1=");
  strcat(body, tempbuffer); 

  // And print the values via USB
//  USB.println(F("***************************************"));
  USB.print(F("Temperature: "));
  USB.println((temperature));
  

  ///////////////////////////////////////////
  // 3. Turn off the sensor
  ///////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////////////////////////////////
  // get actual time
  previous = millis();

  //////////////////////////////////////////////////
  // 1. Switch ON
  //////////////////////////////////////////////////
  error = WIFI_PRO.ON(socket);

  //////////////////////////////////////////////////
  // 2. Join AP
  //////////////////////////////////////////////////

  // check connectivity
  status =  WIFI_PRO.isConnected();

  // Check if module is connected
  if (status == true)
  {
    USB.print(F("WiFi is connected OK"));
    USB.print(F(" Time(ms): "));
    USB.println(millis() - previous);


    // http request
    error = WIFI_PRO.getURL( type, host, port, body);


    // check response
    if (error == 0)
    {
      USB.println(F("HTTP GET OK"));
//      USB.print(F("HTTP Time from OFF state (ms):"));
//      USB.println(millis() - previous);
//
      USB.print(F("\nServer answer:"));
      USB.println(WIFI_PRO._buffer, WIFI_PRO._length);
    }
    else
    {
      USB.println(F("Error calling 'getURL' function"));
      WIFI_PRO.printErrorCode();
    }
  }
  else
  {
    USB.print(F("WiFi is connected ERROR"));
//    USB.print(F(" Time(ms):"));
//    USB.println(millis() - previous);
  }


  bme.OFF();


  ///////////////////////////////////////////
  // 4. Sleep
  ///////////////////////////////////////////

  // Go to deepsleep
  // After 10 seconds, Waspmote wakes up thanks to the RTC Alarm
//  USB.println(F("Go to deep sleep mode..."));
//  PWR.deepSleep("00:00:00:01", RTC_OFFSET, RTC_ALM1_MODE1, ALL_OFF);
//  USB.println(F("Wake up!!\r\n"));


  //////////////////////////////////////////////////
  // 3. Switch OFF
  //////////////////////////////////////////////////
  WIFI_PRO.OFF(socket);
  USB.println(F("WiFi switched OFF\n\n"));
  delay(100);
}




