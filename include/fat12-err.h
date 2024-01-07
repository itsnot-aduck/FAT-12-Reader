#ifndef __FAT12_ERR_H__
#define __FAT12_ERR_H__

#include <stdint.h>
#include <stdio.h>
#include "fat12-err.h"

typedef enum{
    FAT12_SUCCESS,
    FAT12_FULL_STORAGE,
    FAT12_CLI_UNFOUND_CMD,
    FAT12_CLI_INVALID_CMD,
    FAT12_CLI_MUST_FILE,
    FAT12_CLI_MUST_DIR,
    FAT12_DIR_NOT_FOUND,
    FAT12_FILE_NOT_FOUND,
    FAT12_DIR_EXISTED,
    FAT12_FILE_EXISTED,
    FAT12_ITEM_NOT_FOUND,
    FAT12_FILE_OPEN_FAILED,
    FAT12_READ_INVALID_BS,
    FAT12_COUNTERFEIT_FILE,
    READ_FAILED
} FAT12_Status_t;

void msg_log(uint8_t msg);

#endif
/* __FAT12_ERR_H__ */