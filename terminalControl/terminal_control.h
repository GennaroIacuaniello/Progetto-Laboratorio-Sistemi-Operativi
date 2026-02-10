#ifndef TC_H
#define TC_H

#include "color.h"



void clear_screen(); //clears the terminal
void enter_screen(); //enters a temporary alternative screen
void exit_screen(); //exits the temporary alternative screen

void set_cursor(int x, int y); //positions the cursor to x column, y row. Values grow going to the right and down

void set_echo_off();
void set_echo_on();

void set_raw_input_mode(); //does not wait for ENTER to be pressed to read the stdin
void set_cooked_input_mode(); //cooked input should be normal input mode

void get_cols_rows(int *cols, int *rows);
void color_print(char* msg, color_t fg_color, color_t bg_color);

#endif

