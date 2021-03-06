# GroveDriverPack

## 概要

GroveモジュールをGrove対応ボードですばやく利用するためのライブラリです。  
Groveモジュールのクラスと、Grove対応ボードのクラスが用意されており、この2つのクラスを組み合わせるだけでGroveモジュールから値の取得、設定ができます。  

## 対応製品

### Grove対応ボード

* [Wio LTE Cat.1 JP Version](https://seeedjp.github.io/Wiki/Wio_LTE_for_Arduino/Home-ja.html)  
  * Arduino - SeeedJP STM32 Boards by Seeed K.K. + Wio LTE for Arduino by Seeed K.K.
* [Wio 3G SORACOM Edition](https://soracom.jp/products/module/wio_3g_soracom_edition/)
  * Arduino - SeeedJP STM32 Boards by Seeed K.K. + Wio cell lib for Arduino by Seeed K.K.
* [Wio LTE M1/NB1(BG96)](https://soracom.jp/products/module/wio_lte_m1_nb1/)
  * Arduino - SeeedJP STM32 Boards by Seeed K.K. + Wio cell lib for Arduino by Seeed K.K.
  * (Mbed - [Seeed Wio LTE M1/NB1(BG96)](https://os.mbed.com/platforms/Seeed-Wio-BG96/)) ← 開発予定
* [ReButton](https://aka.ms/rebutton)
  * Arduino - SeeedJP ReButton by Seeed K.K.
* [Wio Terminal](https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/)
  * Arduino - Seeed SAMD Boards by Seeed Studio

### Groveモジュール

|Pic|Class|SKU|Name|Note|
|:--|:--|:--|:--|:--|
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar884360_3.jpg" width="100">|GroveGasO2|101020002|[Grove - Oxygen Sensor(ME2-O2-Ф20)](http://wiki.seeedstudio.com/Grove-Gas_Sensor-O2/)|作成中|
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar878321_4.jpg" width="100">|GroveButton|101020003|[Grove - Button](http://wiki.seeedstudio.com/Grove-Button/)||
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar881115_4.jpg" width="100">|GroveUltrasonicRanger|101020010|[Grove - Ultrasonic Ranger](http://wiki.seeedstudio.com/Grove-Ultrasonic_Ranger/)|Mbed未対応|
|<img src="https://statics3.seeedstudio.com/seeed/img/2017-02/0Es2ATc90JsEW4xNSNyBpVak.jpg" width="100">|GroveTempHumiDHT11|101020011|[Grove - Temperature & Humidity Sensor (DHT11)](http://wiki.seeedstudio.com/Grove-TemperatureAndHumidity_Sensor/)|Mbed未対応|
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar881159_2.jpg" width="100">|GroveRotaryAngle|101020017|[Grove - Rotary Angle Sensor](http://wiki.seeedstudio.com/Grove-Rotary_Angle_Sensor/)||
|<img src="https://files.seeedstudio.com/wiki/Grove-Temperature_and_Humidity_Sensor_Pro/img/main.jpg" width="100">|GroveTempHumiDHT22|101020019|[Grove - Temperature & Humidity Sensor Pro (AM2302/DHT22)](http://wiki.seeedstudio.com/Grove-Temperature_and_Humidity_Sensor_Pro/)|Mbed未対応|
|<img src="https://raw.githubusercontent.com/SeeedDocument/Grove-Tilt_Switch/master/img/Tilt1.jpg" width="100">|GroveTiltSwitch|101020025|[Grove - Tilt Switch](http://wiki.seeedstudio.com/Grove-Tilt_Switch/)||
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar881092_3.jpg" width="100">|GroveDigitalLight|101020030|[Grove - Digital Light Sensor](http://wiki.seeedstudio.com/Grove-Digital_Light_Sensor/)||
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar881144_3.jpg" width="100">|GroveTouch|101020037|[Grove - Touch Sensor](http://wiki.seeedstudio.com/Grove-Touch_Sensor/)||
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-01/bazaar676592_img_5740aa.jpg" width="100">|GroveMagneticSwitch|101020038|[Grove - Magnetic Switch](http://wiki.seeedstudio.com/Grove-Magnetic_Switch/)||
|<img src="https://statics3.seeedstudio.com/seeed/img/2016-11/GsPzr5xrvqzue5sH4ZomS1DE.jpg" width="100">|GroveAccelerometer16G|101020054|[Grove - 3-Axis Digital Accelerometer(±16g)](http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer-16g/)||
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-08/bazaar887099_4.jpg" width="100">|GroveEMGDetector|101020058|[Grove - EMG Detector](http://wiki.seeedstudio.com/Grove-EMG_Detector/)||
|<img src="https://statics3.seeedstudio.com/product/101020078%201_02.jpg" width="100">|~~GroveAirQuality~~|101020078|[Grove - Air quality sensor v1.3](http://wiki.seeedstudio.com/Grove-Air_Quality_Sensor_v1.3/)|必要電流が多すぎてWio LTE/3Gでは使えない|
|<img src="https://statics3.seeedstudio.com/images/product/101020091%203.jpg" width="100">|GroveMiniTrackBall|101020091|[Grove - Mini Track Ball](http://wiki.seeedstudio.com/Grove-Mini_Track_Ball/)|作成中|
|<img src="https://raw.githubusercontent.com/SeeedDocument/Grove_Light_Sensor/master/images/cover.jpg" width="100">|GroveLight|101020132|[Grove - Light Sensor v1.2](http://wiki.seeedstudio.com/Grove-Light_Sensor/)||
|<img src="https://statics3.seeedstudio.com/seeed/img/2016-06/R425Ianm2Rr93qmJQFUEwGx7.jpg" width="100">|GroveTempHumiBaroBME280|101020193|[Grove - Temp&Humi&Barometer Sensor (BME280)](http://wiki.seeedstudio.com/Grove-Barometer_Sensor-BME280/)||
|<img src="https://statics3.seeedstudio.com/seeed/img/2016-06/cmJT8HtXruW9KsyjBBeqDNT9.jpg" width="100">|GroveTempHumiSHT31|101020212|[Grove - Temperature&Humidity Sensor (SHT31)](http://wiki.seeedstudio.com/Grove-TempAndHumi_Sensor-SHT31/)||
|<img src="https://statics3.seeedstudio.com/seeed/img/2016-09/mgPnvDQ0Qn1UbtrHuivjpyrb.jpg" width="100">|GroveSpeechRecognizer|101020232|[Grove - Speech Recognizer](http://wiki.seeedstudio.com/Grove-Speech_Recognizer/)||
|<img src="https://statics3.seeedstudio.com/seeed/file/2017-06/bazaar482888_1.jpg" width="100">|GroveI2CColorSensor2|101020341|[Grove - I2C Color Sensor V2](https://www.seeedstudio.com/Grove-I2C-Color-Sensor-V2-p-2890.html)|作成中|
|<img src="https://files.seeedstudio.com/wiki/Grove-I2C_High_Accuracy_Temp-Humi_Sensor-SHT35/img/main.jpg" width="100">|GroveTempHumiSHT35|101020592|[Grove - I2C High Accuracy Temp&Humi Sensor (SHT35)](http://wiki.seeedstudio.com/Grove-I2C_High_Accuracy_Temp%26Humi_Sensor-SHT35/)||
|<img src="https://statics3.seeedstudio.com/images/product/onewiretempsensor.jpg" width="100">|GroveTempOneWire|101990019|One Wire Temperature Sensor||
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar881127_3.jpg" width="100">|GroveRelay|103020005|[Grove - Relay](http://wiki.seeedstudio.com/Grove-Relay/)||
|<img src="https://statics3.seeedstudio.com/images/product/104020006%201.jpg" width="100">|GroveLEDBar2|104020006|[Grove LED Bar v2.0](http://wiki.seeedstudio.com/Grove-LED_Bar/)||
|<img src="https://files.seeedstudio.com/wiki/Grove-16x2_LCD_Series/img/main.jpg" width="100">|GroveLCD1602|104020111,104020112,104020113|[Grove - 16x2 LCD](http://wiki.seeedstudio.com/Grove-16x2_LCD_Series/)|Mbed未対応|
|<img src="https://statics3.seeedstudio.com/seeed/file/2018-07/bazaar881151_3.jpg" width="100">|Grove4DigitDisplay|104030003|[Grove - 4-Digit Display](http://wiki.seeedstudio.com/Grove-4-Digit_Display/)|Wio Terminal未対応|
|<img src="https://statics3.seeedstudio.com/seeed/img/2016-09/6hLLXlBnfODAcWlp2wlaep3j.jpg" width="100">|GroveOLEDDisplay096|104030008|[Grove - OLED Display 0.96"](http://wiki.seeedstudio.com/Grove-OLED_Display_0.96inch/)|Mbed未対応|
|<img src="https://statics3.seeedstudio.com/images/product/Grove%20Circular%20LED.jpg" width="100">|GroveCircularLED|104030013|[Grove - Circular LED](http://wiki.seeedstudio.com/Grove-Circular_LED/)||
|<img src="https://raw.githubusercontent.com/SeeedDocument/Grove-6-Axis_AccelerometerAndGyroscope/master/img/Grove-6-Axis_AccelerometerAndGyroscope_product_view_1200_s.jpg" width="100">|Grove6AxisAccelGyroLSM6DS3|105020012|[Grove - 6-Axis Accelerometer&Gyroscope](http://wiki.seeedstudio.com/Grove-6-Axis_AccelerometerAndGyroscope/)||
|<img src="https://statics3.seeedstudio.com/seeed/img/2017-02/lJ51OXkwCFgmT6iAjE030rSd.jpg" width="100">|GroveBuzzer|107020000|[Grove - Buzzer](http://wiki.seeedstudio.com/Grove-Buzzer/)||
|<img src="https://statics3.seeedstudio.com/images/product/High%20Temperature%20Sensor.jpg" width="100">|GroveHighTemp|111020002|[Grove - High Temperature Sensor](http://wiki.seeedstudio.com/Grove-High_Temperature_Sensor/)||
|<img src="https://statics3.seeedstudio.com/seeed/img/2016-11/txvkIZ9IGno10T8UYi8KTvPS.jpg" width="100">|GroveGPS|113020003|[Grove - GPS](http://wiki.seeedstudio.com/Grove-GPS/)||
|<img src="https://files.seeedstudio.com/wiki/Grove-MP3-V3/img/Grove-MP3-V3-preview.jpg" width="100">|GroveMP3V3|107020069|[Grove - MP3 V3 -Music Player](http://wiki.seeedstudio.com/Grove-MP3-v3/)||
|<img src="https://github.com/SeeedDocument/Grove-Laser_PM2.5_Sensor-HM3301/raw/master/img/main.jpg" width="100">|GrovePM25HM3301|101020613|[Grove - Laser PM2.5 Air Quality Sensor for Arduino - HM3301](http://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/)||
|<img src="https://files.seeedstudio.com/wiki/Grove-CO2-Temperature-Humidity-Sensor-SCD30/img/main.jpg" width="100">|GroveSCD30|101020634|[Grove - CO2 & Temperature & Humidity Sensor for Arduino (SCD30) - 3-in-1](https://wiki.seeedstudio.com/Grove-CO2_Temperature_Humidity_Sensor-SCD30/)||
|<img src="https://files.seeedstudio.com/wiki/Grove-Adjustable_PIR_Motion_Sensor/img/main.jpg" width="100">|GrovePIR|101020617|[Grove - Adjustable PIR Motion Sensor](https://wiki.seeedstudio.com/Grove-Adjustable_PIR_Motion_Sensor/)||

### 拡張基板

|Pic|Class|SKU|Name|Note|
|:--|:--|:--|:--|:--|
|<img src="https://files.seeedstudio.com/wiki/Wio_Extension-RTC/img/pinout.jpg" width="100">|WioExtRTC|103100082|[Wio Extension - RTC](http://wiki.seeedstudio.com/Wio-Extension-RTC/)||

### Groveコネクタ付きモジュール

|Pic|Class|Name|Note|
|:--|:--|:--|:--|
|![1](img/1.png)|OmronBaro2SMPB02E|[絶対圧センサ評価モジュール（Groveコネクタ付）](https://www.omron.co.jp/ecb/product-detail?partId=45066)||

## サンプルコード

各Groveモジュールごとに、examples配下にあります。  
Arduino IDEを使っている場合は、ファイル > スケッチ例 > GroveDriverPack で開くことができます。

## コントリビューター

下記の方々に貢献いただきました。どうもありがとうございます。

* [uko](https://github.com/ukkz)さん
* [n0bisuke](https://github.com/n0bisuke)さん
