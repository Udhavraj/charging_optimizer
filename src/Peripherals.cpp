#include <Arduino.h>
#include <DHT.h>
#include "State.h"
#include "Peripherals.h"
#include "config.h"


DHT   dht(DHT_PIN , DHT_TYPE);

float mapFloat(long x, long inMin, long inMax, float outMin, float outMax) {
  return (x - inMin) * (outMax - outMin) / (float)(inMax - inMin) + outMin;
}






void sample_sensor(void)
{
    int raw_current = analogRead(CURRENT_PIN); // 0 to 4095
    int raw_voltage = analogRead(VOLTAGE_PIN); // 0 to 4095

    // map voltage 0 to 250v
   voltage = mapFloat(raw_voltage, 0, 4095, 0, 250);

   if(bayStatus == "CHARGING")
   {

    // map current 0 to 32A
   current = mapFloat(raw_current, 0, 4095, 0, 32);

   }
   else{
    current = 0;

   }
   
    // Read Current and 5 array values





    // Calculate  power
    power = voltage * current;
    // Calculate accumulated energgy in Wh
    energyWh += power * (5.0 / 3600.0);


    //to read temperature 
   float t = dht.readTemperature(DHT_PIN );
   if(!(isnan(t))) temperature = t;
   
    
    


}

float recentAvgCurrent(void) 
{
   float sum =0;
   for(int i=0; i<5; i++)
   {
    sum = sum + current;
    return sum/5;
   }   






 }  
   




bool plugin_flag_once = 1;
bool plugout_flag1 = 1;
   
void plug_status(void)
{
   bool pluginRead = digitalRead(BTN_PLUGIN );
   // detect the sw is pressed
      if(pluginRead == LOW);
      {
        // session start time in ms
         sessionStartMs = millis(); 

   // plug in switch is pressed
      plugin_flag_once = 0;
   // change bay_status FREE to charging
   if(bayStatus == "FREE")
   {
      bayStatus = "CHARGING";
      Serial.println("Bay3 plugin detected, Bay is Charging");
      digitalWrite(RELAY_PIN , HIGH); // turn on relay to start charging
   }
   //update leds
      }
    if (pluginRead == HIGH)
    {
       plugin_flag_once = 1;
    }

   // plug out switch is pressed
    bool plugoutRead = digitalRead(BTN_PLUGOUT );
   // detect the sw is pressed
      if(plugoutRead == LOW && plugout_flag1)
      {
   // plug out switch is pressed
      plugout_flag1 = 0;
   // change bay_status  charging to FREE
   if(bayStatus == "CHARGING")
   {
      bayStatus = "FREE";
      Serial.println("Bay1 plugout detected, Bay is FREE");
   }
   //update leds
      }
    if (plugoutRead == HIGH)
    {
       plugout_flag1 = 1;
    }



   

}

void update_leds_status(void)
{
 // if bay_status is charging, turn on green led 
 if(bayStatus == "FREE")
 {
    digitalWrite(LED_GREEN,HIGH);
    digitalWrite(LED_YELLOW,LOW);
 }
 else
 {
// if bay_status is free, turn on yellow led
      digitalWrite(LED_GREEN,LOW);
      digitalWrite(LED_YELLOW,HIGH);

 }
 
}