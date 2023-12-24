#include<stdint.h>
#include<stdio.h>
#include<math.h>

#include "FAT_RootEntry.h"





int main()
{
    Status_t status;
	int i = 0;
    char FName[100];

    printf("Enter your path to file (include file name) \n");
    printf("Remember to change path to correct format \n");
    printf("(Example: C:\\\\Desktop\\\\Assignment_3\\\\test.txt) \n\n");
    
    if (scanf("%99[^\n]", FName) != 1 || strlen(FName) == 99) {
        printf("Input error or too many characters.\n");
        
    }
//	printf("%s", FName);
    status = File_Open(FName);
//    printf("\n%d",status);
    //File cannot open 
    if(status == FILE_OPEN_ERROR)
    {
        printf("Can't open file\n");
        
    }
    else
    {
    	
    	Check_Root_Entry(0,3);
	}
    

    
	//\\C:\\Users\\Admin\\Desktop\\testFat\\Code\\floppy.img
    return 0;
}

