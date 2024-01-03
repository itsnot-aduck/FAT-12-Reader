#include <stdio.h>
#include <stdlib.h>
#include "fat12-parser.h"
#include "fat12-entry.h"

/* Linked list information of node */
// typedef struct node{
//     file_entry data;
//     struct node *pNext;
// } file_entry_info;

/* Initialize the instance of linked list*/
file_entry_info *instance = NULL;
static int size = 0;

typedef struct time {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} time;

void getWrtTimeFromFileEnt( const file_entry* ent, time* Cre_Time)
{
    Cre_Time -> year = ((ent->DIR_Cre_Date & 0xFE00) >> 9) + 1980;
    Cre_Time -> month = (ent->DIR_Cre_Date & 0x01E0) >> 5;
    Cre_Time -> day  = (ent->DIR_Cre_Date & 0x001F);
    Cre_Time -> hour = (ent->DIR_Cre_Time & 0xF800) >> 11;
    Cre_Time -> minute = (ent->DIR_Cre_Time & 0x07E0) >> 5;
    Cre_Time -> second  = (ent->DIR_Cre_Time & 0x001F) << 1;
}

void getModiTimeFromFileEnt( const file_entry* ent, time* Mod_Time)
{
    Mod_Time -> year = ((ent->DIR_Modi_Date & 0xFE00) >> 9) + 1980;
    Mod_Time -> month = (ent->DIR_Modi_Date & 0x01E0) >> 5;
    Mod_Time -> day  = (ent->DIR_Modi_Date & 0x001F);
    Mod_Time -> hour = (ent->DIR_Modi_Time & 0xF800) >> 11;
    Mod_Time -> minute = (ent->DIR_Modi_Time & 0x07E0) >> 5;
    Mod_Time -> second  = (ent->DIR_Modi_Time & 0x001F) << 1;
}
void FAT12_linked_list_add(file_entry data){
    file_entry_info *temp = malloc(sizeof(file_entry_info));
    if (temp != NULL){
        if (size == 0){
            instance = malloc(sizeof(file_entry_info));
            instance->data = data;
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
    /* Identify root dir and data clus */
    if (cluster == 0){
        fseek(fptr,FAT12_BS_Stat.RootAddr, SEEK_SET);
    }
    else{
        fseek(fptr,FAT12_BS_Stat.DataAddr + cluster * FAT12_BS_Stat.ClusSize, SEEK_SET);
        log("start address: 0x%X", FAT12_BS_Stat.DataAddr + cluster * FAT12_BS_Stat.ClusSize);
    }
    
    file_entry entry;
    /* if not root (cluster != 0). bypass the e ntry */

    while (ftell(fptr)) {
        fread(&entry, sizeof(file_entry), 1, fptr);
        log("Entry to read %s with attribute %d and dir_reserved %d", entry.DIR_Name, entry.DIR_Attr, entry.DIR_Reserved);
        if ((entry.DIR_Attr == 0x00) && (entry.DIR_Reserved == 0x00))
            break;
        if (entry.DIR_Name[0] != 0xE5 && entry.DIR_Name[2] != 0){
            FAT12_linked_list_add(entry);
            log("Added");
        }
  
    }
}

void printFileEntry(const file_entry *entry) {
    time Cre_Time;
    time Mod_Time;

    if (entry->DIR_Attr == FILE_ATTR_LSN) {
        return;
    }

    printf("+ %s - ", entry->DIR_Name);
    // printFileAttribute(entry->DIR_Attr);

    getWrtTimeFromFileEnt(entry, &Cre_Time);
    printf("Create at: %04d-%02d-%02d %02d:%02d:%02d", Cre_Time.year, Cre_Time.month, Cre_Time.day,
           Cre_Time.hour, Cre_Time.minute, Cre_Time.second);

    printf("     ");

    getModiTimeFromFileEnt(entry, &Mod_Time);
    printf("Modify at: %04d-%02d-%02d %02d:%02d:%02d", Mod_Time.year, Mod_Time.month, Mod_Time.day,
           Mod_Time.hour, Mod_Time.minute, Mod_Time.second);

    printf(" File Size: %u bytes\n", entry->DIR_FileSize);
}

void FAT12_Directory_Export(){
    file_entry_info *pCursor = instance;
    while(pCursor != NULL){
        printFileEntry(&pCursor->data);
        pCursor = pCursor -> pNext;
    }
}
