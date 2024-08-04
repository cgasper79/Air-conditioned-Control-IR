//Global Variables
const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRDaikinESP ac(kIrLed);  // Set the GPIO to be used to sending the message
uint32_t delayMS = 500;

//Wait to read
void waitRead(){
    //Serial.println("Esperando 1 segundo");
    while ((currentMillis - previousMillis) <= delayMS){
        currentMillis = millis();
    }
    previousMillis = currentMillis;
    currentMillis = millis();
}



void setFunctionAc() 
{

    if (controlEnableIR)
    {
        //Power Off AC 
        if(powerOffAc) {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println ("Desconexión AA por tiempo");
            ac.off();
            ac.send(3);
            waitRead();
            PublisMqtt (VERSION,miRSSI,miIP,"Desconectado");
            powerOffAc = false;
            Serial.println(ac.toString());
            ac.on();
            digitalWrite(LED_BUILTIN, LOW);
        } 

        // Adjust Temperature AC
        if (adjustTemp){
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println ("Ajustando temperatura");
            ac.setFan(kDaikinFanAuto);
            ac.setMode(kDaikinAuto);
            ac.setTemp(22);
            ac.send(2);
            waitRead();
            PublisMqtt (VERSION,miRSSI,miIP,"Ajustando Temperatura");
            adjustTemp = false;
            Serial.println(ac.toString());
            digitalWrite(LED_BUILTIN, LOW);
        }

        // Mode Dry 
        if (adjustModeDry){
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println ("Mode Dry");
            ac.setFan(kDaikinFanAuto);
            ac.setMode(kDaikinDry);
            ac.send(2);
            waitRead();
            PublisMqtt (VERSION,miRSSI,miIP,"Mode Dry");
            adjustModeDry = false;
            Serial.println(ac.toString());
            digitalWrite(LED_BUILTIN, LOW);
        }

        // Timer Off On 
        if (timerOffEnable){
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println ("TimerOff On");
            ac.enableOffTimer(60);
            ac.send(2);
            waitRead();
            PublisMqtt (VERSION,miRSSI,miIP,"TimerOff On");
            timerOffEnable = false;
            Serial.println(ac.toString());
            digitalWrite(LED_BUILTIN, LOW);
        } 
    } 

    // Alive Control Ac
    if (aliveControl){
        Serial.println ("Alive Control");
        waitRead();
        PublisMqtt (VERSION,miRSSI,miIP,"Alive");
        aliveControl = false;
    }

}