#include <stdio.h>

typedef enum
{
    MEMIF_UNINIT = 0x00,
    MEMIF_IDLE,
    MEMIF_BUSY,
    MEMIF_BUSY_INTERNAL
} MemIf_StatusType;

MemIf_StatusType MemIf_GetStatus(void)
{
    return MEMIF_IDLE;  
}

int main(void)
{
    MemIf_StatusType status = MEMIF_UNINIT;  
    status = MemIf_GetStatus();

    switch (status)
    {
        case MEMIF_UNINIT:
            printf("Error: 内存接口未初始化！\n");
            break;
        case MEMIF_IDLE:
            printf("内存接口空闲，可发起读写请求。\n");
            printf("-> 执行 MemIf_Write()...\n");
            break;
        case MEMIF_BUSY:
            printf("内存接口繁忙，请稍后重试。\n");
            break;
        case MEMIF_BUSY_INTERNAL:
            printf("内存接口因内部操作繁忙，请稍后重试。\n");
            break;
        default:
            printf("未知状态！\n");
            break;
    }

    return 0;
}
