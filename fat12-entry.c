#include <stdio.h>
#include <stdlib.h>
#include "fat12-parser.h"
#include "fat12-entry.h"
#include <string.h>
#include <time.h>

#define ROOT_DIRECTORY_OFFSET 0x00002600
/* Linked list information of node */
// typedef struct node{
//     file_entry data;
//     struct node *pNext;
// } file_entry_info;

/* Initialize the instance of linked list*/
file_entry_info *instance = NULL;
static int size = 0;

typedef struct my_time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} my_time;

void getCurrentTime(my_time* current) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    current->year = tm_info->tm_year + 1900;
    current->month = tm_info->tm_mon + 1;
    current->day = tm_info->tm_mday;
    current->hour = tm_info->tm_hour;
    current->minute = tm_info->tm_min;
    current->second = tm_info->tm_sec;
}

void printFileAttribute(BYTE attribute) {
    switch (attribute) {
        case FILE_ATTR_TXT:
            printf("File");
            break;
        case FILE_ATTR_RO:
            printf("Read-only");
            break;
        case FILE_ATTR_HIDDEN:
            printf("Hidden");
            break;
        case FILE_ATTR_SYSTEM:
            printf("System");
            break;
        case FILE_ATTR_VOLLAB:
            printf("Volume Label");
            break;
        case FILE_ATTR_DIR:
            printf("DIR\t");
            break;
        case FILE_ATTR_ARCH:
            printf("Archive");
            break;
        case FILE_ATTR_DEVICE:
            printf("Device");
            break;
        case FILE_ATTR_UNUSED:
            printf("Unused");
            break;
        case FILE_ATTR_LSN:
            printf("Long File Name");
            break;
        default:
            printf("Unknown Attribute");
            break;
    }
}


void getWrtTimeFromFileEnt(const file_entry* ent, my_time* Cre_Time) {
    Cre_Time->year = ((ent->DIR_Cre_Date & 0xFE00) >> 9) + 1980;
    Cre_Time->month = (ent->DIR_Cre_Date & 0x01E0) >> 5;
    Cre_Time->day = (ent->DIR_Cre_Date & 0x001F);
    Cre_Time->hour = (ent->DIR_Cre_Time & 0xF800) >> 11;
    Cre_Time->minute = (ent->DIR_Cre_Time & 0x07E0) >> 5;
    Cre_Time->second = (ent->DIR_Cre_Time & 0x001F) << 1;
}

void getModiTimeFromFileEnt(const file_entry* ent, my_time* Mod_Time) {
    Mod_Time->year = ((ent->DIR_Modi_Date & 0xFE00) >> 9) + 1980;
    Mod_Time->month = (ent->DIR_Modi_Date & 0x01E0) >> 5;
    Mod_Time->day = (ent->DIR_Modi_Date & 0x001F);
    Mod_Time->hour = (ent->DIR_Modi_Time & 0xF800) >> 11;
    Mod_Time->minute = (ent->DIR_Modi_Time & 0x07E0) >> 5;
    Mod_Time->second = (ent->DIR_Modi_Time & 0x001F) << 1;
}

int createFolder(const char* folderName, FILE* file) {
    file_entry newFolderEntry;
    fseek(file, ROOT_DIRECTORY_OFFSET, SEEK_SET);

    // Find an empty entry in the root directory
    while (1) {
        size_t bytesRead = fread(&newFolderEntry, sizeof(file_entry), 1, file);

        if (bytesRead != 1) {
            perror("Error reading directory entry");
            return 1;
        }
        if (newFolderEntry.DIR_Name[0] == 0x00 || newFolderEntry.DIR_Name[0] == 0xE5) {
            break; // Found an empty or deleted entry
        }
    }

    // Set attributes for the new folder
    memset(&newFolderEntry, 0, sizeof(file_entry));
    strncpy((char*)newFolderEntry.DIR_Name, folderName, 8);
    newFolderEntry.DIR_Attr = FILE_ATTR_DIR;
    
    // Get current time
    my_time currentTime;
    getCurrentTime(&currentTime);

    // Set folder creation and modification times
    newFolderEntry.DIR_Cre_Time = (currentTime.hour << 11) | (currentTime.minute << 5) | (currentTime.second >> 1);
    newFolderEntry.DIR_Cre_Date = ((currentTime.year - 1980) << 9) | (currentTime.month << 5) | currentTime.day;
    newFolderEntry.DIR_Modi_Time = newFolderEntry.DIR_Cre_Time;
    newFolderEntry.DIR_Modi_Date = newFolderEntry.DIR_Cre_Date;

    // Write the new folder entry to the root directory
    fseek(file, -((long)sizeof(file_entry)), SEEK_CUR); // Move back to the empty entry
    fwrite(&newFolderEntry, sizeof(file_entry), 1, file);

    return 0;
}

void FAT12_linked_list_add(file_entry data, uint32_t address){
    file_entry_info *temp = malloc(sizeof(file_entry_info));
    if (temp != NULL){
        if (size == 0){
            instance = malloc(sizeof(file_entry_info));
            instance->data = data;
            instance->address = address;
            instance->pNext = NULL;
            size++;
        }
        else{
            file_entry_info *pCursor = instance;
            if (temp != NULL){
                /* Move the cursor into the last element of linked list */
                while(pCursor -> pNext != NULL){
                pCursor = pCursor -> pNext;
            }
            /* Add the element */
            pCursor-> pNext = temp;
            temp -> data = data;
            temp -> address = address;
            temp -> pNext = NULL;
            size++;
            }       
        }
    }
}

void FAT12_linked_list_remove(){
    if (size != 0 && instance != NULL){
        file_entry_info *pCursor = instance, *temp;
        while(pCursor -> pNext != NULL){
            temp = pCursor;
            pCursor = pCursor -> pNext;
            free(temp);
        }
        /* restore the linked list instance */
        instance = NULL;
        size = 0;
    }
}

void FAT12_GetDirectory(int cluster){
    log("Check dir entry at cluster %d", cluster);
    uint32_t addr_cursor;
    if (instance != NULL){
        FAT12_linked_list_remove();
    }
    /* Identify root dir and data clus */
    if (cluster == 0){
        addr_cursor = FAT12_BS_Stat.RootAddr;
    }
    else{
        cluster -= 2;
        addr_cursor = FAT12_BS_Stat.DataAddr + cluster * FAT12_BS_Stat.ClusSize;
    }
    log("Start address: 0x%X", addr_cursor);
    fseek(fptr,addr_cursor, SEEK_SET);
    file_entry entry;
    while (ftell(fptr)) {
        /* Cursor has shifted to the right 32 byte */
        fread(&entry, sizeof(file_entry), 1, fptr);
        log("Entry to read %s with attribute %d and dir_reserved %d", entry.DIR_Name, entry.DIR_Attr, entry.DIR_Reserved);
        // if ((entry.DIR_Attr == 0x00) && (entry.DIR_Reserved == 0x00))
        if (entry.DIR_Name[0] == 0x00)
            break;
        if (entry.DIR_Name[0] != 0xE5 && entry.DIR_Name[2] != 0){
            addr_cursor = ftell(fptr) - 32;
            FAT12_linked_list_add(entry, addr_cursor);
            log("Added Succesfully the node in address 0x%X", addr_cursor);
        }
    }
}

void printFileEntry(const file_entry *entry) {
    my_time Cre_Time;
    my_time Mod_Time;

    if (entry->DIR_Attr == FILE_ATTR_LSN || entry->DIR_Name[0] == 0xE5) {
        return;
    }

    printf("%-20s", entry->DIR_Name);
    printFileAttribute(entry->DIR_Attr);

    getWrtTimeFromFileEnt(entry, &Cre_Time);
    printf("%-20s", ""); // Độ rộng 20 kí tự trắng
    printf("%04d-%02d-%02d %02d:%02d:%02d\t\t", Cre_Time.year, Cre_Time.month, Cre_Time.day, Cre_Time.hour,
           Cre_Time.minute, Cre_Time.second);

    getModiTimeFromFileEnt(entry, &Mod_Time);
    printf("%04d-%02d-%02d %02d:%02d:%02d\t", Mod_Time.year, Mod_Time.month, Mod_Time.day, Mod_Time.hour,
           Mod_Time.minute, Mod_Time.second);

    printf("%u bytes\n", entry->DIR_FileSize);
}

void FAT12_Directory_Export(){
    file_entry_info *pCursor = instance;
    while(pCursor != NULL){
        printFileEntry(&pCursor->data);
        pCursor = pCursor -> pNext;
    }
}
