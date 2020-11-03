#ifndef _TERMINAL_PROGRAMS_H
#define _TERMINAL_PROGRAMS_H
#include "types.h"
#include "advanced_string.h"

struct terminal_program{
    void (*function_pointer)(string_array*);
    char* name;
    uint32 name_checksum;
};

struct terminal_program *Terminal_programs;
void init_terminal_program_list();
void terminal_program_test(string_array* args);
void pci_terminal_command(string_array* args);
void terminal_program_test2(string_array* args);

uint16 program_count;

#endif