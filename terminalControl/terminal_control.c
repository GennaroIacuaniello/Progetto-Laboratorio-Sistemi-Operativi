#include <stdio.h>
#include "terminal_control.h"
#include "color.h"
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void clear_screen(){ 
	write(STDOUT_FILENO, "\x1b[H\x1b[2J\x1b[3J", 11);
}

void enter_screen() 
{
	puts("\x1B[?1049h\x1B[H");
}

void exit_screen() 
{
	puts("\x1B[?1049l");
}

void set_echo_off(){

	system("stty -echo"); //removes echo
	printf("\e[?25l"); //removes visible cursor
}
void set_echo_on(){

	system("stty echo"); //restores echo
	printf("\e[?25h"); //restores visible cursor
}

void set_cursor(int x, int y){ 
	printf("\x1B[%d;%dH", y, x);
}

void set_raw_input_mode(){ 
	system("/bin/stty raw");
}
void set_cooked_input_mode(){ 
	system("/bin/stty cooked");
}

void get_cols_rows(int *cols, int *rows){

	char bash_cmd[256] = "tput cols lines";
	FILE *pipe;
	int len; 

	pipe = popen(bash_cmd, "r");

	if (NULL == pipe) {
		perror("pipe");
		exit(1);
	} 
	
	fscanf(pipe, "%d", cols);
	fscanf(pipe, "%d", rows);

	pclose(pipe);
}

void color_print(char *msg, color_t fg_color, color_t bg_color){

	printf("\x1B[38;2;%d;%d;%dm\x1B[48;2;%d;%d;%dm%s\x1b[0m",
		fg_color.r, fg_color.g, fg_color.b, 
		bg_color.r, bg_color.g, bg_color.b, msg);
}