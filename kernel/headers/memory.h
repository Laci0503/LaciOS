#ifndef _MEMORY_H
#define _MEMORY_H
#include "types.h"
#include "advanced_string.h"

#define MEM_STATE_FREE 1
#define MEM_STATE_RESV 0
#define MEM_BASE 0x100000

#define MEM_DEBUG 0

uint32 memory_pieces[200];
uint8 memory_state[25];
uint8 memory_piece_count;
uint32 heap_size;
uint32 avaible_ram;
uint64* memroy_map_region_base;
uint64* memory_map_region_length;
uint8* memory_map_region_type;
uint8 memory_map_region_count;

uint8 mem_get_state(uint8 piece);
void load_memory_map();
void free(uint8* address, uint32 length);
void* malloc(uint32 length);
void mem_clear_overleaps();
void memory_init();
void memcpy(uint8* from, uint8* to, uint32 amount);
uint8 memcmp(uint8* m1, uint8* m2, uint32 length);
void memset(uint8* address, uint32 length, uint8 byte);

#endif