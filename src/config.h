#include <Arduino.h>

#define ADC_BATTERY_PIN 26
#define RELAY_PIN 6
#define ON true
#define OFF false

int idle_interval = 30000;
int warmup_interval = 2*1000*60;
const String SERVER = "test.mosquitto.org";
const String PORT = "1883";

const String TOKEN = "secret";
const String APP_VERSION = "ccfccnadblapdp7slkg0-v1";

const String SENSOR_ID = "00001";
const String DATA = SENSOR_ID + "/data";      //"kp1/" + APP_VERSION + "/dcx/" + TOKEN + "/json";
const String COMMAND = SENSOR_ID + "/command"; //"NULL";// "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/command/zero/status";
const String RESPONSE = SENSOR_ID+"/response";
// const String RESULT = "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/result/zero";

float_t batt_v = 0;
float_t err = 0;
float massConcentrationPm1p0;
float massConcentrationPm2p5;
float massConcentrationPm4p0;
float massConcentrationPm10p0;
float ambientHumidity;
float ambientTemperature;
float vocIndex;
float noxIndex;

uint16_t error_b;
char errorMessage[256];