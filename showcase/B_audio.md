# B · 音频作品集

> 这一包回答一个问题：**能不能把 I2S 数据流、音频算法与产品交互整条链路打通。**

## 覆盖岗位方向

音频算法（反馈抑制 / 移频）｜ 音视频终端 ｜ 安防 IPC·对讲

## 证据清单

| 证据 | 位置 | 证明了什么 | 状态 |
|---|---|---|---|
| 数字调音台-反馈抑制器（实习·阶段交付） | [internship_project_02](../internship/GAOXINXING/internship_project_02-digital-audio-mixer) | BP2668Ax DSP + AT32 主控、FBX 自适应陷波 + 移频防啸叫、AK4619 Codec、I2S 音频流、COBS+CRC32 串口协议 | ✅ 脱敏文档 |
| 桌面 USB 声卡（实习·方案设计） | [internship_project_03](../internship/GAOXINXING/internship_project_03-usb-audio-interface) | UAC2.0、Monitor 零延迟硬件监听、Loopback 内录、48V 幻像电源、SPI 彩屏交互 | ✅ 脱敏文档 |
| 离线语音蓝牙音箱 | [offline_voice_speaker](../personal_project/09_iot‑basic‑projects/offline_voice_speaker) | 杰理音频 SoC、A2DP、离线 KWS 语音唤醒、三路输入事件统一处理 | ✅ 文档 |
| ESP32 智能终端三形态（门铃/触控语音/视觉语音） | [08_esp32‑smart‑terminals](../personal_project/08_esp32‑smart‑terminals) | 目录结构已建 | ⏳ 待补（文件未上传） |
| 《音频爆音定位》专题 | [docs](../docs/audio-pop-noise-debug.md) | 爆音问题的定位方法与排查路径 | ✅ 文档 |
| 移频前后录音样本 | `assets/audio/`（规划） | 啸叫抑制的真实效果 | ⏳ 待补 |
| 频谱对比 / 爆音波形 | `assets/wave/`（规划） | 算法作用前后的客观差异 | ⏳ 待补 |

## 关键数据

FBX 自适应陷波 + 移频双算法 ｜ 4 路麦克风输入独立算法处理 ｜ UAC2.0 ｜ 48V 幻像电源双通道独立继电器

## 局限与未验证

- 实习音频项目按保密要求仅公开**脱敏文档**，无源码、无音频样本；「啸叫抑制有效」目前只有文字描述，**没有录音 / 频谱证据支撑**。
- 08 三个 ESP32 终端子项目**目录为空**，文件尚未上传。
- 离线语音音箱为课程级原型，未做识别率统计。

## 30 秒阅读路径

`实习② README` → `实习③ README` → `09/offline_voice_speaker` → [《音频爆音定位》](../docs/audio-pop-noise-debug.md)

---

## 📌 返回导航

[⬅ 作品集首页](../README.md) ｜ [⬅ 五套作品集导航](../README.md#-五套作品集按岗位方向选一个看) ｜ [⬅ 60+ 岗位方向索引](../docs/career-path-index.md)

> 本页为**策展页**：只做筛选、链接与证据标注，项目内容一律在各目录原文，不在此复制。
