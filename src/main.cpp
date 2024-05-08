/**
 * @file main.cpp
 * @author cgasper79
 * @brief 
 * @version 1.0
 * @date 2024-04-29
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


#define IRVCC    5      // Pin connected to the VCC DHT sensor - Pin D1 (GPIO5)

//Global Variables
bool g_InitSystem = true;
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRDaikinESP ac(kIrLed);  // Set the GPIO to be used to sending the message
uint32_t delayMS = 1000;
bool lastControlEnableIR = false;

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
  
  // Timer Power Off AC 
  if(controlTimer && controlEnableIR) {
    Serial.println ("Desconexión AA por tiempo");
    for (int i = 0; i <= 2; i++){
      ac.off();
      ac.send();
      waitRead();
    }
    PublisMqtt (VERSION,miRSSI,miIP,"Desconectado");
    controlTimer = false;
  } 

  // Adjust Temperature AC
  if (controlTimer && adjustTemp){
    Serial.println ("Ajustando temperatura");
    ac.setFan(1);
    ac.setMode(kDaikinCool);
    ac.setTemp(22);
    ac.send();
    waitRead();
    PublisMqtt (VERSION,miRSSI,miIP,"Ajustando Temp");
    adjustTemp = false;
  }

  // Alive Control Ac
  if (aliveControl){
    Serial.println ("Alive Control");
    waitRead();
    PublisMqtt (VERSION,miRSSI,miIP,"Alive");
    aliveControl = false;
  }

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

