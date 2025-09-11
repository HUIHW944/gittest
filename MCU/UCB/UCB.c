// UCB0: 0xAF000000 ~ 0xAF000FFF (4KB)
// UCB1: 0xAF001000 ~ 0xAF001FFF (4KB)
#include "UCB.h"

#define FLASH_BASE      0xF0000000
#define UCB_CMD_REG     (*(volatile uint32*)(FLASH_BASE + 0x100))
#define UCB_ADDR_REG    (*(volatile uint32*)(FLASH_BASE + 0x104))
#define UCB_STATUS_REG  (*(volatile uint32*)(FLASH_BASE + 0x108))


Flash_ErrorType eraseUCBSector(uint32 ucb_address) {
    uint32 timeout_counter = 0;
    const uint32 MAX_TIMEOUT = 1000000;
    
    // 步骤1：检查 Flash 状态
    while ((UCB_STATUS_REG & 0x01) == 0) {      // Busy bit = 1 表示忙
        if (timeout_counter++ > MAX_TIMEOUT) {
            return FLASH_ERROR_TIMEOUT;
        }
    }
    
    // 步骤2：发送解锁序列
    UCB_CMD_REG = 0x55; // 01010101写入0x55到命令寄存器,第一个解锁码     
    UCB_CMD_REG = 0xAA; // 10101010写入0xAA到命令寄存器,第二个解锁码,与0x55互补
    UCB_CMD_REG = 0x80; // 10000000写入0x80,擦除准备      
    UCB_CMD_REG = 0xAA; // 10101010再次写入0xAA,再次确认            
    UCB_CMD_REG = 0x55; // 01010101写入0x55,完成解锁序列               

    // 步骤3：设置地址,设置要擦除的 UCB 地址（必须是扇区对齐）
    UCB_ADDR_REG = ucb_address; // UCB0起始地址
    
    // 步骤4：执行擦除
    UCB_CMD_REG = 0x30; // 00110000,扇区擦除命令码
    
    // 步骤5：等待完成
    timeout_counter = 0;
    while ((UCB_STATUS_REG & 0x01) == 0) {
        // 可添加超时检测
        if (timeout_counter++ > MAX_TIMEOUT) {
            return FLASH_ERROR_TIMEOUT;
        }
    }
    
    // 步骤6：检查错误标志
    if (UCB_STATUS_REG & 0x02) {
        return FLASH_ERROR_OPERATION;
    }
    
    // 步骤7：验证结果,读取擦除后的数据应该全为0xFF
    uint32* verify_ptr = (uint32*)ucb_address;
    for (int i = 0; i < 1024; i++) {
        if (*verify_ptr != 0xFFFFFFFF) {
            return FLASH_ERROR_VERIFY;
        }
        verify_ptr++;
    }
    
    return FLASH_OK;
}