#include <stdio.h>
#include "fat12-parser.h"
#include "fat12-fattable.h"

FILE *fptr; 
FAT12_BS_Stat_t FAT12_BS_Stat = {0};
uint32_t current_Cluster = 0;

void FAT12_BS_Param_Cal(){
    /* Calulate size of each part */
    FAT12_BS_Stat.FatSize = FAT12_BS_Info.BPB_NumFATs * FAT12_BS_Info.BPB_FATSz16;
    FAT12_BS_Stat.RootSize = (32 * FAT12_BS_Info.BPB_RootEntCnt) / FAT12_BS_Info.BPB_BytsPerSec;
    FAT12_BS_Stat.DataSize = FAT12_BS_Info.BPB_TotSec16 - FAT12_BS_Stat.FatSize - FAT12_BS_Stat.RootSize - 1;

    /* Calculate the start address */
    FAT12_BS_Stat.FatAddr = 0x200; //Second sector
    FAT12_BS_Stat.RootAddr = FAT12_BS_Stat.FatAddr + FAT12_BS_Stat.FatSize * FAT12_BS_Info.BPB_BytsPerSec;
    FAT12_BS_Stat.DataAddr = FAT12_BS_Stat.RootAddr + FAT12_BS_Stat.RootSize * FAT12_BS_Info.BPB_BytsPerSec;
    FAT12_BS_Stat.ClusSize = FAT12_BS_Info.BPB_BytsPerSec * FAT12_BS_Info.BPB_SecPerClus;
    log("FatSize: %d, RootSize: %d, DataSize: %d", FAT12_BS_Stat.FatSize, FAT12_BS_Stat.RootSize, FAT12_BS_Stat.DataSize);
    log("FatAddr: 0x%X, RootAddr: 0x%X, DataAddr: 0x%X", FAT12_BS_Stat.FatAddr, FAT12_BS_Stat.RootAddr, FAT12_BS_Stat.DataAddr);
}

static FAT12_Status_t FAT12_BS_Read(){
    FAT12_Status_t status = FAT12_FILE_OPEN_FAILED;   
    if (fptr != NULL){
        status = FAT12_READ_INVALID_BS;
        /* Check for valid Boot Sector by signature */
        fseek(fptr, 510, SEEK_SET);
        if (fgetc(fptr) == 0x55 && fgetc(fptr) == 0xAA){
            fseek(fptr, 0x03, SEEK_SET);
            fread(&FAT12_BS_Info, sizeof(FAT12_BS_t), 1, fptr);
            FAT12_BS_Param_Cal();
            status = FAT12_SUCCESS;
        } 
    }
    return status;
}

FAT12_Status_t FAT12_read_file(char* file){
    FAT12_Status_t status = READ_FAILED;
    fptr = fopen(file, "rb+");
    if (fptr != NULL){
        status = FAT12_SUCCESS;
    }
    log("open %s with status %d", status ? "Failed" : "Success", status);
    log("Read BS stat of FAT12 File: ");
    status = FAT12_BS_Read();
    return status;
}

void FAT12_close_file(){
    if (fptr != NULL){
        fclose(fptr);       
    }
}


