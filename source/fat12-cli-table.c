#include <stdio.h>
#include <string.h>
#include "fat12-cli-table.h"
#include "fat12-parser.h"
#include "fat12-entry.h"
#include "fat12-path.h"

extern void FAT12_linked_list_remove();

const cli_cmd_info_t cli_change_dir =
{
    change_dir,
    "<dir> \t Change Directory"
};

const cli_cmd_info_t cli_cat_file = 
{
    cat_file,
    "<folder> \t Read contents of file"
};

const cli_cmd_info_t cli_make_dir = 
{
    make_dir,
    "<dir_name> \t Make a new directory"
};

const cli_cmd_info_t cli_remove = 
{
    remove_item,
    "<item_name> \t Remove a item"
};

const cli_cmd_info_t cli_unmount = {
    unmount,
    "<no arg> \t Close file and exit program"
};

const cli_cmd_info_t cli_list = {
    list_item,
    "<no arg> \t List all item"
};

const cli_cmd_info_t cli_help = {
    list_help,
    " "
};

cli_cmd_entry_t command_entry_table[CLI_TABLE_ENTRY] = 
{
    {"cd",&cli_change_dir},
    {"cat",&cli_cat_file},
    {"mkdir",&cli_make_dir},
    {"rm", &cli_remove},
    {"unmount",&cli_unmount},
    {"ls", &cli_list},
    {"help", &cli_help}
};

FAT12_Status_t change_dir(uint8_t argc, char **argv){
    uint8_t status = FAT12_SUCCESS;
    log("num of param %d", argc);
    switch(argc){
        case 1:
            log("change to root");
            FAT12_GetDirectory(0);
            PATH_back_to_root();
            /* Function to move to root */
            break;
        case 2:
            log("Go to %s", argv[1]);
            status = FAT12_CLI_MUST_DIR;
            if (strstr(argv[1],"..")){
                log("back to father");
                if(PATH_Get_Depth()){
                    uint32_t st_cluster = instance->pNext->data.DIR_FstClus;
                    FAT12_GetDirectory(st_cluster);
                }
                PATH_back_to_father();
                status = FAT12_SUCCESS;
            }
            else if (strstr(argv[1], ".") ==0){
                status = FAT12_DIR_NOT_FOUND;
                file_entry_info *pCursor = instance;
                for(int i = 0; i < size; i++){
                    if ((strstr(pCursor->data.DIR_Name, argv[1])) 
                        && pCursor ->data.DIR_Attr == FILE_ATTR_DIR){
                        log("Dir found. Change to new cluster %X", pCursor->data.DIR_FstClus);
                        FAT12_GetDirectory(pCursor->data.DIR_FstClus);
                        PATH_move_to_child(argv[1]);
                        status = FAT12_SUCCESS;
                        break;
                    }
                    pCursor = pCursor -> pNext; 
                }
            }
            break;
        default:  
        {
            status = FAT12_CLI_INVALID_CMD;
            log("Too much argument");
        } 
    }
    return status;
}

FAT12_Status_t cat_file(int8_t argc, char **argv){
    FAT12_Status_t status = FAT12_SUCCESS;
    switch(argc){
        case 1:
            log("No file to cat");
            break;
        case 2:
            status = FAT12_CLI_MUST_FILE;
            char *filename = strtok(argv[1], ".");
            char *ext = strtok(NULL, ".");
            if (ext != NULL){
                status = FAT12_FILE_NOT_FOUND;
                file_entry_info *pCursor = instance;
                for(int i = 0; i < size; i++){
                    if ((strstr(pCursor->data.DIR_Name, filename)) && strstr(pCursor ->data.DIR_Extens, ext)){
                        log("file found");
                        FAT12_Get_File_Content(pCursor->data.DIR_FstClus);
                        status = FAT12_SUCCESS;
                        break;
                    }
                    pCursor = pCursor -> pNext; 
                }
            }
            break;
        default:  
        {
            status = FAT12_CLI_INVALID_CMD;
            log("Too much argument");
        } 
    }   
    return status;
}


FAT12_Status_t make_dir(int8_t argc, char **argv){
    FAT12_Status_t status = FAT12_SUCCESS;
    switch(argc){
        case 1:
            log("No dir to make");
            break;
        case 2:
            log("Make directory %s", argv[1]);
            status = FAT12_CLI_MUST_DIR;
            if (strstr(argv[1],".") == 0){
                status = FAT12_SUCCESS;
                uint32_t first_address;
                uint8_t found = 0; /* mark the first "deleted" address marked */
                file_entry_info *pCursor = instance;
                for(int i = 0; i < size; i++){
                    if(pCursor->data.DIR_Attr == FILE_ATTR_DIR
                        && strstr(pCursor->data.DIR_Name, argv[1])
                        && pCursor->data.DIR_Name[0] != 0xE5) 
                    {
                            log("Folder existed");
                            status = FAT12_DIR_EXISTED;
                            break;
                    }
                    pCursor = pCursor -> pNext; 
                    }
                }
                if(status != FAT12_DIR_EXISTED){
                    FAT12_Create_Folder(argv[1]);
                }
            break;
        default:  
        {
            status = FAT12_CLI_INVALID_CMD;
            log("Too much argument");
        } 
    }   
    return status;
}

FAT12_Status_t remove_item(int8_t argc, char **argv){
    FAT12_Status_t status = FAT12_SUCCESS;
    switch(argc){
        case 1:
            log("No item to remove");
            break;
        case 2:
            status = FAT12_ITEM_NOT_FOUND;
            log("Remove item %s", argv[1]);
            char *filename = strtok(argv[1], ".");
            char *ext = strtok(NULL, ".");
            uint8_t itemCategory = (ext != NULL) ? FILE_ATTR_TXT : FILE_ATTR_DIR;
            log("Is file 0x%X", itemCategory);
            /* find for item */
            uint32_t address;
            uint8_t found = 0;
            file_entry_info *pCursor = instance;
            for(int i = 0; i < size; i++){
                if(strstr(pCursor->data.DIR_Name, argv[1])){
                    switch(itemCategory){
                        case FILE_ATTR_DIR:
                            if (pCursor->data.DIR_Attr == FILE_ATTR_DIR){
                                FAT12_Remove_Item(pCursor);
                                found = 1;
                            }
                        break;
                        case FILE_ATTR_TXT:
                            if (pCursor->data.DIR_Attr == FILE_ATTR_TXT
                                && strstr(pCursor ->data.DIR_Extens, ext))
                            {
                                FAT12_Remove_Item(pCursor);
                                found = 1;
                            }
                        break;
                    }
                    if (found == 1){
                        status = FAT12_SUCCESS;
                        break;

                    }
                }   
                pCursor = pCursor -> pNext; 
            }
            break;
        default:  
        {
            status = FAT12_CLI_INVALID_CMD;
            log("Too much argument");
        } 
    }   
    return status;
}

FAT12_Status_t list_item(int8_t argc, char **argv){
    FAT12_Status_t status = FAT12_CLI_INVALID_CMD;
    if (argc == 1){
        FAT12_Directory_Export();
        status = FAT12_SUCCESS;
    }
    return status;
}

FAT12_Status_t unmount(int8_t argc, char **argv){
    log("Exit the program");
    FAT12_linked_list_remove();
    FAT12_close_file();
    exit(0);
}

FAT12_Status_t list_help(int8_t argc, char **argv){
    for(int i = 0; i < CLI_TABLE_ENTRY - 1; i++){
        printf("%s - %s\n", command_entry_table[i].name, command_entry_table[i].cmd_info->help);
    }
    return 0;
}

FAT12_Status_t cli_handler(char *input){
    FAT12_Status_t status = FAT12_CLI_UNFOUND_CMD;
    char *cur;
    char *argv[5];
    int count = 0;
    cur = strtok(input, " ");
    while(cur != NULL){
        argv[count] = cur;
        cur = strtok(NULL, " ");
        count++;
    }
    log("Cmd arg count: %d", count);
    for(int i = 0; i< CLI_TABLE_ENTRY; i++){
        if (strstr(argv[0],command_entry_table[i].name)){
            status = command_entry_table[i].cmd_info->function(count, argv);
            break;
        }
    }
    log("Cmd state: %d", status);
    return status;
}