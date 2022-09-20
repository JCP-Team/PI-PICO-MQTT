#include <Arduino.h>

#define ADC_BATTERY_PIN 26
#define RELAY_PIN 6
#define ON true
#define OFF false

int reading_interval = 30000;
int warmup_interval = 2*1000*60;
const String SERVER = "test.mosquitto.org";
const String PORT = "1883";

const String TOKEN = "secret";
const String APP_VERSION = "ccfccnadblapdp7slkg0-v1";

const String DATA = "00001/data";//"kp1/" + APP_VERSION + "/dcx/" + TOKEN + "/json";
const String COMMAND = "00001/command";//"NULL";// "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/command/zero/status";
// const String RESULT = "kp1/" + APP_VERSION + "/cex/" + TOKEN + "/result/zero";
