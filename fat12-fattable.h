#ifndef __FAT12_FATTABLE_H__
#define __FAT12_FATTABLE_H__

#include <stdint.h>
#include "fat12-config.h"

uint16_t FAT12_Fat_Read(int num);
void FAT12_Fat_Free(int num);

#endif
/* __FAT12_FATTABLE_H__ */