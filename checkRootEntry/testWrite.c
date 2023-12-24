#include<stdint.h>
#include<stdio.h>

FILE *ptr;

int main()
{
    char FName[] = "C:\\Users\\Admin\\Desktop\\testFat\\testWrite.txt";
    char test[]  = "34";
    char newData[] = "0";
    ptr = fopen(FName,"r+");
	if(ptr == NULL)
    {
        
        printf("Can't open file");

    }
    else
    {
        fseek(ptr,2,SEEK_SET);
        fwrite((char *)"0", sizeof(char), sizeof("0")-1,ptr);
    }

    fclose(ptr);

}
