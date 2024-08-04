//Version
String VERSION = "v1.1";

//define user, if you don't use comment please
#define MQTT_TLS  // to use MQTT TLS 
//#define ESP_SLEEP 3600000000 // Time in uS to sleep 

//WIFI config
const char* ssid     = "Your_SSID_WIFI";
const char* password = "Your_PASSWORD";
const char* hostname = "Your_Hostname";

//Static IP config
IPAddress ip(192, 168, 1, 5);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(1, 1, 1, 1); 
IPAddress secondaryDNS(1, 0, 0, 1);

// info 
IPAddress 	miIP;
IPAddress 	mascaraRed;
IPAddress 	puertaEnlace;
IPAddress 	miDNS;
String 		miRSSI;
String 		macAddr;

// Variables to reconnect wifi (verify each 10 seconds) 
unsigned long previousWifiMillis = 0;
unsigned long currentMillis = 0;
unsigned long previousPushMillis = 0;
unsigned long intervalWifi = 10000;

// Variables to time Read Sensor
unsigned long previousMillis = 0;
unsigned long intervalRead = 1000;
unsigned long intervalSleep = 10000;

//Variables MQTT config
const char* 		MQTT_BROKER_ADRESS = "your.host.mqtt";

//Variables globales
bool controlEnableIR = false;
bool timerOffEnable = false;
bool powerOffAc = false;
bool adjustTemp = false;
bool adjustModeDry = false;
bool aliveControl = false;


#ifdef MQTT_TLS
	const uint16_t MQTT_PORT = 8883;
#else
	const uint16_t MQTT_PORT = 1883;
#endif

const char* 		MQTT_USER = "guest";
const char* 		MQTT_PASS = "password";

// Variable used for MQTT Discovery
const char*         MQTT_DEVICE_MODEL = "WemosDevice";                          // Hardware Model
const char*         MQTT_SW_VERSION = VERSION.c_str();                          // Firmware Version
const char*         MQTT_MANUFACTURED = "cgasper79";                            // Manufacturer Name
String		 		MQTT_CLIENT_NAME = "BlanesAAControl"; 							// Device Name                           
String              MQTT_STATUS_TOPIC = "dispositivos/" + MQTT_CLIENT_NAME + "/status"; 	// MQTT Topic
String              MQTT_SET_TOPIC = "dispositivos/" + MQTT_CLIENT_NAME + "/set"; 	// MQTT Topic
String				MQTT_UNIQUE_ID;   
String				MQTT_DISCOVERY_TOPIC = "homeassistant/status";  			// MQTT Topic Hass Discovery


// Certificates

#ifdef MQTT_TLS

static const char cacert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

static const char client_cert[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

static const char privkey[] PROGMEM = R"KEY(
-----BEGIN PRIVATE KEY-----

-----END PRIVATE KEY-----
)KEY";

#endif






