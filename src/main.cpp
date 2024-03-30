/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 1.0
 * @date 2024-03-29
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <AsyncMqttClient.h>
#include <WiFiClientSecure.h> 
#include <time.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>

#include "config.h"
#include "WifiUtils.hpp"
#include "MQTT.hpp"


#define IRVCC    5      // Pin connected to the VCC DHT sensor - Pin D1 (GPIO5)

//Global Variables
bool g_InitSystem = true;
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRDaikinESP ac(kIrLed);  // Set the GPIO to be used to sending the message
//IRFujitsuAC ac (kIrLed);
uint32_t delayMS = 1000;


//Setup
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Led Wifi connection
  pinMode(IRVCC, OUTPUT); // Power Supply DHT11 - Pin D1 (GPIO5)

  ConnectWiFi_STA(false); //False with DHCP or True with Static IP
  InitMqtt(); //Ini MQTT

  ac.begin(); //ini Ac
  
}

//Wait to read
void waitRead(){
 //Serial.println("Esperando 1 segundo");
 while ((currentMillis - previousMillis) <= delayMS){
    currentMillis = millis();
  }
  previousMillis = currentMillis;
  currentMillis = millis();
}

//Main Loop

void loop() 
{
  
  HandleMqtt();

  //Send HomeAssistant Discovery
  if(g_InitSystem)
  {
    delay(100);
    g_InitSystem = false;
    Serial.println("INIT SYSTEM...");
    MqttHomeAssistantDiscovery();     // Send Discovery Data
    Serial.println ("Sensor Unique ID: " + MQTT_UNIQUE_ID);
    Serial.println ("Sensor FW Version: " + String(MQTT_SW_VERSION));
  }

  // Enable IR and send Power off AC
  digitalWrite(IRVCC, HIGH);
  ac.off();
  ac.setFan(1);
  ac.setMode(kDaikinCool);
  ac.setTemp(23);

  for (int i = 0; i <= 5; i++){
    ac.send();
    waitRead();
    Serial.println (ac.toString());
  }
  

  //PublisMqtt (char msg);

  //Deep Sleep only 10 seconds connected
  #ifdef ESP_SLEEP 
    if (currentMillis >= intervalSleep){
      Serial.println ("Go to sleep");
      ESP.deepSleep(ESP_SLEEP);
    }
  #endif

}

