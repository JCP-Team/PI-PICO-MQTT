#include <Arduino.h>
#include "gsm_mqtt.h"
#include "config.h"
#include "SCD30.h"
#include "ArduinoJson.h"
#include <Seeed_HM330X.h>


HM330X hm330x;
String senor_json_data(){
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
    batt_v = (analogRead(ADC_BATTERY_PIN) * (3.31/4095))*((560.0+156.0)/560.0);
    err = (1/12) * batt_v + 0.065; //error function, determined.
    batt_v += err;
    doc["battery_voltage"] = batt_v;
    //battery data end
    String data;
    serializeJson(doc,data);
    return data;
}

void sensor_setup(){
    pinMode(ADC_BATTERY_PIN,INPUT);
    analogReadResolution(12);
    Wire.begin();
    scd30.initialize();
    if (hm330x.init()) {Serial.println("HM330X init failed");}
    scd30.setAutoSelfCalibration(1);
}

gsm_mqtt *gsm_module;
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
    gsm_module = new gsm_mqtt(SERVER,PORT,COMMAND,mqtt_callback);
}

unsigned long int timerr = 0;
void loop() {
    gsm_module->gsm_mqtt_loop();
    if(gsm_module->timeout(timerr)){
        timerr = gsm_module->set_time(30000);
        gsm_module->pub(senor_json_data(),DATA);
    }
} 

