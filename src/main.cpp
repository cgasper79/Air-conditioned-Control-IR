/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 1.1
 * @date 2024-07-20
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

#include "config.h"
#include "WifiUtils.hpp"
#include "MQTT.hpp"
#include "function.hpp"


#define IRVCC    5      // Pin connected to the VCC DHT sensor - Pin D1 (GPIO5)

//Global Variables
bool g_InitSystem = true;
bool lastControlEnableIR = false;

//Setup
void setup() 
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // Led Wifi connection
  pinMode(IRVCC, OUTPUT); // Power Supply DHT11 - Pin D1 (GPIO5)

  ConnectWiFi_STA(false); //False with DHCP or True with Static IP
  InitMqtt(); //Ini MQTT

  ac.begin(); //ini Ac
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
    PublisMqtt (VERSION,miRSSI,miIP,"Alive");
  }

  //Enable or Disable IR
  if (controlEnableIR){
    // Enable IR
    digitalWrite(IRVCC, HIGH);
    if (controlEnableIR != lastControlEnableIR){
      PublisMqtt (VERSION,miRSSI,miIP,"Control AA Enable");
    }
    lastControlEnableIR = controlEnableIR;
  } else {
    // Disable IR
    digitalWrite(IRVCC, LOW);
    if (controlEnableIR != lastControlEnableIR){
      PublisMqtt (VERSION,miRSSI,miIP,"Control AA Disable");
    }
    lastControlEnableIR = controlEnableIR;
  }
  
  delay(100);
  
  setFunctionAc();

  ReconnectionWifi();
  
  //Deep Sleep only 10 seconds connected
  #ifdef ESP_SLEEP 
    if (currentMillis >= intervalSleep){
      Serial.println ("Go to sleep");
      ESP.deepSleep(ESP_SLEEP);
    }
  #endif
  delay(200);
}


