# CHEN Yiyuan (陈艺苑) · Embedded Software Engineer Portfolio

**Class of 2027 ｜ Seeking: Embedded Software / Firmware Development (MCU Firmware · RTOS · Embedded Linux)**

> 🌐 [中文版（首页）](./README.md)

> 2 internships (incl. 1 project shipped to mass production) · 9 complete hands-on systems · 1 pre-research direction · 26 in-depth technical articles
> Domains: low-level firmware & BSP drivers · industrial control & edge computing · robotics / flight control · audio-video terminals · medical instruments

[Online portfolio (Feishu, nicer layout)](https://my.feishu.cn/docx/D5kzdRqFVoco87x0ESDcsfz8njd?from=from_copylink)

> 💡 **How to read**: every project follows the same structure — `README.md` (overview & highlights), `design.md` (detailed HW/SW design), `docs/` (deep dives / debug logs / retrospectives). Cloning locally and reading with VS Code + Markdown preview is recommended.

---

## 🎯 Five Portfolios (Pick One by Target Role)

> Short on time? Pick the portfolio that matches the target role. Each is a single **curation page** (Chinese): it filters, links, and annotates evidence — the content itself stays in each project directory.

| Portfolio | Target roles | Key evidence | Entry |
|---|---|---|---|
| **A · Motion Control** | flight control, robotics, AGV, motion control, FOC | 1 kHz CAN joint scheduling, 4-stage cascaded PID, SVPWM (source in repo) | [→ A](./showcase/A_motion-control.md) |
| **B · Audio** | audio algorithms (feedback suppression), AV terminals, security IPC | FBX notch + frequency shifting, I2S pipeline, UAC2.0 sound card | [→ B](./showcase/B_audio.md) |
| **C · Mechanical** | structural design | Self-designed thermal chamber (3D design + print verification), robot structure optimization, ANOTECH internship | [→ C](./showcase/C_mechanical.md) |
| **D · Hardware** | motor drivers / power, PCB layout, consumer electronics, AE/FAE | TEC board schematic + PCB source files, mass-production relay board | [→ D](./showcase/D_hardware.md) |
| **E · Data & Reliability** | BMS, chargers, metering, reliability | ±0.25 ℃ measured curves, OCV + temp-compensated SOC, 8-case bug review | [→ E](./showcase/E_data-report.md) |

---

## 🧭 Find by Target Role (Full Index, 60+ roles)

> **[Career-path index — full version (Chinese)](./docs/career-path-index.md)** — a mainline-system index organized by **60+ target roles**:
> embedded firmware (general / Linux / IoT / motor control / industrial / medical / embodied AI / home appliances / energy storage / automotive…),
> hardware R&D, test & reliability, and structural / documentation / FAE roles, plus selections for **English-resume** applications to foreign companies.
> Each role lists the recommended systems to present — use it to guide interviewers when submitting applications.

---

## 🧰 Capability Matrix (Skill → Verifiable Evidence)

| Capability | Supporting projects & articles |
|---|---|
| **New-platform bring-up / domestic-chip migration** | Internship ① domestic Cortex-M0 migration (2 weeks to production) · Internship ② BP2668Ax RISC-V audio DSP bring-up · [OTA for Mass-Production Firmware & Domestic MCU Migration](./docs/firmware‑mass‑produce.md) |
| **Production-grade reliability design** | 06 self-developed IAP + CRC verification + power-loss rollback · Internship ① IWDG watchdog & mains-voltage safety logic · 01 tiered fault safety · [Cortex-M HardFault Localization](./docs/cortex-m-hardfault-debug.md) |
| **Bare-metal / RTOS firmware architecture** | Non-blocking FSM + FreeRTOS task partitioning in 01 / 03 / 04 / 07 · [Non-Blocking FSM Design](./docs/realtime‑fsm.md) · [FreeRTOS Pitfall Notes](./docs/freertos-trap-notes.md) |
| **Control-algorithm implementation** | 02 multi-stage cascaded PID · 03 SVPWM voltage-mode FOC · 01 trot gait & quintic trajectories · 07 ±0.25 ℃ steady-state temperature control · [PID Engineering & Tuning](./docs/pid-engineering-tuning.md) |
| **Buses & protocol stacks** | CAN (01/04/06) · Modbus-RTU (04/05) · COBS+CRC custom framing (Internship ②) · LoRa / MQTT (05) · [CAN Bus in Practice](./docs/can-bus-practice.md) |
| **Embedded Linux / edge computing** | 05 RK3568 running MQTT broker, TCP Socket cross-SoC communication, SQLite local caching · [Edge Gateway: Socket & Local Cache](./docs/linux-edge-socket-cache.md) |
| **Audio-video & GUI** | Internships ②③ I2S audio pipelines, mixing/routing, pop-noise elimination · 08 LVGL + OV2640 + WebSocket · [Audio Pop-Noise Localization](./docs/audio-pop-noise-debug.md) |
| **Host tools & debug toolchain** | 02 Qt5 desktop/Android ground station · 09 Python waveform tooling · J-Link / logic analyzer / UART debugging |

---

## 🏢 Internship Projects (Sanitized — No Confidential Source Code)

**GAOXINXING Technology Group ｜ Audio Division**　*Assistant Embedded Software Engineer*

1. 🔴 **[Power Sequencer — Domestic Chip Migration](./internship/GAOXINXING/internship_project_01-power-sequencer)** — Migration to a domestic Cortex-M0 MCU. Non-blocking FSM drives 8-channel power sequencing; ADC matrix keypad, HLW8032 energy metering, IWDG watchdog. **Firmware adaptation completed independently in 2 weeks and shipped to mass production**; localized and closed 8 HW/SW-coupled bugs.
2. 🔴 **[Digital Mixer — Feedback Suppressor](./internship/GAOXINXING/internship_project_02-digital-audio-mixer)** — RISC-V audio DSP bring-up on the BP2668Ax evaluation board: I2S audio pipelines, a COBS+CRC custom UART protocol, and pre-research on frequency-shifting feedback suppression; resolved pop noise, noise floor, and IO pin-conflict issues coupled across hardware and software.
3. 🟡 **[Desktop USB Sound Card](./internship/GAOXINXING/internship_project_03-usb-audio-interface)** — Joined at the solution-design stage; built the upper-layer application framework before hardware arrived: audio mixing/routing, pop-noise elimination, global state management, and layered software architecture design.

> Each directory contains the project plan, flowcharts, and a development summary; [internship certificates & department overview](./internship/GAOXINXING/README.md).
> **[ANOTECH internship](./internship/ANOTECH)** is an early **product structural design** internship (documentation only, no firmware) — it built 3D spatial intuition, manufacturing awareness, and cross-team collaboration experience.
> ⚠️ NDAs are strictly honored: all internship code and materials are sanitized and contain no confidential business source code or internal BOM parameters.

---

## Personal Hands-On Systems (9)

### 01 · [Quadruped Dual Platform: MCU Monolithic + ROS 2 Layered](./personal_project/01_quadruped_robotics)　👑 Flagship

One theme, two implementations — to answer **where the real-time boundary should actually sit**.

**[HyperDog · Monolithic MCU Master Controller](./personal_project/01_quadruped_robotics/hyperdog_rtos_control)**　`Cortex-M` `FreeRTOS` `IMU (ICM20602)` `CAN` `Trot gait` `Quintic polynomial`
- Diagonal trot gait scheduling + quintic-polynomial foot trajectories with zero velocity/acceleration at endpoints, minimizing landing impacts.
- One-master-multi-slave CAN: the master dispatches 12-DoF joint targets at **1 kHz** with a 200 Hz gait main loop; each joint node runs its own FOC loop.
- Adaptive complementary filtering (impact-triggered dynamic weighting) + four-tier fault protection with graceful leg retraction; architecture / protocol / fusion / gait / safety / calibration are covered in the six-part series in this repo's docs.

**[Nybble · ROS 2 + micro-ROS Layered](./personal_project/01_quadruped_robotics/nybble_micro_ros)**　`ROS 2 Foxy` `XRCE-DDS` `UART+DMA` `FreeRTOS`　🚧 In research
- Planning layer moved to Linux (IK / gait / attitude compensation); the MCU only executes in real time; 12 joint angles dispatched periodically as topics.
- Three-stage jitter absorption: DMA + ring buffer + queue; native TIM1/TIM8 PWM replaces the PCA9685 expander board.
- Currently **architecture design only**, no hardware or code — a deliberate contrast to the monolithic approach above.

### 02 · [BeiDou / GNSS Positioning Flight Controller](./personal_project/02_gnss-flight-controller)　🔴
`STM32H723/F405` `FreeRTOS` `IMU/GNSS fusion` `Cascaded PID` `DShot-600` `ELRS (CRSF)` `MSP OSD` `ESP8285` `Qt5`
- ICM-42688-P + barometer + M10 BeiDou/GNSS multi-source fusion; multi-stage cascaded PID for attitude stabilization, altitude hold, and position hold.
- DShot-600 high-speed digital ESC protocol; ELRS CRSF long-range RC link & telemetry; ported IRC Tramp VTX control and MSP OSD overlay.
- Dual-chip architecture: ESP8285 handles Wi-Fi passthrough for tuning; a Qt5 ground station (desktop/Android) visualizes sensors and pushes parameters in one click.

### 03 · [STM32 BLDC FOC Motor Control System](./personal_project/03_foc‑bldc‑motor)　🔴
`FreeRTOS` `SVPWM` `Voltage-mode FOC` `AS5600 magnetic encoder` `CAN`
- Voltage-mode FOC vector control implemented **without phase-current sensing**, with SVPWM modulation.
- AS5600 magnetic encoder for position; CAN interface to a host PC for real-time motor performance tuning.

### 04 · [STM32 Dual-Axis Motion Control Platform (DC + Stepper)](./personal_project/04_dual‑axis‑motion‑control)　🟡
`STM32F103` `IR2104S H-bridge` `Hall encoder` `Incremental PID` `S-curve accel/decel` `CAN` `Modbus-RTU`
- H-bridge drives the DC motor; pulse-direction mode drives the stepper; hardware over-current protection and dead-time control.
- Incremental PID closed-loop speed control on the DC axis; trapezoidal/S-curve profiles suppress missed steps at start/stop on the stepper axis.
- Commands over CAN and Modbus-RTU; independent per-axis speed control, direction reversal, and multi-axis coordination.

### 05 · [Industrial IoT Edge Gateway System (MCU + Linux)](./personal_project/05_industrial‑edge‑gateway)　🔴
`FreeRTOS` `Modbus-RTU` `CAN` `LLCC68-LoRa` `W5500` `MQTT` `cJSON` `RK3568 (Linux)` `Socket` `SQLite`
- MCU side: RS485 / LoRa / CAN multi-protocol acquisition, protocol conversion, and MQTT uplink.
- Linux side: RK3568 runs the MQTT broker and SQLite local cache.
- TCP Socket bridges MCU↔Linux with reconnect + local caching, forming a complete device-edge-cloud pipeline.

### 06 · [Battery Management & Safe Charging System (BMS + Smart Charger)](./personal_project/06_bms‑and‑charger)　🔴
`BQ76920` `I2C (CRC-8)` `CAN` `IAP Bootloader` `RC522 RFID` `ECB02 BLE` `OLED`
- BMS: BQ76920 monitors 5S lithium cell voltages/current/NTC temperatures; over-voltage, under-voltage, and over-current protection; OCV lookup + temperature compensation for SOC estimation.
- Charger: self-developed IAP firmware upgrade (CRC verification + power-loss rollback, brick-proof); RFID card billing; safety interlock logic for mains-voltage relays.

### 07 · [STM32 Dual-Loop (Heat/Cool) High-Precision Temperature Control](./personal_project/07_stm32‑tec‑temperature‑ctrl)　🔴
`STM32L031` `TEC (Peltier)` `NTC` `Composite digital filtering` `BLE` `OLED` `Rotary encoder`
- Bidirectional TEC driving for both heating and cooling; multi-channel NTC sensing.
- Composite software filtering + hardware LDO/π-filter front end achieves **±0.25 ℃** steady-state accuracy.
- OLED + encoder local UI, BLE Android app, and VOFA+ host-side visualization; non-blocking FSM manages cooling/heating/hold operating modes.

### 08 · [ESP32-S3 Smart Vision & Touch Terminals (Multi-Form-Factor)](./personal_project/08_esp32‑smart‑terminals)　🟡
`ESP-IDF` `FreeRTOS` `LVGL` `FT6336G` `ILI9341V` `I2S` `ES8311` `OV2640/OV3660` `SDIO` `WebSocket` `MQTT` `BLE` `NVS` `OTA`
- Three form-factor sub-projects: video doorbell, touch+voice terminal, vision+voice terminal.
- FT6336G capacitive touch + ILI9341V SPI display with LVGL UI; I2S + ES8311 audio capture/playback.
- Ring buffers isolate audio/video streams from network stalls; SDIO TF-card storage, BLE provisioning, NVS parameter persistence, OTA upgrades.

### 09 · [IoT & Consumer Electronics Practice Collection](./personal_project/09_iot‑basic‑projects)　🟢
`JieLi Byle-U8 SDK` `A2DP` `Offline KWS` `ADC keypad` `IR` `WS2812` `Alibaba Cloud MQTT` `AD8232` `ADC/DMA` `EEPROM`
- Offline-voice Bluetooth speaker: A2DP playback + offline keyword spotting; voice/keypad/IR inputs unified through one event-dispatch layer to control the RGB ring.
- ESP32 smart greenhouse: multi-sensor acquisition, MQTT to cloud, app remote control, reconnect handling, EEPROM parameter persistence.
- STM32 ECG monitor: ADC-DMA sampling of AD8232 signals; three-layer architecture (driver / business / debug) with UART waveform streaming.

---

## 🚧 Work in Progress (Pre-Research · No Deliverables Yet)

> These are technical routes **fully scoped out but not yet started**. They are listed because the problems themselves have engineering value — not to pad the list.
> All metrics are targets; anything unverified is explicitly marked as such.

### 00 · [Embodied-AI Edge Computing Platform](./personal_project/00_embodied_ai_platform)　🚧 In research
`RK3588 (Linux + 6 TOPS NPU)` `Bipedal / quadruped platform` `Brain–cerebellum heterogeneous architecture` `On-device RL inference` `Sim-to-Real`

- **The question to answer**: the Linux side runs policy inference at 50 Hz while the MCU side runs a 20 kHz current loop — three orders of magnitude apart. How do you build a latency-bounded path so upper-layer scheduling jitter never reaches the joints?
- **Three hard constraints**: the safety boundary must close inside the real-time domain (if Linux crashes, the robot must still retract its legs) · an interpolation-and-clamping layer must sit between decision and current loop · every latency segment must be individually instrumented and measurable.
- **Done**: layered architecture, four core risks decomposed, four-stage acceptance criteria, and selection trade-offs with rejection rationale ([proposal document](./personal_project/00_embodied_ai_platform)).
- **Not started**: hardware procurement, BSP porting, and any line of control code — the value of this document is thinking it through, not pretending it's finished.
- **Why it's worth doing**: the cerebellum side reuses 01's CAN master controller and fault safety; the actuation side reuses 03's FOC; the brain side reuses 05's RK3568 and Socket experience. Every layer is backed by a delivered project — what's missing is the one step of connecting them and aligning their time scales.

---

## 📚 Embedded Technical Articles (26 + 4 tool notes)

*Engineering write-ups: principles + pseudocode + measured debug records. Full index at [docs/README.md](./docs/README.md) (Chinese).*

**A. Production Firmware & Reliability**
- 🔴 [OTA for Mass-Production Firmware: Dual Partition + CRC + Power-Loss Rollback](./docs/firmware‑mass‑produce.md)
- 🔴 [Non-Blocking FSM Design: From Blocking Delays to Tick-Driven State Machines](./docs/realtime‑fsm.md)
- 🔴 [Cortex-M HardFault Localization and Debug Techniques](./docs/cortex-m-hardfault-debug.md)
- 🔴 [FreeRTOS Pitfall Notes: Stack Overflow, Priority Inversion, Critical Sections](./docs/freertos-trap-notes.md)

**B. Motors & Control**
- 🔴 [Voltage-Mode FOC Without Current Sensing — Practice](./docs/motor‑foc‑control.md)
- 🔴 [Sensorless FOC Rotor Observers: SMO, PLL Extraction, and Low-Speed HFI](./docs/sensorless-foc-observer.md)
- 🔴 [PID in Engineering Practice: Temperature/Motor Tuning, Overshoot Suppression, Feedforward](./docs/pid-engineering-tuning.md)

**C. Communication Protocols & Peripherals**
- 🟡 [Modbus-RTU + COBS/CRC Protocol Stack: Design and Pitfalls](./docs/comm‑protocol‑driver.md)
- 🟡 [CAN Bus Development: Filters, Error States, Bus-Off Recovery](./docs/can-bus-practice.md)
- 🟡 [MQTT Client in Practice: Reconnect, Last Will, QoS Selection](./docs/mqtt-practice-note.md)
- 🔴 [RK3568 Edge Gateway: Socket Communication, Reconnect, Local Caching](./docs/linux-edge-socket-cache.md)
- 🟡 [ADC Acquisition in Practice: Offset Calibration, Filtering, Mains-Noise Suppression](./docs/adc-engineering-practice.md)

**D. Audio/Video & GUI**
- 🔴 [I2S/PCM Audio in Practice: Locating Pops, Noise Floor, and Clicks](./docs/audio-pop-noise-debug.md)
- 🟡 [ESP-IDF Ring Buffer in Practice](./docs/esp32-ringbuf-practice.md)
- 🔴 [LVGL Embedded GUI in Practice: Memory, Frame Rate, Leak Hunting](./docs/lvgl-gui-practice.md)

**E. Embodied AI Series (11 articles)** — Platform 00 (Rock 5B + Qmini, planned) + Project 01 dual platform (🐕 HyperDog monolithic / 🐈 Nybble layered)

- **E.1 Heterogeneous Platform & Policy Deployment** (Rock 5B + Qmini · 00, in research): 🔴 [Brain–cerebellum heterogeneous architecture](./docs/embodied-brain-body-arch.md) ｜ 🔴 [RK3588 NPU deployment pipeline](./docs/rk3588-npu-deploy.md) ｜ 🔴 [Sim-to-Real: training to Qmini hardware](./docs/sim-to-real-legged.md) ｜ 🟡 [Qmini + Rock 5B bring-up roadmap](./docs/qmini-rock5b-bringup.md)
- **E.2 Whole-Robot Real-Time Control** (HyperDog · monolithic MCU + CAN): 🔴 [Firmware architecture](./docs/hyperdog-arch.md) ｜ 🔴 [CAN joint protocol](./docs/hyperdog-can-protocol.md) ｜ 🔴 [IMU fusion & vibration suppression](./docs/hyperdog-imu-fusion.md) ｜ 🔴 [Trot gait scheduling](./docs/hyperdog-gait-scheduler.md) ｜ 🔴 [Tiered fault safety](./docs/hyperdog-safety.md) ｜ 🟡 [Calibration & step-by-step tuning](./docs/hyperdog-calibration.md)
- **E.3 ROS 2 + micro-ROS Layered Control** (Nybble · boundary on the link): 🟡 [Moving the real-time boundary onto the link](./docs/ros2-micro-ros-layered.md)

**P. Extended Notes**: [docs/tech-notes](./docs/tech-notes/README.md) — embedded-C traps, STM32 peripheral pitfalls, FreeRTOS one-off cases, J-Link / logic-analyzer tips. (Chinese)

---

## 🗂 Repository Layout

```text
CYY-portfolio/
├── README.md                                         # Portfolio home (Chinese)
├── README_EN.md                                      # English version of this page
├── showcase/                                         # Five portfolio curation pages (A motion / B audio / C mechanical / D hardware / E data)
├── assets/                                           # Demo media (waveforms / audio / board photos / data, mostly TODO)
├── docs/                                             # 26 technical articles (Chinese)
│   └── tech-notes/                                   # Embedded C / drivers / RTOS fragments
├── internship/                                       # Internship projects (sanitized)
│   ├── GAOXINXING/
│   │   ├── internship_project_01-power-sequencer     # 8-channel power sequencer (production)
│   │   ├── internship_project_02-digital-audio-mixer # RISC-V digital audio mixer
│   │   └── internship_project_03-usb-audio-interface # Desktop USB sound card
│   └── ANOTECH/                                      # Product structural design (docs only)
└── personal_project/                                 # 9 systems + 1 pre-research
    ├── 00_embodied_ai_platform                       # 🚧 Embodied-AI edge platform (proposal)
    ├── 01_quadruped_robotics                         # Quadruped dual platform (MCU + ROS 2)
    │   ├── hyperdog_rtos_control                     # Monolithic MCU + CAN (design docs)
    │   └── nybble_micro_ros                          # ROS 2 + micro-ROS (design phase)
    ├── 02_gnss-flight-controller                     # BeiDou/GNSS flight controller
    ├── 03_foc-bldc-motor                             # BLDC FOC motor control
    ├── 04_dual-axis-motion-control                   # Dual-axis motion (DC + stepper)
    ├── 05_industrial-edge-gateway                    # Industrial IoT edge gateway (MCU+Linux)
    ├── 06_bms-and-charger                            # Storage BMS + smart charger
    ├── 07_stm32-tec-temperature-ctrl                 # Dual-loop high-precision temp control
    ├── 08_esp32-smart-terminals                      # ESP32-S3 vision/touch terminals
    └── 09_iot-basic-projects                         # IoT & consumer electronics practice
```

---

## 🏷 Legend

> Every project page ends with the same legend (explaining only the markers actually used on that page), so you'll never hit an unexplained symbol.

| Marker | Meaning |
|---|---|
| 👑 | Flagship: highest completeness, full-stack HW/SW loop, shown first in interviews |
| ⭐ | Recommended first read for that role |
| 🔴 | Advanced / production-grade difficulty: real-time behavior, fail-safety, complex algorithm implementation |
| 🟡 | Intermediate module: high-frequency embedded topics, common in campus-recruit interviews |
| 🟢 | Foundational practice: peripheral & protocol verification, showing technical breadth |
| 🚧 | In research / pre-research: plan and validation route done, no deliverables or measured data yet; metrics are targets |

> In the "Technical Articles" section above, 🔴 🟡 indicate **reading difficulty** only, unrelated to project level.

---

## ⚠️ Disclaimer

1. All internship-related code and materials are sanitized and contain no confidential business source code or internal BOM parameters;
2. Documents are original writing; downloading and reading locally is recommended;
3. Personal-project docs and code are for learning and exchange only; internship-project IP belongs to the corresponding companies — commercial use prohibited.
