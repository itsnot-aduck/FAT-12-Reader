#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Macros */
#pragma pack (1)

#define DEBUG_ENABLE

#ifdef DEBUG_ENABLE
    #define DEBUG 1
#else
    #define DEBUG 0
#endif

#define LOG(s, ...) if (DEBUG) printf(s, __VA_ARGS__)
/* Const variable */

#define data_sector_addr 0x4800
#define fat_sector_addr 0x200

typedef enum{
    FAT12_SUCCESS,
    FAT12_COUNTERFEIT_FILE,
} FAT12_Status_t;

/* Private file declarations */


/* Private file definitons */
FILE *fptr;

/**
 * @brief Read FAT Entry Value in FAT Table
*/
uint16_t FAT12_fat_read(int num){
    fseek(fptr, fat_sector_addr + num/2*3, SEEK_SET);
    uint16_t result;
    uint8_t EleArray[3];
    fread(EleArray, 1, 3, fptr);
    LOG("%d %d %d\n", EleArray[0], EleArray[1], EleArray[2]);
    if (num % 2 == 0){
        result = ((EleArray[1] & 0x0F) << 8) | EleArray[0];
    }
    else {
        result = EleArray[2] << 4 | EleArray[1] >> 4;
    }
    LOG("[Debug: FAT Read] Element %d: 0x%X\n", num, result);
    return result;
}

/**
 * @brief Read file in given first cluster 
*/
FAT12_Status_t ReadFileContent(int cluster){
    FAT12_Status_t status = FAT12_COUNTERFEIT_FILE;
    if (cluster >= 2){
        status = FAT12_SUCCESS;
        //(Cluster - 2) + 19 + RootDirectorySector*512)
        int clusterAddr = ((cluster - 2) + 19 + 14) * 512;
        LOG("[Debug: Read File] Find content in address 0x%X\n", clusterAddr);
        fseek(fptr, clusterAddr ,SEEK_SET);
        char *buffer = (char*) malloc(513);
        if(buffer != NULL){
            fread(buffer, 1, 512, fptr);
            printf("%s", buffer);
            // LOG("Sizeof buffer: %d\n", strlen(buffer));
            free(buffer);
        }      
        int next_cluster = FAT12_fat_read(cluster);
        if(next_cluster != 0xFFF){
            LOG("[Debug: Read File] Get entry on FAT %d\n", next_cluster);
            status = ReadFileContent(next_cluster);
        }        
    }
    return status;
}

/* Main File */
int main(){
    fptr = fopen("test_Data.img", "r");
    FAT12_Status_t status = ReadFileContent(2);
    // FAT12_fat_read(0);
    // for (int i = 0; i < 10; i++){
    //     FAT12_fat_read(i);
    // }
    fclose(fptr);
    return 0;
}