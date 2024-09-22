#define BLINKER_BLE
#include <Blinker.h>
#include <Wire.h>
#include <Arduino.h>
#include "Adafruit_SGP30.h"
#include "Adafruit_AHTX0.h"

#define SGP_SCL 27
#define SGP_SDA 26
#define LED_BUILTIN 2
#define HEAT_CTRL 13

// 空气传感器结构体
Adafruit_SGP30 sgp;
// 温湿度传感器结构体
Adafruit_AHTX0 aht;

// 变量
float humi_read = 0, temp_read = 0;
float CO2_read = 0, O2_read = 0, TVOC_read = 0;
int counter = 0;
int target_temp = 0;
sensors_event_t humidity, temp;

// APP的控件
BlinkerButton Button1("btn-abc");
BlinkerNumber Humidity("num-humi");
BlinkerNumber Temperature("num-temp");
BlinkerNumber CO2("num-CO2");
BlinkerNumber O2("num-O2");
BlinkerNumber TVOC("num-TVOC");
BlinkerSlider TEMP_SET("sli-temp");  // 创建实例



void heartbeat()  // 心跳组件，每60s触发一次
{
}


void Temp_set_callback(int val)  // 回调函数 参数val是APP的滑动条长度，在20~40之间
{
  target_temp = val;
}

void button1_callback(const String& state) { // APP的按键
  BLINKER_LOG("get button state: ", state);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void dataRead(const String& data) {
  BLINKER_LOG("Blinker readString: ", data);
  counter++;
}



// 初始化函数
void setup() {
  Serial.begin(115200);
  Wire.begin(SGP_SDA, SGP_SCL);
  BLINKER_DEBUG.stream(Serial);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(HEAT_CTRL, OUTPUT);
  digitalWrite(HEAT_CTRL, LOW);

  Serial.println("SGP30 & AHT10 test");

  if (!aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
  }
  Serial.println("AHT10 or AHT20 found");


  if (!sgp.begin()) {
    Serial.println("SGP30 not found :(");
  }

  // CO2传感器
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);



  // Blinker相关的组件
  Blinker.begin();
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);    // 注册按钮
  TEMP_SET.attach(Temp_set_callback);  // 绑定滑动条的回调函数
  Blinker.attachHeartbeat(heartbeat);  // 注册心跳包
}

// AHT10进行一次测量
void AHT_Measure() {
  aht.getEvent(&humidity, &temp);  // populate temp and humidity objects with fresh data
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degrees C");
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println("% rH");
}

// SGP30进行一次测量
void SGP_Measure() {
  if (!sgp.IAQmeasure()) {
    Serial.println("SGP Measurement failed");
    return;
  }
  Serial.print("TVOC ");
  Serial.print(sgp.TVOC);
  Serial.print(" ppb\t");
  Serial.print("eCO2 ");
  Serial.print(sgp.eCO2);
  Serial.println(" ppm");

  if (!sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    return;
  }
  Serial.print("Raw H2 ");
  Serial.print(sgp.rawH2);
  Serial.print(" \t");
  Serial.print("Raw Ethanol ");
  Serial.print(sgp.rawEthanol);
  Serial.println("");

  counter++;
  if (counter == 30) {
    counter = 0;
    uint16_t TVOC_base, eCO2_base;
    if (!sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }
    Serial.print("****Baseline values: eCO2: 0x");
    Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x");
    Serial.println(TVOC_base, HEX);
  }
}

void loop() {

  // 温湿度传感器测量一次
  AHT_Measure();

  // CO2传感器测量一次
  SGP_Measure();

  humi_read = humidity.relative_humidity;
  temp_read = temp.temperature;
  CO2_read = sgp.eCO2;
  TVOC_read = sgp.TVOC;
  if (CO2_read > 300 && CO2_read <= 1000)
    O2_read = 20.9 - CO2_read * 0.0001 - TVOC_read * 0.001;
  else if (CO2_read > 1000 && CO2_read <= 4000)
    O2_read = 20.9 - CO2_read * 0.0005 - TVOC_read * 0.001;
  else if (CO2_read > 4000 && CO2_read <= 10000)
    O2_read = 20.9 - CO2_read * 0.0008 - TVOC_read * 0.001;
  else
    O2_read = 20.9;

  // 上报显示数据
  Humidity.print(humi_read);
  Temperature.print(temp_read);
  CO2.print(CO2_read);
  TVOC.print(TVOC_read);
  O2.print(O2_read);


  // 加热开关控制
  Serial.print(target_temp);
  if (target_temp > temp_read)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(HEAT_CTRL, HIGH);
  } else
  {
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(HEAT_CTRL, LOW);
  }


  Blinker.delay(2000);

  Blinker.run();
}