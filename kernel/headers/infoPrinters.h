#ifndef _INFO_PRINTERS
#define _INFO_PRINTERS
#include "types.h"

void printPciInfo();
void printPciDevices();
void printAHCI();
void print_memory_map();
void printMemoryManagerTable();
void mmap_terminal_command(string_array* args);
void memory_manager_terminal_command(string_array* args);

#endif