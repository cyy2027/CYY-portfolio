# deploy_guide.md（ESP32智能温室大棚‑部署复现指南）
> 文件路径：`personal_project_03_esp32‑smart‑greenhouse/doc/deploy_guide.md`
> ⚠️ 本文档用于**项目复现**，面向拿到源码想要跑通工程的使用者；项目技术设计请看 [`../design.md`](./design.md)

# ESP32智能温室大棚 部署复现指南
## 硬件环境准备
1. ESP32‑DEVKIT V1开发板
2. CH340串口驱动（未安装则需要安装驱动）
3. 外设：DHT温湿度传感器、土壤湿度传感器、光照传感器、继电器执行模块、OLED屏幕
4. USB数据线（支持数据通信，仅供电的数据线无法烧录）

## 一、开发环境搭建
> 本项目基于 Arduino‑IDE 1.8.10 + ESP32支持包 1.0.6；高版本IDE/ESP32包可能存在兼容性问题，建议使用指定版本。

1. 解压 `arduinoide1.8.10.rar`，放置到磁盘根目录，创建桌面快捷方式。
2. 安装ESP32支持包：运行 `32_package_1.0.6.exe`
   - 如果电脑存在其他版本esp32支持包，需要先卸载，仅保留1.0.6版本，否则编译报错。
3. 安装第三方库，将库文件解压至 `文档\Arduino\libraries\`
> ⚠️注意：解压后不要嵌套多层文件夹，例如 `libraries/pubsubclient‑master/pubsubclient‑master` 会报头文件找不到。
需要安装库清单：
- ArduinoJson
- U8g2_Arduino‑master
- pubsubclient‑master（**必须使用项目提供的修改版本，不要升级官方最新版**）
- Crypto
- DHT_sensor_library
- Adafruit_Unified_Sensor

4. 重启Arduino IDE；工具菜单选择开发板：`DOIT ESP32 DEVKIT V1`，选择正确COM串口。

## 二、固件编译烧录
1. 使用Arduino IDE打开项目源码的 `.ino` 主程序文件。
2. 修改配置头文件 `main.h`：
```cpp
// 阿里云设备三要素，从阿里云平台获取
#define PRODUCT_KEY    "你的ProductKey"
#define DEVICE_NAME    "你的DeviceName"
#define DEVICE_SECRET  "你的DeviceSecret"

// WiFi账号密码
const char* WIFI_SSID     = "你的路由器WiFi名称";
const char* WIFI_PASSWORD = "你的WiFi密码";
```
3. 点击上传，编译并烧录固件到ESP32。
4. 打开串口监视器，波特率设置115200；复位开发板，观察日志：
- 打印`MQTT Connected!`代表已经成功连接阿里云服务器。

## 三、阿里云生活物联网平台配置
> 用于实现APP远程查看、下发控制阈值。
1. 登录 [生活物联网平台](https://living.aliyun.com/)，完成账号授权。
2. 新建项目 → 新建产品；芯片模组选择【其他芯片】，数据格式选择`ICA标准数据格式(Alink JSON)`，ID²认证选择**否**。
3. 添加11项自定义属性，**标识符、数据类型必须与固件保持完全一致**：

| 功能名称 | 标识符 | 数据类型 | 读写类型 |
|---|---|---|---|
| 环境温度 | Temp | float | 只读 |
| 环境湿度 | Humi | int32 | 只读 |
| 土壤湿度 | Humi1 | int32 | 只读 |
| 光照度 | Light | float | 只读 |
| 温度阈值 | Set_Temp | int32 | 读写 |
| 湿度阈值 | Set_Humi | int32 | 读写 |
| 光照阈值 | Set_Light | int32 | 读写 |
| 控制模式 | Set_Mode | bool | 读写 |
| 浇水 | Water | bool | 读写 |
| 光照Led | Led | bool | 读写 |
| 风扇 | Fan | bool | 读写 |

4. 人机交互配置：使用云智能APP，编辑设备控制面板，绑定上面全部属性。
5. 设备调试页面，新增测试设备，复制生成的 `ProductKey、DeviceName、DeviceSecret` 填写进固件。
6. 下载配网二维码，使用「云智能」APP扫码添加设备。

## 四、运行校验
1. 设备上电，串口输出WiFi连接成功、MQTT连接成功。
2. 阿里云平台设备状态变为在线。
3. APP可查看传感器实时数据，可以下发阈值、切换手动/自动模式，继电器执行机构同步动作。

## 常见复现问题排查
1. 找不到COM口：CH340驱动未安装，数据线仅支持供电。
2. 编译提示头文件找不到：库解压嵌套多层目录。
3. MQTT连接失败：检查PubSubClient库版本；核对阿里云三要素；WiFi网络可访问外网。
4. 云端收不到数据：云端属性标识符与固件JSON字段不一致。
5. 设备频繁离线：增加WiFi重连逻辑，检查路由器稳定性。

## 📌文档跳转
- [⬅ 返回项目主页](../README.md)
- [查看软硬件详细设计](../design.md)