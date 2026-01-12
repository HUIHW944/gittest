### **网络管理状态机（CANNM State Machine）：**
1. **Bus-Sleep Mode（总线睡眠模式）**
	- ECU处于低功耗状态。
	- 不发送任何NM报文。
	- CAN控制器处于“Listen-Only”或“Power-down”模式，仅能检测总线活动作为唤醒源。
	- **只有外部事件可以唤醒（如：KL15上电(本地)、CAN唤醒帧（被动）、GPIO中断等）。**
2. **Prepare Bus-Sleep Mode（准备睡眠模式）**
	- 来自 Network Mode 的过渡状态。
	- 表示本节点已无通信需求，等待其他节点也释放网络。
	- 如果在此期间收到 NM 报文（表示他人需要网络），则返回Network Mode。
	- 若超时**未收到 NM 报文**，则允许进入 Bus-Sleep。
3. **Network Mode（网络模式）**
	- (a) Repeat Message State（重复消息状态）刚从睡眠唤醒后的初始阶段。
    	- 行为：
			- 以短周期发送 NM 报文（如 80ms，==快发==）。
			- 目的：快速告知网络“我已上线”，并确认其他节点是否也在运行。
		- 出口条件（必须同时满足）：
			- 本地已完成初始化（由 Com 模块通知 Nm）；
			- 接收到至少一个远程节点的有效 NM 报文（证明网络已激活）；
			- 本节点可能还需要继续通信（即仍有请求）。
		- 满足后 → 进入 Normal Operation State。
	- (b) Normal Operation State（正常运行状态）
		- 网络已稳定。
		- NM 报文以长周期发送（如 500ms 或 1s，==慢发==），减少总线负载。
		- 只要本节点或网络中有任一节点仍在请求通信（通过 NM 报文中的 Request Bit），就不允许睡眠。
        - 当本地唤醒置成休眠，或被动唤醒定时器到时，**NM报文不接收，只接收App报文** → 可进入Ready Sleep。
	- (c) Ready Sleep State（准备睡眠状态）
		- 本节点不再需要通信，撤销“请求位”。
		- 继续发送 NM 报文一段时间，让其他节点知道它可以睡眠。
        - 若在此期间**没有收到其他NM报文和App报文** → 认为全网空闲 → 可进入Prepare Bus-Sleep。


### **完整的休眠唤醒流程** : 正常唤醒 → 建立网络 → 通信 → 休眠
- 步骤1：唤醒源触发
    - 例如：钥匙遥控信号 → BCM的GPIO中断 → 触发 MCU 唤醒。
    - 或者：CAN总线上出现有效帧 → CAN控制器发出唤醒中断。
- 步骤2：EcuM初始化底层模块
    - EcuM_StartupTwo()
        - 初始化MCU，Watchdog，Ram，Clock
        - 初始化CanDrv，CanIf，PduR，Com，Nm等模块
        - 启动CanNm模块
- 步骤3：NM进入Repeat Message State
    - CanNM开始以快周期发送NM报文（含Node ID + Request Bit = 1）
    - 同时监听是否有其他节点回复NM报文
- 步骤4：同步网络状态
    - 收到其他节点NM报文 → 表明网络正在运行
    - 本地Com模块通知 “Communication Ready”
    - CanNM切换至 Normal Operation State（开始慢发）
- 步骤5：应用层开始通信
    - 如：BCM发送车门状态 → Gateway → 仪表显示
- 步骤6：通信结束，请求睡眠
    - 应用层调用Nm_DisableCommunication() 或不再请求通信
    - Com模块通知Nm：“不再需要通信”
    - NM进入Ready Sleep State，NM报文中清除Request Bit
- 步骤7：等待网络空闲
    - 定期检查是否还能接收到其他节点的NM报文
    - 若连续一段时间（如 2 秒）无任何NM报文 → 认为网络空闲
- 步骤8：进入 Prepare Bus-Sleep → Bus-Sleep
    - NM通知EcuM：“我可以睡眠了”
    - EcuM调用EcuM_SetWakeupEvent() 注册唤醒源
    - 关闭不必要的外设，进入低功耗模式
    - CAN控制器设置为“Wake-up capable”模式

### 以CanNM模块为例
|函数|目的|操作|    
|:---|:---|:---|
|CanNM_Init|将模块及其所有通道置于已知的初始状态|将模块整体状态设为CANNM_INIT|
|^|^|遍历每个通道|
|^|^|如配置了节点ID，则将其写入待发送NM报文(TxPdu)的指定位置|
|^|^|清除控制位向量(CBV)|
|CanNm_MainFunction|驱动所有定时器|通常由RTE在MainFunction_Period时间间隔内调用一次（如10ms或20ms）|
|^|^|遍历所有通道的所有定时器，调用CanNm_Internal_TimerTick|
|^|^|TimerTick会根据主函数周期递减TimeLeft。当TimeLeft<=0时，停止定时器并调用其注册的 ExpiredCallback|
|CanNm_NetworkRequest|应用层==主动请求==建立或维持网络通信|从<font color=red>BUS_SLEEP</font>进入：触发BusSleep_to_RepeatMessage状态转换，开始“==快发==”NM报文。如果启用了ActiveWakeupBit，则置位；如果配置了ImmediateNmTransmissions，则立即触发发送（通过调用MessageCycleTimerExpiredCallback实现）|
|^|^|从<font color=red>PREPARE_BUS_SLEEP</font>进入：类似BUS_SLEEP，但可能涉及被动重启逻辑|
|^|^|在<font color=red>NETWORK</font>模式下：如果当前在READY_SLEEP或NORMAL_OPERATION，并且配置允许，可以再次进入REPEAT_MESSAGE状态以“唤醒”网络|
|CanNm_NetworkRelease|应用层声明不再需要网络通信|仅在NORMAL_OPERATION状态下调用有效|
|^|^|将Requested标志置为FALSE|
|^|^|触发NormalOperation_to_ReadySleep状态转换。此时 NM 报文仍会发送（==慢发==），但不再携带“请求”信息|
|CanNm_PassiveStartUp|==被动启动==。当检测到总线上已有其他节点的NM报文时，本节点应被动加入网络|仅在非NETWORK模式下有效，直接进入REPEAT_MESSAGE状态，同步网络|
|CanNm_RxIndication|当CanIf成功接收到一个NM报文时，会调用此函数|更新RxLastPdu，复制接收到的数据|
|^|^|在<font color=red>BUS_SLEEP</font>模式：状态不变，但会发出Nm_NetworkStartIndication给上层，告知有网络活动，可能需要唤醒|
|^|^|在<font color=red>PREPARE_BUS_SLEEP</font>模式：收到报文意味着网络活跃，必须返回REPEAT_MESSAGE状态|
|^|^|在<font color=red>NETWORK</font>模式：重置TimeoutTimer，检查接收到的报文是否有RepeatMessageRequest Bit。如果有，则立即进入REPEAT_MESSAGE状态|
|CanNm_TxConfirmation|当CanIf成功或失败发送一个NM报文后，会调用此函数|如果发送成功，则调用NetworkMode_to_NetworkMode，重置TimeoutTimer|