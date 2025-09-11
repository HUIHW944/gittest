#include <stdio.h>
#include <stdint.h>

// 模拟定时器（用于实现软件看门狗）
typedef struct {
    uint32_t timeout;       // 超时时间（毫秒）
    void (*callback)(void); // 超时回调函数
    uint32_t count;         // 计时计数器
} SoftWatchdog;

// 全局看门狗实例
SoftWatchdog g_watchdog;

// 模拟系统复位函数
void system_reset(void) 
{
    printf("[System reset] try to get back to normal operation...\n");
    // 实际硬件中会调用复位指令，如NVIC_SystemReset()
}

// 看门狗超时回调函数
void watchdog_timeout_callback(void) 
{
    printf("[Timeout Exception] did not feed the dog in time!\n");
    
    // 1. 记录故障信息（示例：打印当前状态）
    printf("[Fault logging] timeout time counter value: %d\n", (int)g_watchdog.count);
    
    // 2. 尝试软件修复（示例：重启关键模块）
    printf("[Try to recover] restart the communication module...\n");
    
    // 3. 若修复无效，触发系统复位
    system_reset();
}

// 初始化软件看门狗
void watchdog_init(uint32_t timeout) 
{
    g_watchdog.timeout = timeout;
    g_watchdog.callback = watchdog_timeout_callback; // 绑定回调函数
    g_watchdog.count = 0;
    printf("watchdog initialization completes, timeout timeout: %dms\n", timeout);
}

// 喂狗操作（重置计数器）
void watchdog_feed(void) 
{
    g_watchdog.count = 0;
    printf("Feeding the dog is successful and the counter resets\n");
}

// 模拟定时器中断（每隔1ms调用一次，实际由硬件定时器触发）
void timer_tick(void) 
{
    g_watchdog.count++;
    if (g_watchdog.count >= g_watchdog.timeout) {
        // 超时：调用回调函数处理
        g_watchdog.callback();
    }
}

int main() 
{
    // 初始化看门狗，超时时间设为1000ms（1秒）
    watchdog_init(1000);
    
    // 模拟正常运行（前5次按时喂狗）
    for (int i = 0; i < 5; i++) {
        // 模拟程序执行（500ms）
        for (int j = 0; j < 500; j++) {
            timer_tick(); // 模拟1ms计时
        }
        watchdog_feed(); // 喂狗
    }
    
    // 模拟异常：第6次不喂狗，导致超时
    printf("===== Simulation program abnormalities, stop feeding the dog =====\n");
    while (1) {
        timer_tick(); // 继续计时，直至超时
    }
    
    return 0;
}
