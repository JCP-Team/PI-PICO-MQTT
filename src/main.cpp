#include <Arduino.h>
#include "gsm_mqtt.h"

#define ADC_PIN 26


gsm_mqtt *gsm_module;
float_t batt_v;
float_t err;

void mqtt_callback(String topic, String message){
    Serial.println(topic);
    Serial.println(message);
}

void setup() {
    Serial.begin(115200);
    while(!Serial);
    pinMode(ADC_PIN,INPUT);
    analogReadResolution(12);
   
     gsm_module = new gsm_mqtt("test.mosquitto.org","commands",mqtt_callback);
}

unsigned long int timerr = 0;
void loop() {
    batt_v = (analogRead(ADC_PIN) * (3.31/4095))*((560.0+156.0)/560.0) ;
    err = (1/12) * batt_v + 0.065; //error function, determined.
    batt_v += err;
    Serial.print("Batt(V)"); Serial.println(batt_v,3);

    gsm_module->gsm_mqtt_loop();
    if(gsm_module->timeout(timerr)){
        timerr = gsm_module->set_time(60000*5);
        gsm_module->pub("Hello world","data");
    }else{
        return;
    }
  
  

  

    delay(1000);
} 

