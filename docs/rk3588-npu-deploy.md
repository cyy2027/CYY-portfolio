# RK3588 NPU 部署链路：从 ONNX 到策略上板

> 📄 文档路径：`docs/rk3588-npu-deploy.md`

> ✍️ 把训练好的策略塞进 Rock 5B 的 NPU，中间要过四道关：图导出、量化校准、算子兼容、板上调度。本文按这四道关走一遍，给出每一关的检查项、常见掉坑点与延迟测量方法。

> ⚠️ **文档定位**：基于 Rock 5B（RK3588）+ RKNN-Toolkit2 官方工具链的**部署路线梳理**，硬件尚未采购，文中时延为**目标值**，未实测。目标载体为 [Unitree Qmini](https://github.com/unitreerobotics/Qmini)，策略参考 [RoboTamer4Qmini](https://github.com/vsislab/RoboTamer4Qmini) 的 PPO 输出。上层架构见[《大脑—小脑异构架构》](./embodied-brain-body-arch.md)。

---

## 一、先看这块板子能给什么

Rock 5B 的核心是 RK3588：

| 项 | 规格 | 对部署的意义 |
|---|---|---|
| CPU | 4×Cortex-A76 @2.4 GHz + 4×Cortex-A55 @1.8 GHz | 大小核异构，绑核策略直接影响抖动 |
| NPU | 3 核，合计 6 TOPS @INT8 | 单核可独立调度，支持多模型并行 |
| 内存 | LPDDR4x/5，8～32 GB | 足够放策略 + 视觉缓冲，不必精算 |
| 视频 | 支持多路 MIPI CSI 输入 | 后续加相机不必换板 |
| 接口 | 千兆以太、USB 3.0、PCIe、40-pin | 与 Qmini 的通信链路有得选 |

对策略推理来说，几个关键事实：

1. **6 TOPS 是 INT8 峰值**，且是三个核加起来。单核 2 TOPS，模型不能自动跨核拆分 —— 多核是用来跑**多个模型**或**多路数据**的，不是加速单个大模型。
2. **FP16 性能远低于 INT8**。同一个模型用 FP16 跑，时延可能是 INT8 的 2～4 倍。如果你的策略对量化敏感，先想清楚是接受精度损失还是接受时延。
3. **NPU 只做算子，不做前后处理**。归一化、resize、拼向量这些还是 CPU 干，这部分往往比推理本身还慢。

---

## 二、四道关

```text
PyTorch (.pt)
    │ ① 导出
    ▼
ONNX (.onnx)  ── 检查：动态轴、算子集版本、控制流
    │ ② 转换 + 量化
    ▼
RKNN (.rknn)  ── 检查：算子是否全部落到 NPU、量化精度
    │ ③ 板上加载
    ▼
rknn_runtime ── 检查：输入输出布局、内存复用
    │ ④ 调度
    ▼
50 Hz 闭环
```

### 2.1 关一：导出 ONNX

策略网络本身通常很"干净"（MLP、GRU、小 CNN），麻烦出在导出时的三件事：

- **动态 batch / 序列长度**。RL 策略多是单步推理（batch=1），导出时把 batch 固定成 1，避免后面 RKNN 对动态 shape 支持不佳。
- **训练态残留**。`Dropout`、`BatchNorm` 的 training 分支、`softmax` 的温度参数，都要在导出前 `eval()` 掉。
- **控制流**。策略里若有 `if`、循环（比如迭代式 IK），ONNX 会导出成 `If`/`Loop` 节点，RKNN 对这类节点支持有限，往往整段回落到 CPU —— 性能直接崩。**能展开就展开，能改成 mask/权重就改。**

导出的自检方式：用 `onnxruntime` 在 PC 上跑一遍，和 PyTorch 输出逐元素比对，误差应小于 1e-5。这一步不做，后面出问题会分不清是导出错还是量化错。

### 2.2 关二：量化与校准

RKNN 的核心收益来自 INT8 量化，代价是精度。转换脚本大致长这样（示意，非可运行代码）：

```python
from rknn.api import RKNN

rknn = RKNN(verbose=True)
rknn.config(
    mean_values=[[0, 0, 0]],      # 输入归一化可放在这里，也可放前处理
    std_values=[[1, 1, 1]],
    target_platform='rk3588',
    quantized_dtype='asymmetric_quantized-8',
    optimization_level=3,
)
rknn.load_onnx(model='policy.onnx')
rknn.build(do_quantization=True, dataset='calib_list.txt')
rknn.export_rknn('policy.rknn')
```

**校准集是唯一需要动脑的地方**，三个原则：

1. **分布要对**。用策略真实会遇到的状态做校准输入（关节角范围、速度范围、IMU 量程），不要拿全零或随机数。
2. **数量不必多**，100～500 条足够，但要覆盖边界。姿态接近极限、速度最大、IMU 饱和 —— 这些样本不放进去，量化后会在这几个点上失真。
3. **校准完必须做逐层误差分析**。RKNN-Toolkit2 提供 `accuracy_analysis`，输出每层的余弦相似度与欧氏距离。**找第一个误差突然变大的层**，那是精度损失源头，而不是笼统地说"量化掉点"。

对控制策略而言，量化误差的容忍度有个特殊之处：**它不体现在"动作错"，而体现在"动作抖"**。量化台阶会让输出在相邻档位间跳变，经过 PD 环放大成力矩脉动。所以评估量化不能用分类准确率，要用**轨迹跟踪误差**和**输出抖动幅度**。

### 2.3 关三：算子兼容

RKNN 不是万能编译器。常见回落情形：

| 情形 | 后果 | 处理 |
|---|---|---|
| 自定义算子 | 不支持，构建失败 | 用基础算子重组；或拆模型，该段放 CPU |
| `Softmax` / `LayerNorm` 在特定维度 | 支持但慢 | 改维度顺序，或合并进相邻层 |
| 动态 shape | 不支持 | 固定 shape，多 shape 就多导几个模型 |
| 高维 reshape / transpose | 触发内存搬移 | 减少布局变换，尽量在 NCHW 内做完 |

判断依据：构建日志里会有算子分布表，**看 NPU 算子占比**。如果 CPU 部分占比超过 20%，时延基本不可控（CPU 侧受调度影响）。

### 2.4 关四：板上调度

板上的推理循环（C 接口，示意）：

```c
rknn_context ctx;
rknn_init(&ctx, model_data, model_size, 0, NULL);

while (running) {
    /* 1. 组观测向量（CPU 侧，往往是最慢的一段） */
    build_observation(obs_buf);

    rknn_input inputs[1] = {0};
    inputs[0].index = 0;
    inputs[0].buf   = obs_buf;
    inputs[0].size  = OBS_DIM * sizeof(float);
    inputs[0].type  = RKNN_TENSOR_FLOAT32;   /* 让 runtime 自己做量化 */
    inputs[0].fmt   = RKNN_TENSOR_NDIM;

    rknn_inputs_set(ctx, 1, inputs);
    rknn_run(ctx, NULL);

    rknn_output outputs[1] = {0};
    outputs[0].want_float = 1;
    rknn_outputs_get(ctx, 1, outputs, NULL);

    publish_action((float *)outputs[0].buf);
    rknn_outputs_release(ctx, 1, outputs);

    sleep_until_next_tick(20);   /* 50 Hz */
}
```

四个容易忽略的点：

1. **`want_float=1` 会触发反量化**，多一次内存拷贝。若后续要自己算，可以直接拿 INT8 输出自己反量化，省一次。
2. **线程绑定**。推理线程绑到大核（A76），并且 `SCHED_FIFO` 提优先级。否则小核调度会让 p99 时延翻倍。
3. **CPU 调频**。默认 `ondemand` 调频器会在负载变化时升降频，造成时延台阶。部署时锁 `performance` 模式。
4. **内存零拷贝**。输入输出 buffer 提前分配好复用，不要在循环里 malloc —— 一次缺页就是几百微秒。

---

## 三、时延怎么测

RKNN 自带性能接口，比在用户态掐表准：

```python
# PC 侧（仿真时延，仅作参考）
rknn.eval_perf(is_print=True)
```

板上用 `rknn_query` 拿每一层的耗时，或直接在循环里打时间戳统计分布：

| 指标 | 目标（Qmini 级 MLP 策略） | 说明 |
|---|---|---|
| 单帧推理 | ≤ 2 ms | INT8，batch=1 |
| 前后处理 | ≤ 2 ms | 通常是瓶颈 |
| 端到端（观测→动作） | ≤ 5 ms | 占 20 ms 预算的 1/4 |
| p99 / 均值 | ≤ 1.5 | 超过说明调度有问题 |

**只看均值会被骗**。用 1000 帧样本算 p50 / p99 / max 三个数，如果 p99 是均值的 3 倍以上，先去查线程绑定和调频器。

---

## 四、这套链路的坑（提前记下）

| 现象 | 根因 | 排查 |
|---|---|---|
| 转换成功但板上输出全零 | 输入 format/type 与模型不匹配 | 打印 `rknn_query` 的输入属性对照 |
| 时延忽高忽低 | CPU 调频 + 线程漂移 | 锁 performance、绑核 |
| 量化后动作抖动明显 | 输出层量化台阶被 PD 放大 | 输出层尝试 `int16` 或 `float`（部分 ops 支持混合精度） |
| 构建日志大量 CPU 算子 | 控制流或自定义算子回落 | 展平控制流、拆模型 |
| 首次推理特别慢 | 模型加载 + 内存分配 | 预热若干帧再进闭环 |
| 跑几分钟后变慢 | 内存泄漏（未 release 输出） | 检查 `rknn_outputs_release` 配对 |

---

## 五、与 Qmini 的衔接

Qmini 原版用树莓派 4B + ONNX Runtime 跑 RoboTamer4Qmini 的策略（[RoboTamerSdk4Qmini](https://github.com/vsislab/RoboTamer4Qmini) 自带 C++ 部署 SDK，支持 CPU/GPU backend）。换成 Rock 5B 后，链路变成：

```text
原版：  ONNX ──► ONNX Runtime (CPU/CUDA) ──► 关节指令
本方案：ONNX ──► rknn (NPU) ──► 关节指令
```

需要改的部分只有推理后端，**观测构造、动作后处理、安全校验都可复用 SDK 逻辑**。这也是选 Rock 5B 而不是从零写一套的理由：生态上已经有能跑的整条链，换的只是最后一段算力。

需要先确认的事：SDK 的推理接口是否做了抽象（能否替换 backend）。若写死在 ONNX Runtime，得自己包一层。

---

## 参考

- Rockchip RKNN-Toolkit2 / rknn-runtime 官方仓库与文档（算子支持列表、量化指南）
- Radxa Rock 5B 产品文档与 RK3588 数据手册
- `vsislab/RoboTamer4Qmini`：ONNX Runtime 部署 SDK 与真机接线流程
- `unitreerobotics/Qmini`：整机 BOM、电气框图、URDF

## 📌返回导航

[⬅ 返回技术文档目录](./README.md)　｜　[⬅ 返回作品集首页](../README.md)
