#include <stdint.h>
#include <string.h>

// 假设Flash基地址为0x08000000，扇区大小为4KB
#define FLASH_BASE_ADDR    0x08000000
#define FLASH_SECTOR_SIZE  0x1000  // 4096字节

// Flash控制寄存器地址（模拟，实际需参考芯片手册）
#define FLASH_CR           (*(volatile uint32_t*)0x40022010)
#define FLASH_SR           (*(volatile uint32_t*)0x40022014)

// 控制寄存器位定义
#define FLASH_CR_ERASE     (1 << 1)  // 擦除使能
#define FLASH_CR_PROG      (1 << 0)  // 编程使能
#define FLASH_SR_BSY       (1 << 0)  // 忙状态标志

/**
 * 等待Flash操作完成（轮询忙标志）
 */
static void flash_wait_busy(void) {
    while (FLASH_SR & FLASH_SR_BSY);  // 等待忙标志清零
}

/**
 * 擦除指定扇区
 * @param sector 扇区编号（0开始）
 * @return 0=成功，-1=失败
 */
int flash_erase_sector(uint32_t sector) {
    // 1. 计算扇区起始地址
    uint32_t sector_addr = FLASH_BASE_ADDR + sector * FLASH_SECTOR_SIZE;
    
    // 2. 检查地址合法性
    if (sector_addr >= (FLASH_BASE_ADDR + 0x100000)) {  // 假设总大小为1MB
        return -1;
    }
    
    // 3. 使能擦除操作
    FLASH_CR |= FLASH_CR_ERASE;
    
    // 4. 写入扇区地址触发擦除（实际芯片可能需要特定指令）
    *(volatile uint32_t*)sector_addr = 0x00000000;
    
    // 5. 等待擦除完成
    flash_wait_busy();
    
    // 6. 关闭擦除使能
    FLASH_CR &= ~FLASH_CR_ERASE;
    
    return 0;
}

/**
 * 向Flash写入数据（需先擦除对应扇区）
 * @param addr 相对于Flash基地址的偏移地址
 * @param data 待写入数据指针
 * @param len 数据长度（字节）
 * @return 0=成功，-1=失败
 */
int flash_write(uint32_t addr, const uint8_t* data, uint32_t len) {
    // 1. 计算绝对地址
    uint32_t flash_addr = FLASH_BASE_ADDR + addr;
    
    // 2. 检查地址和长度合法性
    if ((flash_addr + len) > (FLASH_BASE_ADDR + 0x100000)) {
        return -1;
    }
    
    // 3. 使能编程模式
    FLASH_CR |= FLASH_CR_PROG;
    
    // 4. 按字节写入（实际可能要求按半字/字写入）
    for (uint32_t i = 0; i < len; i++) {
        *(volatile uint8_t*)(flash_addr + i) = data[i];
        flash_wait_busy();  // 等待每个字节写入完成
    }
    
    // 5. 关闭编程模式
    FLASH_CR &= ~FLASH_CR_PROG;
    
    return 0;
}

/**
 * 从Flash读取数据
 * @param addr 相对于Flash基地址的偏移地址
 * @param buf 接收数据的缓冲区
 * @param len 读取长度（字节）
 * @return 0=成功，-1=失败
 */
int flash_read(uint32_t addr, uint8_t* buf, uint32_t len) {
    // 1. 计算绝对地址
    uint32_t flash_addr = FLASH_BASE_ADDR + addr;
    
    // 2. 检查地址和长度合法性
    if ((flash_addr + len) > (FLASH_BASE_ADDR + 0x100000)) {
        return -1;
    }
    
    // 3. 直接拷贝数据（Flash可读时可直接访问）
    memcpy(buf, (const void*)flash_addr, len);
    
    return 0;
}

// 使用示例
int main(void) {
    uint8_t write_data[] = "Hello Flash!";
    uint8_t read_data[20] = {0};
    
    // 1. 擦除第0扇区
    if (flash_erase_sector(0) != 0) {
        // 处理错误
    }
    
    // 2. 写入数据到扇区0的偏移0处
    if (flash_write(0, write_data, strlen((char*)write_data) + 1) != 0) {
        // 处理错误
    }
    
    // 3. 从相同位置读取数据
    if (flash_read(0, read_data, sizeof(read_data)) != 0) {
        // 处理错误
    }
    
    // 4. 验证结果（实际应用中可添加校验逻辑）
    
    while (1);
    return 0;
}
