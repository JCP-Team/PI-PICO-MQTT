#include <Arduino.h>
#include "gsm_mqtt.h"
#include "SCD30.h"
#include "ArduinoJson.h"
#include <Seeed_HM330X.h>


HM330X hm330x;

StaticJsonDocument<150> hm330x_json_data(){
    StaticJsonDocument<150> doc;
    uint8_t buf[30];
    if (hm330x.read_sensor_value(buf, 29)) {
        Serial.println("HM330X read result failed");
        return doc;
    }
    if (NULL == buf) {
        return doc;
    }
    doc["PM1.0"] = (uint16_t) buf[2 * 2] << 8 | buf[2 * 2 + 1];
    doc["PM2.5"] = (uint16_t) buf[3 * 2] << 8 | buf[3 * 2 + 1];
    doc["PM10"] = (uint16_t) buf[4 * 2] << 8 | buf[4 * 2 + 1];
    return doc;
}

StaticJsonDocument<150> scd30_json_data(){
    float result[3] = {0};
    StaticJsonDocument<150> doc;
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);   
        doc["Carbon Dioxide"] = result[0];
        doc["Temperature"] = result[1];
        doc["Humidity"] = result[2];   
    }
    return doc;
}

String senor_json_data(){
    StaticJsonDocument<150> scd30 = scd30_json_data();
    StaticJsonDocument<150> hm330x = hm330x_json_data();
    StaticJsonDocument<300> doc;
    doc["scd30"] = scd30;
    doc["hm330x"] = hm330x;
    String data;
    serializeJson(doc,data);
    return data;
}

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
    Wire.begin();
    scd30.initialize();
    if (hm330x.init()) {Serial.println("HM330X init failed");}

    gsm_module = new gsm_mqtt("test.mosquitto.org","00001/commands",mqtt_callback);
    scd30.setAutoSelfCalibration(1);

}

unsigned long int timerr = 0;
void loop() {
    batt_v = (analogRead(ADC_PIN) * (3.31/4095))*((560.0+156.0)/560.0) ;
    err = (1/12) * batt_v + 0.065; //error function, determined.
    batt_v += err;
    Serial.print("Batt(V)"); Serial.println(batt_v,3);

    gsm_module->gsm_mqtt_loop();
    if(gsm_module->timeout(timerr)){
        timerr = gsm_module->set_time(20000);
        gsm_module->pub(senor_json_data(),"00001/data");
    }else{
        return;
    }
  
  

  

    delay(1000);
} 

