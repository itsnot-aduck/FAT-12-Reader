#include<stdint.h>
#include<stdio.h>

#include "FAT_RootEntry.h"

static FILE *ptr;

Status_t File_Open(char *FName)
{
	Status_t status = FILE_OK;
	ptr = fopen(FName,"r");
	
	if(ptr == NULL) status = FILE_OPEN_ERROR;
	
	return status; 
}

int Hexa_Convert(char *ptr)
{
	//make sure s_HexValue = 0 before assign data
	int s_HexValue = 0;
	int i,k=0;

	for(i=1;i>=0;i--)
	{
		
		if(ptr[i] == '\n'|| (ptr+i) == NULL)
		{
			break;
		}
		//If ptr[i] not belong to '0' to '9' or 'A' to 'F'
		//Printf not valid and return 0
		if(!((ptr[i] >= '0' &&ptr[i] <= '9') || ptr[i] >= 'A' && ptr[i] <= 'F' ))
		{
			break;			
		}
		
		//Change character value which show in ASCII table to Hexa Value of it
		if(ptr[i] >= '0' && ptr[i] <= '9')
		{
			ptr[i] = ptr[i] - '0';
		}
		if(ptr[i] >= 'A' && ptr[i] <= 'F' )
		{
			ptr[i] = ptr[i] - 55;
		}
		//Add to result to receive hexa value
		s_HexValue += ptr[i] * pow(16,k);

		k++;
	}

	return s_HexValue;
	
}

//Ham nhan dien so cluster number nay la the hien thu muc hay file
//Trong cung 1 luc chi truy cap den 1 thu muc
//Input: cluster number cua thu muc dang mo, va cluster number cua root_entry muon test
//Output: enum trang thai, la thu muc, la tep, hay co loi cu phap

Status_RootEntry_t Check_Root_Entry(uint16_t Root_EntryCurrent, uint16_t Root_EntryCheck)
{
    uint32_t position, offset ;
	uint16_t Root_Entry_tmp;
	uint8_t tmp[3];
	uint32_t p_Pos;

	position = (Root_EntryCheck - 2)*512 + DataStartSector;
	offset = 27;
	Status_RootEntry_t status;
  	
	if(Root_EntryCheck == 0) 
	{
		status = ROOT_ENTRY_NOT_VALID;
	}
	else
	{
		
		//Kiem tra 2 thu, 1 la byte dau tien co phai "2E" hay dau "." khong ?
		//Tiep theo kiem tra cluster xem co trung khong 
		//Chuyen con tro den vi tri cua Root_EntryCheck do
		
		//Check byte dau tien
		fseek(ptr,position, SEEK_SET );
		p_Pos = ftell(ptr);
		if(fgetc(ptr) == 0x2E)
		{
//			fseek(ptr,position+1, SEEK_SET);
//			fgets(tmp,3,ptr);
//			printf("%x",tmp);
			
			fseek(ptr,position+32, SEEK_SET);
			if(fgetc(ptr) == 0x2E && fgetc(ptr)==0x2E) 
			{
				status = ROOT_ENTRY_IS_FOLDER;
				printf("Folder");
			}
			
		}
		else
		{
			status = ROOT_ENTRY_IS_FILE;
		}
	}
    return status;
	
}





//Ham nhay den vi tri bat ki
//Input la bytes muon nhay toi
//Output la ket qua nhay toi duoc hay khong
