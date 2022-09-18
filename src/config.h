#include <Arduino.h>

#define ADC_BATTERY_PIN 26

const String SERVER = "mqtt.cloud.kaaiot.com";
const String PORT = "1883";

const String TOKEN = "secret";
const String APP_VERSION = "ccfccnadblapdp7slkg0-v1";

const String DATA = "kp1/" + APP_VERSION + "/dcx/" + TOKEN + "/json";
const String COMMAND = "NULL";// "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/command/zero/status";
// const String RESULT = "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/result/zero";

float_t batt_v = 0;
float_t err = 0;