#include "Bootloader.h"

// 定义地址和状态
#define APP_START_ADDR 0x08008000  // 主程序在Flash中的起始地址
#define BOOT_PIN       GPIO_PIN_0  // 用于触发Bootloader模式的引脚
#define MAX_PACKET_LEN 64          // 最大CAN数据包长度
#define CAN_OK         1
#define BOOT_PIN       1 

// 状态枚举
typedef enum {
    BOOT_IDLE,
    BOOT_WAITING_FOR_AUTH,
    BOOT_RECEIVING_DATA,
    BOOT_VERIFYING,
    BOOT_START_APP
} BootState;

typedef struct {
    uint16 id;
    uint16 len;
    sint32 data[];
} CanMessage;

// 全局变量
BootState current_state = BOOT_IDLE;
uint8 auth_key[4] = {0x12, 0x34, 0x56, 0x77};  // 授权密钥
uint32 flash_write_addr = APP_START_ADDR;       // Flash写入地址

// 函数声明
static void bootloader_init(void);
static void check_boot_condition(void);
static void process_can_message(void);
static void jump_to_application(void);

int main(void) {
    // 1. 初始化最基础硬件（仅初始化启动必需的外设）
    bootloader_init();
    // 2. 检查启动条件（进入Bootloader模式还是直接启动应用）
    check_boot_condition();

    // 3. Bootloader主循环
    while (current_state != BOOT_START_APP) {
        process_can_message();  // 处理CAN总线上的消息
    }

    // 4. 跳转到主程序
    jump_to_application();

    // 正常不会执行到这里
    while (1);
}

// 初始化
void bootloader_init(void)
{
    gpio_init();
    can_init(500000);  // 初始化CAN，波特率500kbps
    flash_init();
}

// 检查启动条件
void check_boot_condition(void) {
    // 条件1：检测到BOOT_PIN引脚被拉低（如外接按钮或诊断仪触发）
    if (gpio_read(BOOT_PIN) == 0) {
        current_state = BOOT_WAITING_FOR_AUTH;
        can_send("Enter Bootloader, waiting for auth", 30);
        return;
    }

    // 条件2：检查主程序是否存在（简单判断首地址是否为有效指令）
    uint32 app_header = *(volatile uint32*)APP_START_ADDR;
    if (app_header != 0xFFFFFFFF) {  // 非全FF表示有程序
        current_state = BOOT_START_APP;
    } else {
        current_state = BOOT_WAITING_FOR_AUTH;  // 无程序，等待升级
        can_send("No application found, enter update mode", 36);
    }
}

// 处理CAN消息
void process_can_message(void) {
    CanMessage msg;
    if (can_receive(&msg) == CAN_OK) {
        switch (current_state) {
            case BOOT_WAITING_FOR_AUTH:
                // 验证授权密钥（简单比对）
                if (msg.id == 0x123 && msg.len == 4) {
                    if (memcmp(msg.data, auth_key, 4) == 0) {
                        current_state = BOOT_RECEIVING_DATA;
                        can_send("Auth success, ready to receive data", 32);
                        eraseUCBSector(APP_START_ADDR);  // 擦除应用区域
                        flash_write_addr = APP_START_ADDR;
                    } else {
                        can_send("Auth failed", 11);
                    }
                }
                break;

            case BOOT_RECEIVING_DATA:
                // 接收程序数据并写入Flash
                if (msg.id == 0x124 && msg.len > 0) {
                    flash_write(flash_write_addr, msg.data, msg.len);
                    flash_write_addr += msg.len;
                    can_send("Data received", 13);  // 发送确认
                }
                // 收到结束指令
                else if (msg.id == 0x125 && msg.data[0] == 0x01) {
                    current_state = BOOT_VERIFYING;
                    can_send("Start verifying data", 19);
                }
                break;

            case BOOT_VERIFYING:
                // 简单校验（实际应使用CRC或哈希）
                can_send("Verify success, ready to start app", 30);
                current_state = BOOT_START_APP;
                break;

            default:
                break;
        }
    }
}

// 跳转到主程序
void jump_to_application(void) {
    // 关闭中断和外设
    can_deinit();
    // 跳转到应用程序（ARM架构典型做法）
    typedef void (*AppFunc)(void);
    AppFunc app_entry = (AppFunc)*(volatile uint32*)(APP_START_ADDR + 4);
    // 设置栈顶指针
    __set_MSP(*(volatile uint32*)APP_START_ADDR);
    // 跳转到应用
    app_entry();
}
