#ifndef FAT_ROOT_ENTRY_H
#define FAT_ROOT_ENTRY_H

#define BPB_BytsPerSec 512
#define FatStartSector 512
#define RootDirStartSector 9728
#define DataStartSector 16896

typedef enum
{
	FILE_OK						=	0U,
	FILE_OPEN_ERROR				=	1U,
	FILE_END					=   2U
	
}Status_t;

typedef enum
{
    ROOT_ENTRY_IS_FILE,
    ROOT_ENTRY_IS_FOLDER,
    ROOT_ENTRY_NOT_VALID,
    ROOT_ENTRY_SYNTAX_ERROR
}Status_RootEntry_t;


int Hexa_Convert(char *ptr);

Status_RootEntry_t Check_Root_Entry(uint16_t Root_EntryCurrent, uint16_t Root_EntryCheck);



#endif
