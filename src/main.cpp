#include <Arduino.h>
#include "gsm_mqtt.h"
#include "config.h"
//sensors
#include "SCD30.h"
#include "ArduinoJson.h"
#include <Seeed_HM330X.h>
#include <Multichannel_Gas_GMXXX.h>
gsm_mqtt *gsm_module;
GAS_GMXXX<TwoWire> gas;void external_state(bool in){ //switches relay ON/OFF
    if(in) digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN,LOW);
}

HM330X hm330x;
String senor_json_data(){
    scd30.initialize();
    if (hm330x.init()) {Serial.println("HM330X init failed");}
    scd30.setAutoSelfCalibration(1);

    StaticJsonDocument<308> doc;
    //scd30 data
    float result[3] = {0};
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);   
        doc["carbon_dioxide"] = result[0];
        doc["Temperature"] = result[1];
        doc["Humidity"] = result[2];   
    }
    //scd30 data end
    //hm330x data begin
    uint8_t buf[30];
    if (hm330x.read_sensor_value(buf, 29)) {
        Serial.println("HM330X read result failed");
        return "";
    }
    if (NULL == buf) {
        return "";
    }
    doc["PM1_0"] = (uint16_t) buf[2 * 2] << 8 | buf[2 * 2 + 1];
    doc["PM2_5"] = (uint16_t) buf[3 * 2] << 8 | buf[3 * 2 + 1];
    doc["PM10"] = (uint16_t) buf[4 * 2] << 8 | buf[4 * 2 + 1];
    //hm330x data end

    //battery data begin
    doc["battery_voltage"] = gsm_module->batt_voltage;
    //battery data end
    String data;
    serializeJson(doc,data);

    return data;
}

void sensor_setup(){
    pinMode(ADC_BATTERY_PIN,INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    analogReadResolution(12);
    Wire.begin();
    scd30.initialize();
    if (hm330x.init()) {Serial.println("HM330X init failed");}
    scd30.setAutoSelfCalibration(1);
    gas.begin(Wire, 0x08);
}


void mqtt_callback(String topic, String message){
    // Serial.println(topic);
    // Serial.println(message);
    // StaticJsonDocument<150> doc;
    // deserializeJson(doc,message);
    // StaticJsonDocument<150> response;
    // response["id"] = doc[0]["id"];
    // response["statusCode"] = 200;
    // response["reasonPhase"] = "OK";
    // response["payload"] = "Success";
    // String data;
    // serializeJson(response,data);
    // data = "["+data+"]";
    // gsm_module->pub(data,RESULT);
}

void setup() {
    Serial.begin(115200);
    sensor_setup();
    gsm_module = new gsm_mqtt("test.mosquitto.org",PORT,COMMAND,mqtt_callback);
}

enum MAINSTATE{WARMUP,SEND};
MAINSTATE main_state = MAINSTATE::WARMUP;
unsigned long int timerr = 0;
void loop() {
    // switch(main_state){
    //     case MAINSTATE::SEND:{
    //         if(gsm_module->timeout(timerr)){
    //             //send data
    //             //turn off relay
    //             timerr = gsm_module->set_time(SEND_DELAY);
    //             main_state = MAINSTATE::WARMPUP;
    //         }
    //         break;
    //     }
    //     case MAINSTATE::WARMPUP:{
    //         if(gsm_module->timeout(timerr)){
    //             timerr = gsm_module->set_time(WARMUP_TIME);
    //             //turn relay on
    //             main_state = MAINSTATE::SEND
    //         }
    //     }
    // }
    gsm_module->gsm_mqtt_loop();
    switch (main_state)
    {
    case MAINSTATE::WARMUP:
         if(gsm_module->timeout(timerr)){
         main_state = MAINSTATE::SEND;
         external_state(ON);
         timerr = gsm_module->set_time(warmup_interval);
         }
        break;
    case MAINSTATE::SEND:
         if(gsm_module->timeout(timerr)){
             gsm_module->pub(senor_json_data(),DATA);
              main_state= MAINSTATE::WARMUP;
              external_state(OFF);
              timerr = gsm_module->set_time(reading_interval);
         }
        break;
    }
} 

