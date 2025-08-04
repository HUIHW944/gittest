#ifndef MEMIF_H
#define MEMIF_H

typedef enum
{
    MEMIF_UNINIT = 0x00,
    MEMIF_IDLE,
    MEMIF_BUSY,
    MEMIF_BUSY_INTERNAL
} MemIf_StatusType;

#endif /* MEMIF_H */ 