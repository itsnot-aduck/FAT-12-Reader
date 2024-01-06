#include <stdio.h>
#include <string.h>
#include "fat12-parser.h"
#include "fat12-path.h"
#include "fat12-cli-table.h"
#include "fat12-err.h"
#include "fat12-entry.h"

int main(){
    char buffer[20];
    FAT12_read_file("floppy.img");
    FAT12_GetDirectory(0);
    while(1){
        Path_Get_Current();
        fflush(stdin);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strlen(buffer) - 1] = '\0';
        msg_log(cli_handler(buffer));
    }
    return 0;
}