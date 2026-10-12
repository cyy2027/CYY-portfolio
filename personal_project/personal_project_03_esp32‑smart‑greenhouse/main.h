/**********************************************************************
//需要自行修改的几个参数：引号不要去掉，是修改下面正文部分，不是修改这里的注释部分
1，认证三元组，这两个参数通过阿里云平台添加设备得到，具体看说明文件的阿里云网页操作部分
#define PRODUCT_KEY      "" //--------------------替换自己的PRODUCT_KEY--------------------
#define DEVICE_NAME      "" //--------------------替换自己的DEVICE_NAME--------------------
#define DEVICE_SECRET    "" //--------------------替换自己的DEVICE_SECRET--------------------

2，家里无线路由器的账号和密码
const char* WIFI_SSID     = "";   //无线路由器的账号，------要替换自己的----------
const char* WIFI_PASSWORD = "";   //无线路由器的密码，------要替换自己的----------
***********************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <string.h>
#include "aliyun_mqtt.h"
#include "DHT.h"
#include <U8g2lib.h>


#define RELAY_ON    HIGH            //继电器高电平打开
#define RELAY_OFF   LOW             //继电器低电平关闭

#define HUMI1_PIN   34              //土壤湿度传感器AD
int DHTPin = 32;                    //DHT11
int Led = 25;                       //LED输出，高电平亮
int Water = 27;                     //继电器输出，1有效，浇水控制
int Fan = 2;                        //继电器输出，1有效，风扇控制

#define SCL  4                      //OLED屏时钟
#define SDA  13                     //OLED屏数据
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SCL, SDA, U8X8_PIN_NONE);//scl,sda,rst

#define DHTTYPE DHT11               //DHT 11
float rtemp;                        //dht11变量
uint8_t rhumi;
DHT dht(DHTPin, DHTTYPE);           //DHT定义

int BH1750address = 0x23;           //BH1750 I2C地址，模块ADDR下拉。如果接高电平的话地址为5C
byte buff[2];
float Light_Val = 0;                //采集到的光照值

//---------------------Time 相关参数---------------------
unsigned long now0;                 //定时
unsigned long LastTime0 = 0;
unsigned long now1;                 //定时定时上传数据到云平台
unsigned long LastTime1 = 0;
unsigned long now2;                 //定时
unsigned long LastTime2 = 0;
unsigned long now3;                 //定时采集传感器
unsigned long LastTime3 = 0;

bool Mqtt_State = false;            //连接状态
bool Led_State = false;             //Led状态，true为打开,false为关闭
bool Water_State = false;           //继电器状态，true为打开,false为关闭
bool Fan_State = false;             //继电器状态，true为打开,false为关闭
uint16_t Humi1_AD = 0;              //土壤湿度
uint8_t Humi1_Val = 0;              //土壤湿度百分比

struct Sys_Config                   //设置参数信息
{
  uint16_t store;                   //数据有效标志位，0x55AA说明写入数据了
  uint16_t Set_Temp;                //设置的温度阈值
  uint16_t Set_Humi;                //设置的湿度阈值
  uint16_t Set_Light;               //设置的光照阈值
  bool Control_Mode;                //控制模式，true为自动,false为手动
};
Sys_Config Set_Pra;

//-------------------------MQTT相关参数---------------------------------
WiFiClient   espClient;
PubSubClient mqttClient(espClient);

#define PRODUCT_KEY      ""                      //--------------------替换自己的PRODUCT_KEY--------------------认证三元组
#define DEVICE_NAME      ""             //--------------------替换自己的DEVICE_NAME--------------------
#define DEVICE_SECRET    "" //--------------------替换自己的DEVICE_SECRET--------------------
 
#define DEV_VERSION       "ALI-V1.01"                       //固件版本信息，自定义，可以用于后续的OTA升级程序

#define ALINK_METHOD_PROP_POST    "thing.event.property.post"                                             //发送接收信息的method方式
#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"  //发送接收信息的JSON结构，method方式固定，params包含实际的读写数据
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"        //属性上报TOPIC
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"  //云端响应属性上报TOPIC
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"       //属性设置TOPIC
#define ALINK_TOPIC_DEV_INFO      "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""                    //OTA版本信息上报TOPIC
#define ALINK_VERSION_FROMA      "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"                        //版本信息的JSON结构

const char* WIFI_SSID     = "cyy2027";   //家里无线路由器的账号和密码，------要替换自己的----------，引号不要去掉
const char* WIFI_PASSWORD = "cyy2027";
//---------------------------------------------------------------------


#endif
