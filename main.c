#include <stdio.h>
#include <string.h>
#include "fat12-parser.h"
#include "fat12-entry.h"
#include "fat12-fattable.h"
#include "fat12-checkInput.h"
#include "fat12-path.h"

// int main(){
//     FAT12_read_file("test_Data.img");
//     FAT12_GetDirectory(0);
//     // printf("Before: %X %X\n", FAT12_Fat_Read(2), FAT12_Fat_Read(3));
//     // FAT12_Fat_Free(3);
//     // printf("After: %X %X\n", FAT12_Fat_Read(2), FAT12_Fat_Read(3));
//     FAT12_Create_Folder("Demo4");
//     FAT12_GetDirectory(18);
//     FAT12_Directory_Export();
//     FAT12_close_file();
//     return 0;
// }   

int main(){
    FAT12_read_file("floppy.img");
    FAT12_Input_t statusInput;
    CheckInput_t statusCheckInput;
    uint32_t currenClus = 0, preClus = 0, fileClus; 
    FAT12_GetDirectory(0);
    // FAT12_GetDirectory(6);
    while (1)
    {
        char CharInput[50];
        char name_tmp[12];
        printf("\n");
        Path_Get_Current();
        fflush(stdin);
        if (scanf("\n %99[^\n]", CharInput) != 1 || strlen(CharInput) == 99) {
            printf("Input error or too many characters.\n");
            break;
        }
        else{
            statusInput = CheckInput(CharInput, name_tmp);
            log("test tmp: %s", name_tmp);
            switch (statusInput)
            {
                case FAT12_List:
                    FAT12_Directory_Export();
                    log("FAT12_List\n");
                    break;
                
                case FAT12_ListFull:
                    FAT12_GetDirectory(currenClus);
                    FAT12_Directory_Export();
                    log("FAT12_ListFull\n");
                    break;

                case FAT12_OpenFolder:
                    log("FAT12_OpenFolder\n");
                    statusCheckInput = FAT12_CheckExist(name_tmp, instance,&currenClus);
                    log("%d\n",statusCheckInput);
                    if (statusCheckInput == INPUT_FOLDER_EXIST)
                    {
                        FAT12_GetDirectory(currenClus);
                        PATH_move_to_child(name_tmp);
                    }
                    break;

                case FAT12_ChangeDir:
                    log("FAT12_ChangeDir\n");
                    break;

                case FAT12_CreateDir:
                    log("FAT12_CreateDir\n");
                    break;
                
                case FAT12_OpenFile:
                    log("FAT12_OpenFile\n");
                    statusCheckInput = FAT12_CheckExist(name_tmp, instance,&fileClus);
                    log("%d\n",statusCheckInput);
                    if (statusCheckInput == INPUT_FILE_EXIST)
                    {
                        FAT12_Get_File_Content(fileClus);
                    }
                    break;
                default:
                    printf("Error Command\n");
                    break;
            }
        }
    }
FAT12_close_file();
return 0;
}