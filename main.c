#include "fat12-parser.h"
#include "fat12-entry.h"

int main(){
    FAT12_read_file("floppy.img");
    FAT12_GetDirectory(6);
    FAT12_Directory_Export();
    // FAT12_Get_File_Content(8);
    FAT12_close_file();
    return 0;
}   