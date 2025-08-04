#include <stdio.h>
#include "MemIf.h"

MemIf_StatusType MemIf_GetStatus(void)
{
    return MEMIF_BUSY;  
}

int main(void)
{
    MemIf_StatusType status = MEMIF_UNINIT;  
    status = MemIf_GetStatus();

    switch (status)
    {
        case MEMIF_UNINIT:
            printf("Error: MemIf no init\n");
            break;
        case MEMIF_IDLE:
            printf("MemIf idle\n");
            printf("MemIf_Write()...\n");
            break;
        case MEMIF_BUSY:
            printf("MemIf busy, try later\n");
            break;
        case MEMIF_BUSY_INTERNAL:
            printf("MemIf busy internal, try later\n");
            break;
        default:
            printf("unknown state\n");
            break;
    }

    return 0;
}
