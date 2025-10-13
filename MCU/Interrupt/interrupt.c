/*============================================
 * 文件: main.c
 * 芯片: Infineon AURIX TC334
 * 功能: 实现外部中断 + STM 定时中断
 *============================================*/
#include "Platform_Types.h"
#include "Ifx_Types.h"
#include "IfxCpu_Irq.h"
#include "IfxScuWdt.h"
#include "IfxPort.h"
#include "IfxStm.h"

// ========================
// 函数声明
// ========================
void systemInit(void);
void initExternalInterrupt(void);
void initStmInterrupt(void);

// ========================
// 全局变量
// ========================
volatile boolean g_extFlag = FALSE;
volatile uint32 g_stmCounter = 0;

// ========================
// 中断服务函数声明（使用 IFX_INTERRUPT 宏）
// 格式: IFX_INTERRUPT(函数名, Core ID, Priority)
// ========================

// 外部中断 ISR（对应 ERU -> SRC12）
IFX_INTERRUPT(extint_handler, 0, 13); 

// STM0 匹配中断 ISR（对应 STM0_SR0）
IFX_INTERRUPT(stm_handler, 0, 11);    

// 系统初始化
void systemInit(void)
{
    // 关闭看门狗（仅用于开发）
    IfxScuWdt_disableCpuWatchdogTimer();

    // 注意：系统时钟已在启动代码中初始化
    // 默认 fCPU = 180 MHz, fSTM = fCPU / 2 = 90 MHz → T_tick = 11.11ns
}

//外部中断配置（P00.0上升沿触发）
void initExternalInterrupt(void)
{
    // 1. 设置 P00.0 为输入，上拉
    IfxPort_setPinMode(&MODULE_P00, 0, IfxPort_Mode_input);
    IfxPort_setPinPullMode(&MODULE_P00, 0, IfxPort_Pull_up);

    // 2. 配置 ERU 输入通道 0（EICHE0）连接到 P00.0
    SCU_ERU0_IGCR0.B.SRCSEL = 0;  // 来自 PORT 边沿检测
    SCU_ERU0_IGCR0.B.EDCON = 1;   // 禁用(00)、上升沿触发(01)、下降沿触发(10)、双边沿触发(11)
    SCU_ERU0_IGCR0.B.ENINT0 = 0;  // 暂时禁用中断
    SCU_ERU0_INOUTI0.B.P00_0 = 1; // 使能 P00.0 到 ERU 输入线 0

    // 3. 配置 ERU 输出通道 EICR0 映射到 SR12
    SCU_ERU0_EICR0.B.IEN = 1;    // 使能中断输出
    SCU_ERU0_EICR0.B.ENIRQ = 1;  // 使能 IRQ
    SCU_ERU0_EICR0.B.TOS = 0;    // 发送到 CPU0
    SCU_ERU0_EICR0.B.SR_ID = 12; // 使用 SERVICE REQUEST LINE 12

    // 4. 配置 SRC 寄存器（中断向量控制器）
    volatile Ifx_CPU_SRC src;
    src = SRC_SCUERU0;                     // ERU0 的 SRC 寄存器
    IfxSrc_init(src, IfxSrc_Tos_cpu0, 13); // 优先级 13
    IfxSrc_enable(src);                    // 启用中断源
}

// STM0 定时中断配置（周期 1ms）
void initStmInterrupt(void)
{
    // 假设 fSTM = 90 MHz → 每个计数周期 = 1 / 90M ≈ 11.11 ns
    // 目标：1ms = 1,000,000 ns → 计数值 = 1,000,000 / 11.11 ≈ 90,000
    const uint32 tickCount = 90000UL;

    // 1. 初始化 STM0 计数器
    MODULE_STM0.CMTIM0.U = 0;                     // 清零当前时间

    // 2. 设置匹配值（CMODATA0）
    MODULE_STM0.CMODATA0.U = tickCount;

    // 3. 使能比较通道 0
    MODULE_STM0.CMCON.B.MS0 = 1;                  // 使能匹配
    MODULE_STM0.CMCON.B.MCI0 = 0;                 // 清除匹配标志

    // 4. 配置中断
    MODULE_STM0.ICR.B.SRM = 1;                    // 使能匹配中断
    MODULE_STM0.ICR.B.TBIE = 0;                   // 禁用溢出中断（可选）

    // 5. 配置 SRC（中断源控制寄存器）
    volatile Ifx_CPU_SRC src = SRC_STM0SR0;       // STM0_SR0 对应匹配中断0
    IfxSrc_init(src, IfxSrc_Tos_cpu0, 11);        // 优先级 11
    IfxSrc_enable(src);                           // 启用中断
}

//STM0 中断服务函数（周期1ms）
IFX_INTERRUPT(stm_handler, 0, 11)
{
    // 必须清除中断标志位
    MODULE_STM0.ISR.B.SR0 = 1;

    // 更新计数器
    g_stmCounter++;

    // 可在此添加周期性任务（如 ADC 触发、PID 控制等）
}

// 外部中断服务函数（P00.0 上升沿触发）
IFX_INTERRUPT(extint_handler, 0, 13)
{
    // 清除 ERU 中断标志（写 1 清零）
    SCU_ERU0_EICR0.B.LINES = 1;

    // 设置标志供主循环处理
    g_extFlag = TRUE;
}

// ========================
// 主函数
// ========================
int core0_main(void)
{
    // 启用全局中断（EA = 1）
    IfxCpu_enableInterrupts();

    systemInit();

    // 配置外设中断
    initExternalInterrupt();
    initStmInterrupt();

    // 初始化 GPIO（LED）
    IfxPort_setPinMode(&MODULE_P00, 1, IfxPort_Mode_output);
    IfxPort_setPinMode(&MODULE_P00, 2, IfxPort_Mode_output);
    IfxPort_setPinLow(&MODULE_P00, 1);
    IfxPort_setPinLow(&MODULE_P00, 2);

    // 主循环
    while (1)
    {
        if (g_extFlag)
        {
            IfxPort_togglePin(&MODULE_P00, 1);  // P00.1 LED 翻转
            g_extFlag = FALSE;
        }

        // 每 500ms 翻转 P00.2 LED
        if (g_stmCounter % 500 == 0)
        {
            IfxPort_togglePin(&MODULE_P00, 2);
            while (g_stmCounter % 500 == 0); // 防抖处理（简单方式）
        }
    }

    return 0;
}