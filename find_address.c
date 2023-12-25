#include <stdio.h>
#include <stdint.h>
#pragma pack(1)

#define FILE_START_ADDR 0x0

typedef struct {
	uint8_t		BS_OEMName[8];
	uint16_t 	BPB_BytsPerSec;
    uint8_t 	BPB_SecPerClus;
    uint16_t 	BPB_RsvdSecCnt;
    uint8_t 	BPB_NumFATs;
    uint16_t 	BPB_RootEntCnt;
    uint16_t 	BPB_TotSec16;
    uint8_t 	BPB_Media;
    uint16_t 	BPB_FATSz16;
    uint16_t 	BPB_SecPerTrk;
    uint16_t 	BPB_NumHeads;
    uint32_t 	BPB_HiddSec;
    uint32_t 	BPB_TotSec32;
    uint8_t 	BS_DrvNum;
    uint8_t 	BS_Reserved;
    uint8_t 	BS_BootSig;
    uint32_t 	BS_VolID; 
	uint8_t		BS_VolLab[11];
	uint8_t		BS_FileSysType[8];  
} FAT12_Sector0_t;

void FAT12_PrintSector0(const FAT12_Sector0_t *sector0) {
    printf("BS_OEMName: %s\n", sector0->BS_OEMName);
    printf("BPB_BytsPerSec: %d\n", sector0->BPB_BytsPerSec);
    printf("BPB_SecPerClus: %d\n", sector0->BPB_SecPerClus);
    printf("BPB_RsvdSecCnt: %d\n", sector0->BPB_RsvdSecCnt);
    printf("BPB_NumFATs: %d\n", sector0->BPB_NumFATs);
    printf("BPB_RootEntCnt: %d\n", sector0->BPB_RootEntCnt);
    printf("BPB_TotSec16: %d\n", sector0->BPB_TotSec16);
    printf("BPB_Media: %d\n", sector0->BPB_Media);
    printf("BPB_FATSz16: %d\n", sector0->BPB_FATSz16);
    printf("BPB_SecPerTrk: %d\n", sector0->BPB_SecPerTrk);
    printf("BPB_NumHeads: %d\n", sector0->BPB_NumHeads);
    printf("BPB_HiddSec: %d\n", sector0->BPB_HiddSec);
    printf("BPB_TotSec32: %d\n", sector0->BPB_TotSec32);
    printf("BS_DrvNum: %d\n", sector0->BS_DrvNum);
    printf("BS_Reserved: %d\n", sector0->BS_Reserved);
    printf("BS_BootSig: %d\n", sector0->BS_BootSig);
    printf("BS_VolID: %d\n", sector0->BS_VolID);
    printf("BS_VolLab: %s\n", sector0->BS_VolLab);
    printf("BS_FileSysType: %s\n", sector0->BS_FileSysType);
}

FAT12_Sector0_t sector0 = {
//        .BS_OEMName = "MSDOS5.0",
//        .BPB_BytsPerSec = 512,
//        .BPB_SecPerClus = 1,
//        .BPB_RsvdSecCnt = 1,
//        .BPB_NumFATs = 2,
//        .BPB_RootEntCnt = 224,
//        .BPB_TotSec16 = 2880,
//        .BPB_Media = 0xF0,
//        .BPB_FATSz16 = 9,
//        .BPB_SecPerTrk = 18,
//        .BPB_NumHeads = 2,
//        .BPB_HiddSec = 0,
//        .BPB_TotSec32 = 0,
//        .BS_DrvNum = 0,
//        .BS_Reserved = 0,
//        .BS_BootSig = 0x29,
//        .BS_VolID = 0x00000109,
//        .BS_VolLab = "NO NAME",
//        .BS_FileSysType = "FAT12"
};

/*
- FatStartSector = BPB_ResvdSecCnt = 1 

- FatSectors = BPB_FATSz * BPB_NumFATs = 9 * 2 = 18 sector


- RootDirStartSector = FatStartSector + FatSectors = 1 + 18 = 19 sector

(The 32 in the equation is the size of a directory entry)
- RootDirSectors = (32 * BPB_RootEntCnt) / BPB_BytsPerSec;

		 = (32 * 224) / 512
		 
		 = 14
- DataStartSector = RootDirStartSector + RootDirSectors;
		  
		  = 19 + 14 = 33

- DataSectors = BPB_TotSec - DataStartSector
	      
	      = 2880 - 33 = 2847
*/

uint32_t FatStartSector(const FAT12_Sector0_t *ptr) {
    return ptr->BPB_RsvdSecCnt;
}

uint32_t FatSectors(const FAT12_Sector0_t *ptr) {
    return ptr->BPB_FATSz16 * ptr->BPB_NumFATs;
}

uint32_t RootDirStartSector(const FAT12_Sector0_t *ptr) {
    return FatStartSector(ptr) + FatSectors(ptr);
}

uint32_t RootDirSectors(const FAT12_Sector0_t *ptr) {
    return (32 * ptr->BPB_RootEntCnt) / ptr->BPB_BytsPerSec;
}

uint32_t DataStartSector(const FAT12_Sector0_t *ptr) {
    return RootDirStartSector(ptr) + RootDirSectors(ptr);
}

uint32_t DataSectors(const FAT12_Sector0_t *ptr) {
    uint32_t DataSectors = 0;
	if (ptr->BPB_TotSec32 != 0) {
        DataSectors = ptr->BPB_TotSec32 - DataStartSector(ptr);
    } else {
        DataSectors = ptr->BPB_TotSec16 - DataStartSector(ptr);
    }
    return DataSectors;
}

void FAT12_PrintSector(const FAT12_Sector0_t *ptr) {
    printf("FatStartSector: %d\n", FatStartSector(ptr));
    printf("FatSectors: %d\n", FatSectors(ptr));
    printf("RootDirStartSector: %d\n", RootDirStartSector(ptr));
    printf("RootDirSectors: %d\n", RootDirSectors(ptr));
    printf("DataStartSector: %d\n", DataStartSector(ptr));
    printf("DataSectors: %d\n", DataSectors(ptr));
}

/*
	BootSectorAddr 0x0
	FatSectorAddr 0x200
	RootDirSectorAddr 0x2600
	DataSectorAddr 0x4200
*/

void FAT12_FindStartAddr(const FAT12_Sector0_t *ptr) {
	uint32_t FatSector = FatSectors(ptr);
	uint32_t RootDirSector = RootDirSectors(ptr);
	
	uint32_t BootSectorStartAddr = FILE_START_ADDR;
	uint32_t FatStartAddr = BootSectorStartAddr + sector0.BPB_BytsPerSec;
	uint32_t RootDirStartAddr = FatStartAddr + sector0.BPB_BytsPerSec * FatSector;
	uint32_t DataStartAddr = RootDirStartAddr + sector0.BPB_BytsPerSec * RootDirSector;
	
	printf("BootSectorStartAddr: %x\n", BootSectorStartAddr);
	printf("FatStartAddr: %x\n", FatStartAddr);
	printf("RootDirStartAddr: %x\n", RootDirStartAddr);
	printf("DataStartAddr: %x\n", DataStartAddr);
}

void main() {
    FILE * fileptr = fopen("D://HUY//Documents//Embedded Developer FA2023//C Fundamentals//Group 5//C Mock Project_File FAT Parsing//floppy.img", "r");

    if (fileptr == NULL) {
        printf("Cannot open file!\n");
    }

    fseek(fileptr, 0x03, SEEK_SET);
    fread(&sector0, sizeof(FAT12_Sector0_t), 1, fileptr);

    fclose(fileptr);
    
    FAT12_PrintSector0(&sector0);
//	printf("%d", sizeof(FAT12_Sector0_t));
	printf("\n");
	FAT12_PrintSector(&sector0);
	printf("\n");
	FAT12_FindStartAddr(&sector0);
}

