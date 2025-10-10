#include "Platform_Types.h"
#include "IfxGtm.h"
#include "IfxPort.h"
#include "IfxSrc_reg.h"

// 定义心跳引脚
#define HEARTBEAT_PORT     &MODULE_P00
#define HEARTBEAT_PIN      0

// 定义 GTM 资源
#define GTM_INSTANCE       (&MODULE_GTM)
#define GTM_TIM_MODULE     IfxGtm_Tim_Ch_0
#define GTM_TIM_CHANNEL    0
#define GTM_TIM_IRQ_GROUP  0

// 超时阈值（单位：ticks）
// 假设 GTM clock = 100MHz，周期 = 10ns
// 设定最大允许间隔为 20ms → 20ms / 10ns = 2,000,000 ticks
#define WATCHDOG_TIMEOUT   2000000UL

// 函数声明
void init_gtm_watchdog(void);
void feed_external_watchdog(void); // 相当于“喂狗”
void gtmtim_isr(void);

// 全局变量
volatile boolean g_system_hung = FALSE;

//--------------------------------------------------------------------------
// 初始化 GTM 看门狗
//--------------------------------------------------------------------------
void init_gtm_watchdog(void)
{
    // 1. 配置 GPIO 为输出（用于发送心跳）
    IfxPort_setPinMode(HEARTBEAT_PORT, HEARTBEAT_PIN, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinHigh(HEARTBEAT_PORT, HEARTBEAT_PIN);

    // 2. 初始化 GTM
    IfxGtm_enable(GTM_INSTANCE);
    IfxGtm_Cmu_setClockFrequency(GTM_INSTANCE, IfxGtm_ClockSource_cm0, 100000000); // 100MHz

    // 3. 配置 TIM 通道为输入捕获模式
    IfxGtm_Tim_Ch config;
    IfxGtm_Tim_Ch_init(&config);

    config.gtm = GTM_INSTANCE;
    config.module = GTM_TIM_MODULE;
    config.channel = GTM_TIM_CHANNEL;
    config.clock = IfxGtm_Tim_ClockSource_clock0; // 使用 cm0 时钟
    config.inputSignal.edge = IfxGtm_Tim_InputSignalEdge_risingEdge; // 捕获上升沿
    config.triggerMode = IfxGtm_Tim_TriggerMode_pulseWidthAndPeriod; // 测量周期

    IfxGtm_Tim_Ch_setup(&config);

    // 4. 配置超时比较器（ACB）
    IfxGtm_Acb_Set acbConfig;
    IfxGtm_Acb_Set_init(&acbConfig);
    acbConfig.gtm = GTM_INSTANCE;
    acbConfig.acbIndex = 0;
    acbConfig.timModule = GTM_TIM_MODULE;
    acbConfig.timChannel = GTM_TIM_CHANNEL;
    acbConfig.compareValue = WATCHDOG_TIMEOUT;
    acbConfig.compareMode = IfxGtm_Acb_CompareMode_timeout;
    acbConfig.action = IfxGtm_Acb_Action_interrupt;

    IfxGtm_Acb_Set_setup(&acbConfig);

    // 5. 配置中断
    volatile Ifx_SRC_SRCR *src = &SRC_GTM0INT0; // 假设 INT0 映射到 TIM_CH_0
    src->B.CLKSEL = 0;  // 选择服务核心（CPU0）
    src->B.TOS = 0;     // 发送到 CPU
    src->B.SRE = 1;     // 使能中断
    src->B.SETCLR = 1;  // 设置有效

    // 6. 使能 GTM 模块中断
    IfxGtm_enableNotification(GTM_INSTANCE, GTM_TIM_IRQ_GROUP);

    // 7. 开启全局中断
    __enable_interrupt();
}

//--------------------------------------------------------------------------
// “喂狗”函数：产生一个心跳脉冲
//--------------------------------------------------------------------------
void feed_external_watchdog(void)
{
    // 输出一个 1us 宽的脉冲（约10个时钟周期）
    IfxPort_setPinLow(HEARTBEAT_PORT, HEARTBEAT_PIN);
    for(volatile int i = 0; i < 10; i++);
    IfxPort_setPinHigh(HEARTBEAT_PORT, HEARTBEAT_PIN);
}

//--------------------------------------------------------------------------
// GTM TIM 中断服务函数 —— 看门狗超时处理
//--------------------------------------------------------------------------
void gtmtim_isr(void)
{
    // 清除中断标志（根据实际寄存器地址）
    // 示例：GTM_INSTANCE->IRQ_NOTIFY.U = (1 << GTM_TIM_IRQ_GROUP);

    // 标记系统异常
    g_system_hung = TRUE;

    // 执行安全动作
    enter_safe_state();

    // 可选：触发系统复位
    // SCU_SW_RESET(); 
}

//--------------------------------------------------------------------------
// 安全状态函数
//--------------------------------------------------------------------------
void enter_safe_state(void)
{
    // 关闭电机驱动
    // 切断高压继电器
    // 点亮故障灯
    // 记录 DTC 故障码
}

//--------------------------------------------------------------------------
int main(void)
{
    // 初始化系统
    System_init();

    // 初始化 GTM 看门狗
    init_gtm_watchdog();

    while (1)
    {
        // 正常任务处理
        task_scheduler();

        // 定期喂狗（假设每10ms喂一次）
        feed_external_watchdog();

        // 如果这里卡住超过20ms，GTM 看门狗会触发中断
    }
}
//--------------------------------------------------------------------------