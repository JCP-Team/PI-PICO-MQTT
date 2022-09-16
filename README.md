# PI-PICO-MQTT
GDRT related prototyping for Pi Pico based MQTT end point.



---
## Sensors:

- [x] PM2.5
 * SN-GCJA5L
   * [Datasheet](https://b2b-api.panasonic.eu/file_stream/pids/fileversion/244939)
   * [Usage](https://b2b-api.panasonic.eu/file_stream/pids/fileversion/8768)
 * [HM3301](https://www.seeedstudio.com/Grove-Laser-PM2-5-Sensor-HM3301.html)
   * [Datasheet](https://files.seeedstudio.com/wiki/Grove-Laser_PM2.5_Sensor-HM3301/res/HM-3300%263600_V2.1.pdf)
   * [Usage](https://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/)
 * [HPMA115s0](https://sps.honeywell.com/us/en/products/advanced-sensing-technologies/healthcare-sensing/particulate-matter-sensors/hpm-series)
   * [Datasheet](https://prod-edam.honeywell.com/content/dam/honeywell-edam/sps/siot/en-us/products/sensors/particulate-matter-sensors-hpm-series/documents/sps-siot-particulate-hpm-series-datasheet-32322550-ciid-165855.pdf?download=false)
   *[Usage](https://forum.digikey.com/t/hpm-series-pm2-5-particle-sensor/858/4)

- [x] Humidity & Temperature
  * DHT20v2
      * [Datasheet](https://files.seeedstudio.com/products/101020932/DHT20-Product-Manual.pdf)
      * [Datasheet 2] (https://cdn.sparkfun.com/assets/8/a/1/5/0/DHT20.pdf)
      * [Usage](https://wiki.seeedstudio.com/Grove-Temperature-Humidity-Sensor-DH20/)
  * [BME680](https://wiki.seeedstudio.com/Grove-Temperature_Humidity_Pressure_Gas_Sensor_BME680/)
      * [Datasheet]( https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001.pdf)
      *  [Datasheet 2: PCB](https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME680-DS001.pdf)
- [x] CO2 & OTher
*SGP30
      *  [Datasheet](https://files.seeedstudio.com/wiki/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/res/Sensirion_Gas_Sensors_SGP30_Datasheet_EN.pdf)
      * [Usage] (https://wiki.seeedstudio.com/Grove-VOC_and_eCO2_Gas_Sensor-SGP30/)
 * [SCD30](https://media.digikey.com/pdf/Data%20Sheets/Sensirion%20PDFs/CD_DS_SCD30_Datasheet_D1.pdf)
      * [Datasheet](https://media.digikey.com/pdf/Data%20Sheets/Sensirion%20PDFs/CD_DS_SCD30_Datasheet_D1.pdf)
      * [Calibration Notes](https://sensirion.com/products/catalog/SCD30/)
      * [Arduino](https://wiki.seeedstudio.com/Grove-CO2_Temperature_Humidity_Sensor-SCD30/)
      * [github](https://github.com/Seeed-Studio/Seeed_SCD30)
  * [Grove Multichanel Gas sensor v2]( https://www.seeedstudio.com/Grove-Multichannel-Gas-Sensor-v2-p-4569.html)
      *  [Usage](https://wiki.seeedstudio.com/Grove-Multichannel-Gas-Sensor-V2/)
      *  [GIthub Library](https://github.com/ostaquet/Arduino-Grove-MultichannelGasSensor-driver)
  * [SN0219 Infrared](https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/SEN0219_Web.pdf)
      * [Tutorial](https://wiki.dfrobot.com/Gravity__Infrared_CO2_Sensor_For_Arduino_SKU__SEN0219)
