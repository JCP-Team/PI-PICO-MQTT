#include <Arduino.h>

#define ADC_BATTERY_PIN 26
#define RELAY_PIN 6
#define ON true
#define OFF false

int reading_interval = 30000;
const warmup_interval = 2*1000*60;
const String SERVER = "mqtt.cloud.kaaiot.com";
const String PORT = "1883";

const String TOKEN = "secret";
const String APP_VERSION = "ccfccnadblapdp7slkg0-v1";

const String SENSOR_ID = "00001"
const String DATA = SENSOR_ID + "/data";      //"kp1/" + APP_VERSION + "/dcx/" + TOKEN + "/json";
const String COMMAND = SENSOR_ID + "/command"; //"NULL";// "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/command/zero/status";
// const String RESULT = "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/result/zero";

float_t batt_v = 0;
float_t err = 0;