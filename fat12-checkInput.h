#ifndef __FAT12_CHECKINPUT_H__
#define __FAT12_CHECKINPUT_H__
#include "fat12-entry.h"

typedef enum {
    FAT12_List,                 //"ls"   //0
    FAT12_ListFull,             //"ls-f" //1
    FAT12_OpenFolder,           //"cd "  //2
    FAT12_ChangeDir,            //"cd.." //3
    FAT12_CreateDir,            //"mkdir "//4
    FAT12_OpenFile,             //"cat "  //5
    FAT12_ErrorCommand,                   //6
}FAT12_Input_t;

typedef enum
{
    INPUT_ENTRY_EMPTY,
    INPUT_FILE_EXIST,
    INPUT_FOLDER_EXIST,
    INPUT_NOT_FOUND
}CheckInput_t;

#ifndef _FILE_ENTRY_INFO
#define _FILE_ENTRY_INFO

#endif
FAT12_Input_t CheckInput(char *input, char *name_tmp);
CheckInput_t FAT12_CheckExist(char *pName, file_entry_info *pFileEntry,uint32_t *cluster);

#endif 
/* __FAT12_CHECKINPUT_H__ */