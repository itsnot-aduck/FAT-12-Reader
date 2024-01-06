#ifndef __FAT12_ERR_C__
#define __FAT12_ERR_C__

#include <stdio.h>
#include "fat12-err.h"

char* message[11] = {
    "",                                     //FAT12_SUCCESS
    "ERR: Full storage\n",                  //FAT12_FULL_STORAGE
    "ERR: Cmd not found\n",                 //FAT12_CLI_UNFOUND_CMD
    "ERR: Wrong number of agrs\n",          //FAT12_CLI_INVALID_CMD
    "ERR: Cannot do with Directory\n",      //FAT12_CLI_MUST_FILE
    "ERR: Cannot do with File\n",           //FAT12_CLI_MUST_DIR
    "ERR: Directory not found\n",           //FAT12_DIR_NOT_FOUND
    "ERR: File not found\n",                 //FAT12_FILE_NOT_FOUND
    "ERR: Directory existed\n",             //FAT12_DIR_EXISTED
    "ERR: File existed\n",                   //FAT12_FILE_EXISTED
    "ERR: No such file or directory\n",
};

void msg_log(uint8_t msg){
    printf("%s", message[msg]);
}

#endif
/* __FAT12_ERR_C__ */