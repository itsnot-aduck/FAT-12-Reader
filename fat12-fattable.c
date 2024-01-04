#include <stdio.h>
#include <stdint.h>
#include "fat12-parser.h"

uint16_t FAT12_Fat_Read(int num){
    fseek(fptr, 0x200 + num/2*3, SEEK_SET);
    uint16_t result;
    uint8_t EleArray[3];
    fread(EleArray, 1, 3, fptr);
    log("Read: %d %d %d\n", EleArray[0], EleArray[1], EleArray[2]);
    if (num % 2 == 0){
        result = ((EleArray[1] & 0x0F) << 8) | EleArray[0];
    }
    else {
        result = EleArray[2] << 4 | EleArray[1] >> 4;
    }
    log("FAT Ele: %X\n", result);
    return result;
}
/* Set Available value for cluster */
void FAT12_Fat_Free(int num){
    fseek(fptr, 0x200 + num/2*3, SEEK_SET);
    uint16_t result;
    uint8_t EleArray[3];
    fread(EleArray, 1, 3, fptr);
    log("Read: %d %d %d\n", EleArray[0], EleArray[1], EleArray[2]);
    if ( num % 2 == 0){
        EleArray[0] = 0x00;
        EleArray[1] &= 0xF0; 
    }
    else {
        EleArray[2] = 0x00;
        EleArray[1] &= 0xF;
    }                                                                                                                        
    /* Back cursor */
    fseek(fptr, 0x200 + num/2*3, SEEK_SET);                                                                                                                                                                                                  
    for (int i = 0; i < 3; i++){
        fputc(EleArray[i], fptr);
    }
}

void FAT12_Fat_Set_Full(int num){
    fseek(fptr, 0x200 + num/2*3, SEEK_SET);
    uint16_t result;
    uint8_t EleArray[3];
    fread(EleArray, 1, 3, fptr);
    log("Read: %d %d %d\n", EleArray[0], EleArray[1], EleArray[2]);
    if ( num % 2 == 0){
        EleArray[0] = 0xFF;
        EleArray[1] = (EleArray[1] & 0xF0) + 0x0F; 
    }
    else {
        EleArray[2] = 0xFF;
        EleArray[1] = (EleArray[1] & 0xF) + 0xF0;
    }                                                                                                                        
    /* Back cursor */
    fseek(fptr, 0x200 + num/2*3, SEEK_SET);                                                                                                                                                                                                  
    for (int i = 0; i < 3; i++){
        fputc(EleArray[i], fptr);
    }
}

uint16_t FAT12_Get_First_Available(){
    int position = -2;
    for (int i = 2; i < FAT12_BS_Stat.DataAddr; i++){
        if(FAT12_Fat_Read(i) == 0x00){
            position = i;
            break;
        }
    }
    return position;
}