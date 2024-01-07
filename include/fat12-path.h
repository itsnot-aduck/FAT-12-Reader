#ifndef __FAT12_PATH__
#define __FAT12_PATH__

#include "fat12-config.h"

void PATH_move_to_child(char* node);
void PATH_back_to_father();
void PATH_back_to_root();
void Path_Get_Current();
uint8_t PATH_Get_Depth();
#endif /*__FAT12_PATH__*/