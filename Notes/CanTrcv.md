#### CanTrcv（CAN 收发器管理器）：通过SPI控制硬件TCAN114x芯片(can收发器)的工作状态，如正常模式、睡眠模式、唤醒模式
CAN收发器是CAN总线系统的核心硬件组件，连接CAN控制器（负责协议处理，如单片机内置的CAN模块，输出的是 TTL/CMOS 电平的数字信号）与物理CAN总线（如双绞线），**实现“数字逻辑信号”和“总线差分信号”的双向转换**，同时提供总线驱动、电平匹配和保护功能。

|对比项|CanTrcv|CanNM|
|---|---|---|
|所属层级|微控制器抽象层MCAL|通讯服务层BSW|
|物理实物|控制CAN收发芯片硬件（如TCAN114x）|软件协议栈，运行在CPU上|
|主要职责|开关收发器电源，响应总线唤醒信号|发送/接收网络管理报文/调节节点睡眠同步|
|休眠控制方式|设置收发器为Sleep模式（低功耗）|发送最后一个NM报文后请求进入Passive状态|
|唤醒检测方式|检测总线上的显性电平变化（硬件级）|收到NM PDU后认为被唤醒，启动通信|

***

**CanTrcv休眠流程**(ECU触发)
```mermaid
sequenceDiagram
    participant App
    participant EcuM
    participant CanTrcv
    participant TCAN114x

    App->>EcuM: 请求睡眠（例如钥匙 OFF）
    EcuM->>CanTrcv: CanTrcv_SetOpMode(SLEEP)
    CanTrcv->>TCAN114x: 通过 SPI 设置为 Sleep 模式
    Note right of TCAN114x: 仅保留唤醒检测电路供电
    Note right of CanTrcv: 收发器不再响应 CAN 报文
```

**CanTrcv唤醒流程**(外部触发)
```mermaid
sequenceDiagram
    participant CAN_Bus
    participant TCAN114x
    participant CanTrcv
    participant EcuM
    participant MCU

    CAN_Bus->>TCAN114x: 出现唤醒帧（如其他 ECU 发送 NM 报文）
    TCAN114x-->>CanTrcv: 触发 WAKE 引脚上升沿（中断）
    CanTrcv->>EcuM: 调用回调函数 CanIf_Cbk_TriggerTransceiverWakeup()
    EcuM->>MCU: 启动时钟、恢复 CPU 运行
    EcuM->>CanTrcv: CanTrcv_SetOpMode(NORMAL)
    CanTrcv->>TCAN114x: 通过 SPI 恢复正常通信模式
```

**CanNM休眠流程**
```mermaid
sequenceDiagram
    participant EcuM
    participant CanNm
    participant PduR
    participant CanIf
    participant CanDrv
    participant CAN_Bus

    EcuM->>CanNm: Prepare Bus Sleep
    CanNm->>CanNm: 启动 NxTimeout 计时器
    loop 每个周期（如 100ms）
        CanNm->>PduR: Transmit(NM PDU)
        PduR->>CanIf: 发送 NM 报文
        CanIf->>CanDrv: 写入 CAN 控制器
        CanDrv->>CAN_Bus: 发送 CAN 帧（含 ReadySleepBit=1）
    end
    Note over CanNm: 其他节点收到后标记本节点即将休眠

    CanNm->>EcuM: NM State = Ready Sleep
    Note right of EcuM: 允许进入低功耗模式
```

**CanNM唤醒流程**
```mermaid
sequenceDiagram
    participant EcuM
    participant CanNm
    participant PduR
    participant CanIf
    participant CanDrv
    participant CAN_Bus

    EcuM->>CanNm: Start Network (Wake Up)
    CanNm->>CanNm: 初始化状态机 → RepeatMessageState
    loop 周期性发送（如 100ms）
        CanNm->>PduR: Transmit(NM PDU)
        PduR->>CanIf: 发送 NM 报文
        CanIf->>CanDrv: 写入控制器
        CanDrv->>CAN_Bus: 广播 NM 报文（含 Alive Bit）
    end
    Note over CanNm: 表示“我已上线，请注意”

    CanNm->>CanNm: 经过 NmRepeatMessageTime 后进入 NormalOperation
```

**整体协同流程图**
```mermaid
graph TD
    A[用户关闭钥匙KeyOFF] --> B[EcuM: 请求睡眠]
    B --> C["CanNm: 发送最后几帧NM报文<br>(ReadySleep=1)"]
    C --> D[CanNm: 进入ReadySleep状态]
    D --> E[EcuM: 准备好睡眠]
    E --> F["CanTrcv:SetMode(SLEEP)"]
    F --> G[TCAN114x:进入低功power模式<br>仅监听总线唤醒]
    G --> H[ECU 进入低功耗待机]

    I[其他 ECU 发送 NM 报文] --> J[TCAN114x 检测到总线跳变]
    J --> K[CanTrcv 中断触发]
    K --> L["CanTrcv调用CanIf_Cbk_TriggerTransceiverWakeup()"]
    L --> M[EcuM被唤醒]
    M --> N["CanTrcv: SetMode(NORMAL)"]
    N --> O[CanTrcv 恢复正常通信]
    O --> P[CanNm: Start Network]
    P --> Q[CanNm 开始发送 NM 报文]
    Q --> R[进入 NormalOperation 状态]
    R --> S[ECU 正常运行, 可通信]
```