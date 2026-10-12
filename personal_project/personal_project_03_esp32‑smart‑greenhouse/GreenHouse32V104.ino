/**********************************************************************
//本程序用来通过阿里云平台实现智慧大棚的控制

土壤湿度传感器接开发板，越干燥AD值越大
VCC -->3V3
GND -->GND
AO  -->D34

光照传感器接开发板
GND -->GND
ADD -->不接
SDA -->D21
SCL -->D22
VCC -->3V3

LED灯接开发板，高电平有亮
R   -->D26
GND -->GND

OLED接开发板
GND -->GND
VCC -->3V3
SCL -->D4
SDA -->D13

DHT11接开发板
+   -->3V3
OUT -->D32
-   -->GND

继电器模块接开发板
DC+ -->VIN
DC- -->GND
IN1 -->D27
IN2 -->D2
***********************************************************************/
#include "main.h"

#define  VERSION   "V104"


void connect_wifi()                             //联网
{
  WiFi.mode(WIFI_STA);
  u8g2.setCursor(0, 15);
  u8g2.print("连接路由器....");
  u8g2.sendBuffer();
  WiFi.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);         //用固定的账号密码连接网络
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nWiFi connected to: ");
  Serial.println(WIFI_SSID);
  Serial.print("IP:   ");
  Serial.println(WiFi.localIP());               //得到IP地址

  u8g2.setCursor(0, 32);
  u8g2.print("连接路由器成功");
  u8g2.setCursor(0, 63);
  u8g2.print("IP: " + WiFi.localIP().toString());//显示本机的IP
  u8g2.sendBuffer();
  delay(1000);                                  //延时1S
}

void Led_On(void)                               //灯亮
{
   digitalWrite(Led, HIGH);
   Led_State = true;
   Serial.println("Led on!");
}

void Led_Off(void)                              //灯灭
{
   digitalWrite(Led, LOW);
   Led_State = false;
   Serial.println("Led off!");
}

void Water_On (void)                            //浇水继电器打开
{
  digitalWrite(Water, RELAY_ON);
  Water_State = true;
  Serial.println("Water on!");
}

void Water_Off (void)                           //浇水继电器关闭
{
  digitalWrite(Water, RELAY_OFF);
  Water_State = false;
  Serial.println("Water off!");
}

void Fan_On (void)                              //风扇继电器打开
{
  digitalWrite(Fan, RELAY_ON);
  Fan_State = true;
  Serial.println("Fan on!");
}

void Fan_Off (void)                             //风扇继电器关闭
{
  digitalWrite(Fan, RELAY_OFF);
  Fan_State = false;
  Serial.println("Fan off!");
}

void mqtt_callback(char *topic, byte *payload, unsigned int length)//mqtt回调函数，收到数据后调用此函数处理
{
  Serial.print("Message arrived [");            //打印主题
  Serial.print(topic);
  Serial.print("] ");

  for (int i = 0; i < length; i++){             //打印接收到的数据，JSON数据
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (strstr(topic, ALINK_TOPIC_PROP_SET)){     //如果是属性设置的话，处理
    StaticJsonDocument<500> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    uint8_t num;
    if (doc["params"].containsKey("Set_Temp")){ //如果包含属性，温度阈值
      Set_Pra.Set_Temp = doc["params"]["Set_Temp"];//控制参数
      Pra_Write();                              //保存数据
    }
    if (doc["params"].containsKey("Set_Humi")){ //如果包含属性，土壤湿度阈值
      Set_Pra.Set_Humi = doc["params"]["Set_Humi"];//控制参数
      Pra_Write();                              //保存数据
    }
    if (doc["params"].containsKey("Set_Light")){ //如果包含属性，光照阈值
      Set_Pra.Set_Light = doc["params"]["Set_Light"];//控制参数
      Pra_Write();                              //保存数据
    }
    if (doc["params"].containsKey("Set_Mode")){ //如果包含属性，控制模式
      Set_Pra.Control_Mode = doc["params"]["Set_Mode"];//控制参数
      Pra_Write();                              //保存数据
    }
    if (doc["params"].containsKey("Water")){    //如果包含属性，浇水控制
      num = doc["params"]["Water"];             //控制参数
      if (num == 1){                            //打开
        if ((Water_State == false) && (Set_Pra.Control_Mode == false)){//手动控制
          Water_On();
        }
      }
      else{                                     //关闭
        if ((Water_State == true) && (Set_Pra.Control_Mode == false)){//手动控制
          Water_Off();
        }
      }
    }
    if (doc["params"].containsKey("Led")){      //如果包含属性，光照控制
      num = doc["params"]["Led"];               //控制参数
      if (num == 1){                            //打开
        if ((Led_State == false) && (Set_Pra.Control_Mode == false)){//手动控制
          Led_On();
        }
      }
      else{                                     //关闭
        if ((Led_State == true) && (Set_Pra.Control_Mode == false)){//手动控制
          Led_Off();
        }
      }
    }
    if (doc["params"].containsKey("Fan")){      //如果包含属性，风扇控制
      num = doc["params"]["Fan"];               //控制参数
      if (num == 1){                            //打开
        if ((Fan_State == false) && (Set_Pra.Control_Mode == false)){//手动控制
          Fan_On();
        }
      }
      else{                                     //关闭
        if ((Fan_State == true) && (Set_Pra.Control_Mode == false)){//手动控制
          Fan_Off();
        }
      }
    }
    Control_Deal();                             //控制处理
    Dis_Deal();                                 //显示处理
    mqtt_Send_Data();                           //发送数据
  }
}

void mqtt_version_post()                        //发送版本数据
{
  char param[512];
  char jsonBuf[1024];

  sprintf(param, "{\"id\": 123,\"params\": {\"version\": \"%s\"}}", DEV_VERSION);
  Serial.println(param);
  mqttClient.publish(ALINK_TOPIC_DEV_INFO, param);
}

void mqtt_check_connect()
{
  while (!mqttClient.connected()){              //mqtt未连接上
    Mqtt_State = false;
    while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET)){//连接服务器
      Serial.println("MQTT connect succeed!");

      Mqtt_State = true;                        //已经连接上
      Serial.println("subscribe done");
      mqtt_version_post();                      //发送版本信息
    }
  }
}
 
void mqtt_Send_Data (void)                      //定时上传采集到的数据信息
{
  char param[512];
  char jsonBuf[1024];
  
  sprintf(param, "{\"Temp\":%.1f,\"Humi\":%d,\"Humi1\":%d,\"Light\":%.1f,\"Set_Temp\":%d,\"Set_Humi\":%d,\"Set_Light\":%d,\"Set_Mode\":%d,\"Water\":%d,\"Led\":%d,\"Fan\":%d}", 
    rtemp, rhumi, Humi1_Val, Light_Val, 
    Set_Pra.Set_Temp, Set_Pra.Set_Humi, Set_Pra.Set_Light, Set_Pra.Control_Mode,
    Water_State, Led_State, Fan_State);
  sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
  Serial.println(jsonBuf);
  mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}

void Control_Deal(void)                         //控制处理
{
  if (Set_Pra.Control_Mode == false){           //手动控制的话，退出不处理
    return;
  }

  if (((uint16_t)(rtemp * 10) > (Set_Pra.Set_Temp * 10))){//温度超标，打开风扇
    if (Fan_State == false){                    //继电器在关闭状态的话才打开
      Fan_On();
    }
  }
  else{
    if (Fan_State == true){                     //继电器在打开状态的话才关闭
      Fan_Off();
    }
  }

  if (((uint16_t)Humi1_Val < Set_Pra.Set_Humi)){//湿度超标，浇水
    if (Water_State == false){                  //继电器在关闭状态的话才打开
      Water_On();
    }
  }
  else{
    if (Water_State == true){                   //继电器在打开状态的话才关闭
      Water_Off();
    }
  }
  
  if (((uint16_t)(Light_Val) < Set_Pra.Set_Light)){//光照超标，打开LED
    if (Led_State == false){                    //继电器在关闭状态的话才打开
      Led_On();
    }
  }
  else{
    if (Led_State == true){                     //继电器在打开状态的话才关闭
      Led_Off();
    }
  }
}

/*
湿度：72%    自动/手动模式
温度：32.3℃ 阈值：45
土壤：45%    阈值：60
光照：866lux 阈值：1000
浇水打开/关闭 风扇打开/关闭
*/
void Dis_Deal(void)                             //显示处理
{
  int temp1, humi1;
  temp1 = rtemp * 10; 
  humi1 = rhumi; 

  u8g2.clearBuffer();

  u8g2.setCursor(0, 13);                        //显示当前湿度
  u8g2.print("湿度: ");
  if (humi1 < 10){
    u8g2.print(" "); 
    u8g2.print(humi1); 
  }else
    u8g2.print(humi1); 
  u8g2.print("%");

  u8g2.setCursor(75, 13);
  if (Set_Pra.Control_Mode == true){
    u8g2.print("自动模式"); 
  }
  else{
    u8g2.print("手动模式"); 
  }
  
  u8g2.setCursor(0, 26);                        //显示当前温度
  u8g2.print("温度: ");
  if (temp1 < 100){
    u8g2.print(" "); 
    u8g2.print(temp1 / 10); 
  }else
    u8g2.print(temp1 / 10); 
  u8g2.print(".");
  u8g2.print(temp1 % 10); 
  u8g2.print("℃");

  u8g2.setCursor(75, 26);
  u8g2.print("阈值:");
  u8g2.print(Set_Pra.Set_Temp); 

  u8g2.setCursor(0, 39);                        //显示土壤湿度
  u8g2.print("土壤: ");
  if (Humi1_Val < 10){
    u8g2.print(" "); 
    u8g2.print(Humi1_Val); 
  }else
    u8g2.print(Humi1_Val); 
  u8g2.print("%");
  
  u8g2.setCursor(75, 39);
  u8g2.print("阈值:");
  u8g2.print(Set_Pra.Set_Humi); 
  
  u8g2.setCursor(0, 51);                        //显示光照
  u8g2.print("光照: ");
  u8g2.print((uint16_t)Light_Val); 
  u8g2.print("Lux");

  u8g2.setCursor(75, 51);
  u8g2.print("阈值:");
  u8g2.print(Set_Pra.Set_Light); 
  
  u8g2.setCursor(0, 63);                        //显示浇水和风扇状态
  if (Water_State == true){
    u8g2.print("浇水打开"); 
  }
  else{
    u8g2.print("浇水关闭"); 
  }
  u8g2.setCursor(75, 63);
  if (Fan_State == true){
    u8g2.print("风扇打开"); 
  }
  else{
    u8g2.print("风扇关闭"); 
  }

  u8g2.sendBuffer();
}

float BH1750(void)                              //BH1750设备操作
{
  int i=0;
  float  val=0;
  
  Wire.beginTransmission(BH1750address);        //开始I2C读写操作
  Wire.write(0x10);                             //1lx reolution 120ms//发送命令
  Wire.endTransmission();                       //结束操作
  
  delay(200);                                   //延时等待转换完成
  
  Wire.beginTransmission(BH1750address);
  Wire.requestFrom(BH1750address, 2);
  while(Wire.available()){                      //读取数据
    buff[i] = Wire.read();                      //receive one byte
    i++;
  }
  Wire.endTransmission();                       //如果读到2字节数据的话，转换处理
  if (2 == i){
    val = ((buff[0] << 8) | buff[1]) / 1.2;
  }
  return val;
}

void Pra_Read(void)                             //读取数据
{
  uint16_t i;
  uint8_t *p = (uint8_t*)(&Set_Pra);            //配置结构体指针

  EEPROM.begin(200);                            //申请操作字节数据
  for(i = 0; i < sizeof(Sys_Config); i ++){
    p[i] = EEPROM.read(i);                      //读数据
  }
  if (Set_Pra.store != 0x33AA) {                //没有写入过数据的话，采用默认参数
    Set_Pra.Set_Temp = 35;
    Set_Pra.Set_Humi = 50;
    Set_Pra.Set_Light = 1000;
    Set_Pra.Control_Mode = true;
  }
  
  Serial.printf("Set_Temp: %d\n", Set_Pra.Set_Temp);
  Serial.printf("Set_Humi: %d\n", Set_Pra.Set_Humi);
  Serial.printf("Set_Light: %d\n", Set_Pra.Set_Light);
  Serial.printf("Control_Mode: %d\n", Set_Pra.Control_Mode);
}

void Pra_Write(void)                            //保存数据
{
  uint16_t i;
  uint8_t *p = (uint8_t*)(&Set_Pra);            //配置结构体指针

  Set_Pra.store = 0x33AA;                       //置标志位
  EEPROM.begin(200);                            //申请操作字节数据
  for(i = 0; i < sizeof(Sys_Config); i ++){
     EEPROM.write(i, p[i]);
  }
  EEPROM.commit();                              //保存数据
  
  Serial.printf("Set_Temp: %d\n", Set_Pra.Set_Temp);
  Serial.printf("Set_Humi: %d\n", Set_Pra.Set_Humi);
  Serial.printf("Set_Light: %d\n", Set_Pra.Set_Light);
  Serial.printf("Control_Mode: %d\n", Set_Pra.Control_Mode);
}


void setup() 
{
  Serial.begin(115200);                         //初始化串口
  Serial.println();                             //打印回车换行

  pinMode(Led, OUTPUT);                         //初始化IO
  pinMode(Water, OUTPUT); 
  pinMode(Fan, OUTPUT);
  Led_Off ();                                   //关闭
  Water_Off ();
  Fan_Off ();

  Pra_Read();                                   //读取EEPROM数据
  Wire.begin();                                 //SDA,SCL
  
  u8g2.begin();                                 //U8G2初始化
  u8g2.enableUTF8Print();                       //使能UTF8
  u8g2.clearBuffer();                           //清空显示缓存
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);       //设置字体12*12
  connect_wifi();                               //联网处理

  pinMode(DHTPin, INPUT);                       //DHT11配置
  dht.begin();
  mqttClient.setCallback(mqtt_callback);        //设置回调函数
}

void loop() 
{
  if (!mqttClient.connected()) {                //如果没有连接服务器的话，连接处理
    mqtt_check_connect();
  }
  mqttClient.loop();

  now3 = millis();                              
  if ((now3 - LastTime3 >= 5000) || (LastTime3 == 0)){//定时采集传感器信息
    LastTime3 = now3;

    float rtemp1 = dht.readTemperature();       //读取温湿度
    float rhumi1 = dht.readHumidity(); 
    if (isnan(rtemp1) || isnan(rhumi1)){        //读到的值不对的话不处理
    }
    else{
      rtemp = rtemp1;
      rhumi = (uint8_t)rhumi1; 
      Serial.printf("temp：%.1f, humi：%d\r\n", rtemp, rhumi);
    }

    Humi1_AD = analogRead(HUMI1_PIN);           //AD值，土壤湿度
    Humi1_Val = 100 - (Humi1_AD * 100 / 4096);  //湿度值
    Serial.printf("Humi1_AD: %d, Humi1_Val: %d\n", Humi1_AD, Humi1_Val);

    Light_Val = BH1750();                       //光照度
    Serial.print("Light: ");Serial.println(Light_Val);

    Control_Deal();                             //控制处理
    Dis_Deal();                                 //显示处理
  }

  now1 = millis();
  if ((now1 - LastTime1 >= 30000) || (LastTime1 == 0)){ //定时上传数据
    LastTime1 = now1;
    if (Mqtt_State == true){                    //已经连接上，上传数据
      mqtt_Send_Data();
    }
  }
}
