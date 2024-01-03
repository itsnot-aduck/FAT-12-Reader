#include "fat12-parser.h"
#include "fat12-entry.h"
#include "fat12-fattable.h"

int main(){
    FAT12_read_file("test_Data.img");
    // FAT12_GetDirectory(6);
    printf("Before: %X %X\n", FAT12_Fat_Read(2), FAT12_Fat_Read(3));
    FAT12_Fat_Free(2);
    printf("After: %X %X\n", FAT12_Fat_Read(2), FAT12_Fat_Read(3));
    // FAT12_Directory_Export();
    // FAT12_Get_File_Content(8);
    FAT12_close_file();
    return 0;
}   