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