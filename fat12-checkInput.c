#include <stdio.h>
#include <string.h>
#include "fat12-checkInput.h"

FAT12_Input_t CheckInput(char *input, char *name_tmp)
{
    int i=0;
    char *arr1 = "ls";
    char *arr2 = "ls-f";
    char *arr3 = "cd..";
    char *arr4 = "cd";
    char *arr5 = "mkdir";
    char *arr6 = "cat ";
    
    FAT12_Input_t status = FAT12_ErrorCommand;
    while ( *(input + i) != '\0' )
    {
        //if input == "cd" check if it follow format "cd X" or not
        //if yes then it is command OpenFolder -- "cd X"
        if (status == FAT12_ChangeDir && *(input+i) == 32)
        {
            status = FAT12_OpenFolder;
            break;
        }
        if (status == FAT12_CreateDir && *(input+i) == 32)
        {
            status = FAT12_CreateDir;
            break;
        }
        if (status == FAT12_OpenFile && *(input+i) == 32)
        {
            break;
        }
        //case "ls" -- list       
        if (*(input+i) == *(arr1 + i))
        {
            status = FAT12_List;
        }

        //case "ls-f" -- list full
        else if (*(input+i) == *(arr2 + i))
        {
            status = FAT12_ListFull;
        }

        //case cd.. -- back to previous folder
        else if(*(input+i) == *(arr3 + i) )
        {
            status = FAT12_ChangeDir;
        }

        //case "cd X" -- open X folder
        else if (*(input+i) == *(arr4 + i))
        {
            status = FAT12_ChangeDir;
        }

        //case "mkdir X" -- create a folder named X
        else if (*(input+i)== *(arr5 + i))
        {
            status = FAT12_CreateDir;
        }

        //case "cat X" -- open file X
        else if (*(input+i)== *(arr6 + i))
        {
            status = FAT12_OpenFile;
        }      
        i++;
    }
    i=0;
    while (*(input + i) != '\0' )
    {
        switch (status)
        {
        case FAT12_OpenFolder:
            *(name_tmp + i)  = *(input + i + 3);
            break;
        case FAT12_CreateDir:
            *(name_tmp + i) =  *(input + i + 6);
            break;
        case FAT12_OpenFile:
            *(name_tmp + i) =  *(input + i + 4 );
            break;
        } 
        i++;
    }
    return status;
}



static void combineNameAndExt(file_entry *entry, char *combined) {
    // Xoá các ký tự trắng ở cuối DIR_Name
    int i;
    for (i = 7; i >= 0; i--) {
        if (entry->DIR_Name[i] != ' ') {
            break;
        }
    }
    entry->DIR_Name[i + 1] = '\0';

    // Gộp DIR_Name và DIR_Extens thành một chuỗi duy nhất với dấu "." ngăn cách
    snprintf(combined, 12, "%s.%s", (char *)(entry->DIR_Name), (char *)(entry->DIR_Extens));
}

CheckInput_t FAT12_CheckExist(char *pName, file_entry_info *pFileEntry, uint32_t *cluster) {
    CheckInput_t status;
    if (pFileEntry == NULL) {
        status = INPUT_ENTRY_EMPTY;
    }
    else
    {
        char entryName[12];

        while (pFileEntry != NULL) {
            combineNameAndExt(&(pFileEntry->data), entryName);
            if (strncmp(pName, entryName, strlen(pName)) == 0) {
                
                *cluster = (pFileEntry->data).DIR_FstClus;
                if ((pFileEntry->data).DIR_Extens[0] != ' ') {
                    status = INPUT_FILE_EXIST;  
                } 
                else {
                    status = INPUT_FOLDER_EXIST;
                }
                break;
            }
            else
            {
                status =  INPUT_NOT_FOUND;
            }
            
            pFileEntry = pFileEntry->pNext;
        }
    }
    

    return status;
}
