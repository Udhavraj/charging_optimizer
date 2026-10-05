#include <Arduino.h>
#include <WiFi.h>
#include "State.h"
#include "config.h"
#include "Peripherals.h"
#include "Network.h"
#include "Telementry.h"
#include "model.h"
#include "edge_ai.h"
#include "optimization.h"
#include "attributes.h"
#include "rpc.h"


void setup()
{
    mqtt.loop();

    Serial.begin(115200);
    dht.begin();  // initialise sesnor

    // Congiure  esp32 with real time
    configTime(0,0,"pool.ntp.org","time.nist.gov");

    /// congigure pheripals pins
    pinMode(BTN_PLUGIN , INPUT_PULLUP);
    pinMode(BTN_PLUGOUT , INPUT_PULLUP);
    pinMode(RELAY_PIN , OUTPUT);
    pinMode(LED_GREEN , OUTPUT);
    pinMode(LED_YELLOW , OUTPUT);
    pinMode(LED_RED , OUTPUT);


    // connect a board to wifi
    connectWiFi();



    // configuer the MQTT Server
      mqtt.setServer(MQTT_SERVER, MQTT_PORT); // MQTT SERVER ADRESS AND PORT NUMBER

      // set the callback function upon receiving the data from the cloud
    mqtt.setCallback(mqttCallback);
    mqtt.setBufferSize(512); // set the buffer size to 512 bytes for the mqtt client

    
    // Connecting board to cloud
    connectMQTT(); //TOKEN ,DEVICE ID
   
}

unsigned long now;
unsigned long last_print;

void loop()
{
    //push data every 5 sec
    now = millis();
    if((now - last_print) > 5000)
    {
        last_print = now;
         // read data from sensors // Read Voltage,current, temperature, power, bay status
         sample_sensor();

         // run ai to get the predction
         runEdgeAIInference();

         // decide load based on the predictions
          runOptimization();



         // publish the data
         publishTelemetry ();
        
         


    }
    plug_status();
    updateLeds();
    
}

