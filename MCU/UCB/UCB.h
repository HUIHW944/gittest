/*******************************************************************************
 * @file   : UCB.h
 *
 * @date   : Sep 11, 2025
 * @author : huihui.wang
 *
 ******************************************************************************/

#ifndef UCB_H
#define UCB_H

/*=== INCLUDE SECTOR ========================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Platform_Types.h"

/*=== MACRO DEFINITION SECTOR ===============================================*/



/*=== DATA TYPE DECLARATION SECTOR ==========================================*/

typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR_TIMEOUT,
    FLASH_ERROR_OPERATION,
    FLASH_ERROR_VERIFY
} Flash_ErrorType;

/*=== INTERFACE DECLARATION SECTOR ==========================================*/


Flash_ErrorType eraseUCBSector(uint32 ucb_address);





#endif /* UCB_H */