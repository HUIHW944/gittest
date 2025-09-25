/*******************************************************************************
 * Copyright(c) 2023 Jiangsu Soarwhale Technology CO.,LTD. All rights reserved
 *
 * @file   : Bootloader_dual_bank.c
 *
 * @date   : Sep 11, 2025
 * @author : huihui.wang
 *
 ******************************************************************************/

/*=== INCLUDE SECTOR ========================================================*/

// #include "IfxFlash.h"
// #include "IfxScuWdt.h"
// #include "can.h"
// #include "uds.h"
#include "Platform_Types.h"

/*=== MACRO DEFINITION SECTOR ===============================================*/

#define UDS_OK       1

// TC3xx PFlash Bank地址定义（以TC375为例）
#define BANK_A_START 0x80000000UL  // Bank A起始地址
#define BANK_B_START 0x80300000UL  // Bank B起始地址（与A间隔3MB）
#define BANK_SIZE    0x00300000UL  // 单个Bank大小（3MB）

// 启动标志位存储地址（DFlash的UCB区域）
#define BOOT_FLAG_ADDR 0xAF001000UL
#define BOOT_FROM_A    0xAA55AA55UL  // 从Bank A启动标志
#define BOOT_FROM_B    0x55AA55AAUL  // 从Bank B启动标志

/*=== DATA TYPE DECLARATION SECTOR ==========================================*/

typedef enum {
    IfxFlash_Status_not_ok,
    IfxFlash_Status_ok,
    IfxFlash_Status_invalidAddress
} IfxFlash_Status;

typedef struct {
    uint16 service;
    uint16 len;
    sint32 data[];
} UdsMessage;

/*=== LOCAL FUCTION DECLARATION SECTOR ======================================*/



/*=== VARIABLE DEFINITION SECTOR ============================================*/

// 当前运行的Bank（Bootloader初始化时判断）
static uint32 current_bank = 0;  // 0=A, 1=B
static uint32 target_bank = 0;   // 待升级的目标Bank

/*=== FUNCTION DEFINITION SECTOR ============================================*/

// 初始化：判断当前启动的Bank
void bootloader_init(void) {
    // 读取启动标志位
    uint32 boot_flag = *(volatile uint32*)BOOT_FLAG_ADDR;
    current_bank = (boot_flag == BOOT_FROM_B) ? 1 : 0;
    target_bank = (current_bank == 0) ? 1 : 0;  // 目标Bank为当前Bank的对立面
    
    // 初始化Flash控制器和CAN
    IfxFlash_initModule(&MODULE_FLASH0);
    can_init(500000);
}

// 擦除目标Bank（先解锁，再按扇区擦除）
IfxFlash_Status erase_target_bank(void) {
    IfxFlash_Status status;
    uint32 bank_start = (target_bank == 0) ? BANK_A_START : BANK_B_START;
    
    // 解锁Flash（TC3xx需要安全解锁序列）
    IfxScuWdt_clearSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
    status = IfxFlash_unlockSector(bank_start, bank_start + BANK_SIZE - 1);
    IfxScuWdt_setSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
    
    if (status != IfxFlash_Status_ok) {
        return status;
    }
    
    // 擦除整个目标Bank（按物理扇区逐个擦除）
    for (uint32 addr = bank_start; addr < bank_start + BANK_SIZE; addr += 0x10000) {  // 假设扇区大小16KB
        IfxScuWdt_clearSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
        status = IfxFlash_eraseSector(addr);  // TC3xx擦除函数
        IfxScuWdt_setSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
        
        if (status != IfxFlash_Status_ok) {
            return status;
        }
    }
    
    return IfxFlash_Status_ok;
}

// 写入数据到目标Bank（分块写入）
IfxFlash_Status write_to_target_bank(uint32 offset, uint8* data, uint32 len) {
    IfxFlash_Status status;
    uint32 bank_start = (target_bank == 0) ? BANK_A_START : BANK_B_START;
    uint32 write_addr = bank_start + offset;  // 目标地址=Bank起始+偏移量
    
    // 写入前校验地址是否在目标Bank范围内
    if (write_addr + len > bank_start + BANK_SIZE) {
        return IfxFlash_Status_invalidAddress;
    }
    
    // 写入数据（TC3xx最小写入单位为32字节，需对齐）
    IfxScuWdt_clearSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
    status = IfxFlash_write(write_addr, data, len, IfxFlash_ProgramMode_32bit);
    IfxScuWdt_setSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
    
    return status;
}

// 升级完成后切换启动Bank
void switch_boot_bank(void) {
    uint32 new_boot_flag = (target_bank == 0) ? BOOT_FROM_A : BOOT_FROM_B;
    
    // 写入新的启动标志位（需先解锁DFlash）
    IfxScuWdt_clearSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
    IfxFlash_unlockSector(BOOT_FLAG_ADDR, BOOT_FLAG_ADDR);
    IfxFlash_write(BOOT_FLAG_ADDR, (uint8*)&new_boot_flag, 4, IfxFlash_ProgramMode_32bit);
    IfxFlash_lockSector(BOOT_FLAG_ADDR, BOOT_FLAG_ADDR);
    IfxScuWdt_setSafetyEndinitInline(&MODULE_SCU.WDTCPU[0]);
}

// 主升级流程
void bootloader_main(void) {
    bootloader_init();
    uds_response(0x74, "Enter dual-bank update mode");  // 响应UDS指令
    
    // 1. 擦除目标Bank
    if (erase_target_bank() != IfxFlash_Status_ok) {
        uds_response(0x7F, "Erase target bank failed");
        return;
    }
    
    // 2. 接收并写入新程序（通过CAN接收数据块）
    uint32 offset = 0;
    while (1) {
        UdsMessage msg;
        if (uds_receive(&msg) == UDS_OK) {
            if (msg.service == 0x36) {  // UDS传输数据服务
                if (write_to_target_bank(offset, msg.data, msg.len) != IfxFlash_Status_ok) {
                    uds_response(0x7F, "Write data failed");
                    return;
                }
                offset += msg.len;
                uds_response(0x76, "Data received");  // 确认接收
            }
            else if (msg.service == 0x37) {  // 传输结束服务
                uds_response(0x77, "Transfer completed");
                break;
            }
        }
    }
    
    // 3. 验证目标Bank数据（简化为CRC校验）
    if (verify_bank_crc(target_bank) != 0) {  // 假设verify_bank_crc是自定义校验函数
        uds_response(0x7F, "Verify failed");
        return;
    }
    
    // 4. 切换启动Bank并重启
    switch_boot_bank();
    uds_response(0x71, "Update success, rebooting");
    reset_mcu();  // 重启MCU，从新Bank启动
}




/**** END OF FILE *************************************************************/
