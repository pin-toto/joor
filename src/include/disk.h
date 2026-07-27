#ifndef DISK_H
#define DISK_H

#include "kernel.h"

void read_sector(uint32_t sector, uint8_t* buffer);
void write_sector(uint32_t sector, const uint8_t* buffer);

#endif
