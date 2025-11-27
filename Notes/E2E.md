E2E（End-to-End Protection）是一种用于汽车电子系统的“端到端数据保护机制”，它的核心目标是：
 - 防止数据被篡改（完整性）
 - 防止重放攻击（新鲜度）
 - 检测丢包或错序（顺序性）
 - 使用CRC校验→接收方重新计算CRC，不一致则丢弃
 - 使用Rolling Counter→只接受递增的消息，跳跃过大为异常，一般为4字节（0~15）
它就像给每条消息贴上“防伪标签 + 编号”，确保通信双方能判断这条消息是不是真的、新的、完整的。 

```mermaid
graph TD
    subgraph Sending["发送方 (如: 发动机ECU)"]
        A[Application<br>生成信号] --> B[Com Module<br>打包CAN报文]
        B --> C[E2eSrv_ProtectWrapper<br>查表获取配置范围]
        C --> F["E2e_Protect(i)"]
        F --> G["E2e_WriteCountValue()"]
        F --> H["E2e_ComputeCRC()"]
        F --> I["E2e_WriteCRC()"]
        I --> J[CAN Bus<br>ID=0x425]
    end
    J --> K["Receiving<br>接收方 (如: VCU)"]
    subgraph Receiving["接收方 (如: VCU)"]
        K --> L[E2eSrv_CheckWrapper<br>查表获取检查范围]
        L --> O["E2e_Check(i)"]
        O --> P["ReadCountFromData()"]
        O --> Q["E2E_CheckRollCounter()"]
        O --> R["ReadCRCFromData()"]
        O --> S["E2e_ComputeCRC()"]
        S --> T[CRC Compare]
        T --> U{Result}
        U -- OK --> V[Accept Message]
        U -- Fail --> W[Reject and Log Error]
    end

    style Sending fill:#f0f8ff,stroke:#333
    style Receiving fill:#fff8f0,stroke:#333
```