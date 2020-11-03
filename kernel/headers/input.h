#ifndef _INPUT_H
#define _INPUT_H
#include "types.h"

#define KEYBOARD_PORT 0x60

uint8 testinputinterruptlast_code;
uint8 testinputinterrupt_event_slot;
char ascii_keymap[128];
char shift_ascii_keymap[128];

void init_keymap();
uint8 next_keyboard_byte();
void input_test();
void read(char buffer[],uint8 count);
void testinputinterrupt(uint8 key);
void test_input_interrupt();

#endif