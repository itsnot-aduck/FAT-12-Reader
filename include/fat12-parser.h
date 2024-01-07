#ifndef __FAT_12_PARSER_H__
#define __FAT_12_PARSER_H__

#include <stdio.h>
#include "fat12-config.h"
#include <stdlib.h>
#include <stdint.h>
#include "fat12-err.h"

/* For logging */
#ifdef ENABLE_DEBUG
    #define LOG 1
#else
    #define LOG 0
#endif

#define log(...) {  \
    do {    \
        if ((LOG)) { \
            printf("\n[%s:%s:%d] ", __FILE__, __func__, __LINE__); \
            printf(__VA_ARGS__); \
            printf("\n"); \
        } \
    } while (0); \
} // Logging


/* Public variable */

extern uint32_t current_Cluster;

/* Enum of Bootsector segment information */
#pragma pack(1)
typedef struct {
	uint8_t		BS_OEMName[8];
	uint16_t 	BPB_BytsPerSec;
    uint8_t 	BPB_SecPerClus;
    uint16_t 	BPB_RsvdSecCnt;
    uint8_t 	BPB_NumFATs;
    uint16_t 	BPB_RootEntCnt;
    uint16_t 	BPB_TotSec16;
    uint8_t 	BPB_Media;
    uint16_t 	BPB_FATSz16;
    uint16_t 	BPB_SecPerTrk;
    uint16_t 	BPB_NumHeads;
    uint32_t 	BPB_HiddSec;
    uint32_t 	BPB_TotSec32;
    uint8_t 	BS_DrvNum;
    uint8_t 	BS_Reserved;
    uint8_t 	BS_BootSig;
    uint32_t 	BS_VolID; 
	uint8_t		BS_VolLab[11];
	uint8_t		BS_FileSysType[8];  
} FAT12_BS_t;
#pragma pack()


/* Info (by sectors) */
typedef struct info{
    uint32_t FatSize;
    uint32_t RootSize;
    uint32_t DataSize;
    uint32_t ClusSize;
    /* Address */
    uint32_t FatAddr;
    uint32_t RootAddr;
    uint32_t DataAddr;
} FAT12_BS_Stat_t;

static FAT12_BS_t FAT12_BS_Info;

/* External variable */
extern FAT12_BS_Stat_t FAT12_BS_Stat;
extern FILE* fptr;


/* Function Protototype */
FAT12_Status_t FAT12_read_file(char* file);
void FAT12_close_file();


#endif 
/* __FAT_12_PARSER_H */