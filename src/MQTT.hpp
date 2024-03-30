
#ifdef MQTT_TLS 
    // X.509 parsed 
	BearSSL::X509List cert(cacert);
	BearSSL::X509List client_crt(client_cert);
	BearSSL::PrivateKey key(privkey);
	// Secure client connection class
	BearSSL::WiFiClientSecure espClient;
	// Secure client connection class
	PubSubClient mqttClient (espClient);
#else
	WiFiClient espClient;
	PubSubClient mqttClient (espClient);
#endif

//MQTT homeAssitant Discovery
void MqttHomeAssistantDiscovery()
{
	String discoveryTopic;
	String payload;
	String strPayload;

    //Generate de MQTT UNIQUE ID FROM MAC
    for (int i=8; i < macAddr.length(); i++ )
    {
        MQTT_UNIQUE_ID.concat(String(macAddr[i],HEX));
    }

    //Send Home Assistant Discovery
	if (mqttClient.connected())
	{
		Serial.println("SEND HOME ASSISTANT DISCOVERY!!!");
        StaticJsonDocument<600> payload;
        JsonObject device;
        JsonArray identifiers;

		/*Control AC*/

		discoveryTopic = "homeassistant/sensor/" + MQTT_UNIQUE_ID + "/" + MQTT_CLIENT_NAME + "/config";
    
        payload["name"] = MQTT_CLIENT_NAME + "Control";
        payload["uniq_id"] = MQTT_UNIQUE_ID + "_AC";
        payload["stat_t"] = MQTT_STATUS_TOPIC;
        payload["dev_cla"] = "control";
        device = payload.createNestedObject("device");
        device["name"] = MQTT_CLIENT_NAME;
        device["model"] = MQTT_DEVICE_MODEL;
        device["sw_version"] = MQTT_SW_VERSION;
        device["manufacturer"] = MQTT_MANUFACTURED;
        identifiers = device.createNestedArray("identifiers");
        identifiers.add(MQTT_UNIQUE_ID);
        serializeJson(payload, strPayload);
        mqttClient.publish(discoveryTopic.c_str(), strPayload.c_str());
	}
}


// When MQTT received
void OnMqttReceived(char* topic, byte* inFrame, unsigned int length) 
{
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
    
    for (int i = 0; i < length; i++) 
    {
        Serial.print((char)inFrame[i]);
        messageTemp += (char)inFrame[i];
    }
    Serial.println();
  
    if(String(topic) == String("homeassistant/status")) 
    {
        if(messageTemp == "online")
            MqttHomeAssistantDiscovery();
    }
}


//Ini MQTT
void InitMqtt() 
{    
    #ifdef MQTT_TLS
	    //espClient.setTrustAnchors(&cert);
		espClient.setInsecure();
		espClient.setClientRSACert(&client_crt, &key);
		//setClock();
	#endif
	mqttClient.setServer(MQTT_BROKER_ADRESS, MQTT_PORT);
	mqttClient.setCallback(OnMqttReceived);
	mqttClient.setBufferSize(512);
	
}

//Subscribe MQTT Topic for Discovery HomeAssistant
void SuscribeMqtt()
{
	mqttClient.subscribe("homeassistant/status");
	Serial.println ("Topic Subscribed");
}

// connect MQTT
void ConnectMqtt()
{
	while (!mqttClient.connected())
	{
		Serial.print("Starting MQTT connection...");
		if (mqttClient.connect(MQTT_CLIENT_NAME.c_str(), MQTT_USER, MQTT_PASS))
		{
			SuscribeMqtt();
		}
		else
		{
			Serial.print("Failed MQTT connection, rc=");
			Serial.print(mqttClient.state());
			Serial.println(" try again in 5 seconds");

			delay(5000);
		}
	}
}

//HandleMqtt
void HandleMqtt()
{
	if (!mqttClient.connected())
	{
		ConnectMqtt();
	}
	mqttClient.loop();
}


//Publish Mqtt

void PublisMqtt ( String msg )
{
    StaticJsonDocument<200> payload;  
    payload["ac"] = msg;

    String strPayload;
    serializeJson(payload, strPayload);

    if(mqttClient.connected())
    {
        mqttClient.publish(MQTT_STATUS_TOPIC.c_str(), strPayload.c_str()); 
    }
	
}



