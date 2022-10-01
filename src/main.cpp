#include <Arduino.h>
#include "gsm_mqtt.h"
#include "config.h"
#include "SCD30.h"
#include "ArduinoJson.h"
#include <SensirionI2CSen5x.h>

SensirionI2CSen5x sen55;
void external_state(bool in){ //switches relay ON/OFF
    if(in) digitalWrite(RELAY_PIN, HIGH);
    else digitalWrite(RELAY_PIN,LOW);
}


String senor_json_data(){
    scd30.initialize();
    scd30.setAutoSelfCalibration(1);

    StaticJsonDocument<436> doc; //300+8+128
    //scd30 data
    float result[3] = {0};
    if (scd30.isAvailable()) {
        scd30.getCarbonDioxideConcentration(result);   
        doc["carbon_dioxide"] = result[0];
        doc["Temperature"] = result[1];
        doc["Humidity"] = result[2];   
    }
    //scd30 data end
    //SEN55 data start
    error_b = sen55.readMeasuredValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
        noxIndex);

    if (error_b) {
        Serial.print("error_b trying to execute readMeasuredValues(): ");
        errorToString(error_b, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
        doc["MassConcentrationPm1p0"]=massConcentrationPm1p0;
        doc["MassConcentrationPm2p5"]=massConcentrationPm2p5;
        doc["MassConcentrationPm4p0"]=massConcentrationPm4p0;
        doc["MassConcentrationPm10p0"]=massConcentrationPm10p0;
        if (isnan(ambientHumidity)) {
            doc["AmbientHumidity"]="n/a";
        } else {
            doc["AmbientHumidity"]=(ambientHumidity);
        }
        if (isnan(ambientTemperature)) {
            doc["AmbientTemperature"]="n/a";
        } else {
            doc["AmbientTemperature"]=ambientTemperature;
        }
        if (isnan(vocIndex)) {
            doc["VocIndex"]="n/a";
        } else {
            doc["VocIndex"]=vocIndex;
        }
        if (isnan(noxIndex)) {
            doc["NoxIndex"]="n/a";
        } else {
            doc["NoxIndex"]=noxIndex;
        }
    }
    
    //SEN55 data end
    
    String data;
    serializeJson(doc,data);

    return data;
}

void sensor_setup(){
    pinMode(RELAY_PIN, OUTPUT);
    Wire.begin();
    sen55.begin(Wire);
    error_b = sen55.deviceReset();
    if (error_b) {
        Serial.print("Error trying to execute deviceReset(): ");
        errorToString(error_b, errorMessage, 256);
        Serial.println(errorMessage);
    }
    error_b = sen55.startMeasurement();
    if (error_b) {
        Serial.print("Error executing startMeasurement(): ");
        errorToString(error_b, errorMessage, 256);
        Serial.println(errorMessage);
    }
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

enum MAINSTATE{WARMUP,SEND};
MAINSTATE main_state = MAINSTATE::WARMUP;
unsigned long int timerr = 0;
void loop() {
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

