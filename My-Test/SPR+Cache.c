#include <stdint.h>
#include <stdio.h>

#define DCACHE_CTRL (*((volatile uint32_t *)0xF0000C00)) 
#define PCACHE_CTRL (*((volatile uint32_t *)0xF0000C04))
// 1. 存储在PSPR（程序暂存RAM）
// 使用编译器特定指令将变量放入PSPR区域
__attribute__((section(".pspr_data"))) 
volatile uint32_t pspr_var = 0x11111111;

// 2. 存储在DSPR（数据暂存RAM）
// 使用编译器特定指令将变量放入DSPR区域
__attribute__((section(".dspr_data"))) 
volatile uint32_t dspr_var = 0x22222222;

// 3. 存储在可被PCache缓存的区域（通常是Flash或主RAM）
// 常量代码和只读数据会被PCache缓存
const uint32_t pcache_const = 0x33333333;

// 4. 存储在可被DCache缓存的区域（通常是主RAM）
// 普通全局变量会被DCache缓存
uint32_t dcache_var = 0x44444444;

// 函数代码会被加载到Flash并由PCache缓存
void process_data(void) {
    // 局部变量通常在栈上，栈可能位于PSPR
    uint32_t stack_var = 0x55555555;
    
    // 操作PSPR中的变量
    pspr_var += 1;
    
    // 操作DSPR中的变量
    dspr_var *= 2;
    
    // 读取会被PCache缓存的常量
    uint32_t temp = pcache_const + stack_var;
    
    // 操作会被DCache缓存的变量
    dcache_var = temp;
}

// 缓存控制函数（示意）
void enable_caches(void) {
    // 实际代码需根据TC3xx手册配置缓存寄存器
    // 使能DCache
    DCACHE_CTRL |= (1 << 0);
    
    // 使能PCache
    PCACHE_CTRL |= (1 << 0);
}

int main(void) {
    // 初始化缓存（实际项目中通常由启动代码完成）
    // 使能DCache和PCache
    enable_caches();
    
    while (1) {
        process_data();
        
        // 简单延时
        for (volatile uint32_t i = 0; i < 100000; i++);
    }
}
