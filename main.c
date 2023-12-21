#include <stdio.h>
#include <stdint.h>

#define DEBUG_ENABLE 0
#define DEBUG(s, ...) if (DEBUG_ENABLE) printf(s, __VA_ARGS__)

FILE *fptr;

uint16_t fat_read(int num){
    fseek(fptr, num*2, SEEK_SET);
    uint16_t result;
    uint8_t EleArray[2];
    fread(EleArray, 1, 2, fptr);
    DEBUG("[Debug:] %d %d\n", EleArray[0], EleArray[1]);
    if (num % 2 == 0){
        result = EleArray[1] & 0x0F << 8 | EleArray[0];
    }
    else {
        result = EleArray[1] << 4 | EleArray[0] >> 4;
    }
    printf("Element %d: 0x%X\n", num, result);
    return result;
}



int main(){
    fptr = fopen("sample.img", "r");
    for(int i = 0; i<=7; i++){
        fat_read(i);
    }
    fclose(fptr);
    return 0;
}