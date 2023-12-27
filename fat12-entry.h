#ifndef __FAT12_ENTRY_H__
#define __FAT12_ENTRY_H__

#include <stdint.h>
#include <stdio.h>


typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

#define FILE_ATTR_TXT 0x00
#define FILE_ATTR_RO 0x01
#define FILE_ATTR_HIDDEN 0x02
#define FILE_ATTR_SYSTEM 0x04
#define FILE_ATTR_VOLLAB 0x08
#define FILE_ATTR_DIR 0x10
#define FILE_ATTR_ARCH 0x20
#define FILE_ATTR_DEVICE 0x40
#define FILE_ATTR_UNUSED 0x80
#define FILE_ATTR_LSN 0x0F

#define FILE_ROOT_ADDR 0x2600

typedef struct file_entry {
    BYTE    DIR_Name[8];
    BYTE    DIR_Extens[3];
    BYTE    DIR_Attr;
    BYTE    DIR_Reserved;
    BYTE    Create_time_ms;
    WORD    DIR_Cre_Time;
    WORD    DIR_Cre_Date;
    WORD    DIR_Last_Acc;
    WORD    EA_Index;
    WORD    DIR_Modi_Time;
    WORD    DIR_Modi_Date;
    WORD    DIR_FstClus;
    DWORD   DIR_FileSize;
} __attribute__((packed)) file_entry; 


/* Tao struct phu luu thong tin cua cac node (khong luu het entry) */

typedef struct node{
    file_entry data;
    struct node *pNext;
} file_entry_info;

extern file_entry_info *instance;

void FAT12_GetDirectory(int cluster);
void FAT12_Directory_Export();

#endif
/* __FAT12_ENTRY_H__ */