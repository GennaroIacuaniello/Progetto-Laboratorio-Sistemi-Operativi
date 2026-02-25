#include "match.h"
#include "network.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>    //To handle "moving in the map" during the game
#include <errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <limits.h>

struct termios orig_termios;        //Used to manipulate input reading during a match

//Array of players backgrond colors
const char* bg_colors[4] = {
    "\033[44m", //0: Blue (b)
    "\033[41m", //1: Red (r)
    "\033[42m", //2: Green (g)
    "\033[43m"  //3: yellow (y)
};


//Reset to go back to normal terminal background color
#define RESET       "\033[0m"

//Background colors
#define BG_BLACK    "\033[40m"   //For empty cells ('e')
#define BG_WHITE    "\033[47m"   //For walls ('w')
#define BG_BLUE     "\033[44m"   //Player 0 ('b')
#define BG_RED      "\033[41m"   //Player 1 ('r')
#define BG_GREEN    "\033[42m"   //Player 2 ('g')
#define BG_YELLOW   "\033[43m"   //Player 3 ('y')

#define FG_WHITE_BOLD "\033[1;37m"
#define FG_BLACK_BOLD "\033[1;30m"


void handle_being_in_match(int socket_fd){

    enable_terminal_game_mode();

    uint32_t size_received, size = 0;

    //First, reads map's size from the server
    ssize_t r =recv_all_in_match(socket_fd, &size_received, sizeof(size_received), 0);

    if (r == sizeof(size_received))
        size = ntohl(size_received);


    //Allocating the map
    char** map = malloc(size*sizeof(char*));

    if (!map) {

        perror("malloc");
        close(socket_fd);

        printf("\033[?1049l"); // EXIT_ALT_SCREEN
        fflush(stdout);

        printf("%s", "\nErrore fatale\n");
        
        exit(0);

    }

    int i, j;
    for(i=0; i < size; i++){

        map[i] = malloc(size*sizeof(char));

        if (!(map[i])) {

            perror("malloc");
            close(socket_fd);

            printf("\033[?1049l"); // EXIT_ALT_SCREEN
            fflush(stdout);

            printf("%s", "\nErrore fatale\n");
            
            exit(0);

        }

    }

    //Initialize map all empty ('e')
    for(i=0; i < size; i++)
        for(j=0; j < size; j++)
            map[i][j] = 'e';

    int stdin_open = 1;
    fd_set rset;

    uint32_t status_received, status_code = 1;
	
    while (1) {
    	
    	FD_ZERO(&rset);
    	
    	if (stdin_open) 
			FD_SET(STDIN_FILENO, &rset);
		
        FD_SET(socket_fd, &rset);
        
        int maxfd = (STDIN_FILENO > socket_fd ? STDIN_FILENO : socket_fd) + 1;
        
        int nready = select(maxfd, &rset, NULL, NULL, NULL);

        if (nready < 0) {

            if (errno == EINTR) 
                continue;

            perror("select");
            close(socket_fd);
            exit(1);
            break;

        }
        
        //Socket ready: read from server
        if (FD_ISSET(socket_fd, &rset)) {

            ssize_t r = recv_all_in_match(socket_fd, &status_received, sizeof(status_received), 0);

            if (r == sizeof(status_received)){
                status_code = ntohl(status_received);
            }else{
                status_code = UINT_MAX;    //if an error occured, garbage value
            }
            
            switch (status_code){
                case 2:
                    //Deallocating the map
                    for(i = 0; i < size; i++)
                            free(map[i]);
                    
                    free(map);

                    handle_match_ending(socket_fd);
                    return;
                case 0:
                    handle_local_info(socket_fd, map, size);
                    break;
                case 1:
                    handle_global_info(socket_fd, map, size);
                    break;
                default:
                    //Error occured, do nothing
                    break;
            }

        }

        // stdin pronto: leggi e invia al server
        if (stdin_open && FD_ISSET(STDIN_FILENO, &rset)) {

            char option;

            
            int nread = read(STDIN_FILENO, &option, 1);     //Reads 1 char (1 byte)

            if (nread <= 0) {
                //EOF on stdin
                stdin_open = 0;
                option = 'l';    // Garbage value

                if (shutdown(socket_fd, SHUT_WR) < 0) { 
                    perror("shutdown"); 
                    disable_terminal_game_mode();     //Disables game mode in terminal

                    printf("\033[?1049l"); // EXIT_ALT_SCREEN
                    fflush(stdout);

                    exit(1);
                    break; 
                }

            }

            //Sends the move to the server
            send_all_in_match(socket_fd, &option, sizeof(option));

        }
        
    }

    
}


void init_terminal(){

    tcgetattr(STDIN_FILENO, &orig_termios);

}

void enable_terminal_game_mode() {

    tcgetattr(STDIN_FILENO, &orig_termios); //Saves previous attributes
    struct termios raw = orig_termios;
    
    // Disabilits ECHO (doesn't print pressed keys) and ICANON (to have instant reading, without having to wait to press enter)
    raw.c_lflag &= ~(ECHO | ICANON); 
    
    //Sets new attributes
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

void disable_terminal_game_mode() {

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);  //Sets previous attributes

    tcflush(STDIN_FILENO, TCIFLUSH);

}


void print_map(uint32_t players_positions[MAX_PLAYERS_MATCH][2], uint32_t my_id, char** map, uint32_t size){

    int i, j, p;

    //CLEAR_SCREEN and CURSOR_HOME to put the cursor at the top left corner of the screen
    printf("\033[2J\033[H");

    //Print upper border
    printf("  +");

    for(i=0; i < size; i++) 
        printf("--");

    printf("+\n");
    

    for (i = 0; i < size; i++) {
        printf("  |"); //Print left border
        
        for (j = 0; j < size; j++) {
            
            //Check players positions
            int player_on_cell = -1;
            for (p = 0; p < MAX_PLAYERS_MATCH; p++) {
                if (players_positions[p][0] == i && players_positions[p][1] == j) {
                    player_on_cell = p;
                    break; //Player founded
                }
            }

            //If there is a player, print its symbol and its color
            if (player_on_cell != -1) {

                if (player_on_cell == my_id) {

                    //This player
                    printf("%s%s★ %s", FG_WHITE_BOLD, bg_colors[player_on_cell], RESET);

                } else {

                    //Enemies
                    printf("%s%s♦ %s", FG_BLACK_BOLD, bg_colors[player_on_cell], RESET);

                }
                continue; //Go to next cell
            }

            //When there is no player
            switch (map[i][j]) {
                case 'e': 
                    printf("%s  %s", BG_BLACK, RESET); 
                    break;
                case 'w': 
                    printf("%s  %s", BG_WHITE, RESET); 
                    break;
                case 'b': 
                    printf("%s  %s", BG_BLUE, RESET); 
                    break;
                case 'r': 
                    printf("%s  %s", BG_RED, RESET); 
                    break;
                case 'g': 
                    printf("%s  %s", BG_GREEN, RESET); 
                    break;
                case 'y': 
                    printf("%s  %s", BG_YELLOW, RESET); 
                    break;
                default:  
                    printf("  "); 
                    break;
            }
        }
        printf("|\n"); //Print right border
    }


    //Print lower border
    printf("  +");

    for(i=0; i < size; i++) 
        printf("--");

    printf("+\n");


}

void handle_local_info(int socket_fd, char** map, uint32_t size){

    Message_with_local_information message_with_local_information;

    recv_all_in_match(socket_fd, &message_with_local_information, sizeof(message_with_local_information), 0);

    uint32_t my_id = ntohl(message_with_local_information.my_id);

    //Converts back all the coordinates received
    int p;
    for (p = 0; p < MAX_PLAYERS_MATCH; p++) {
        message_with_local_information.players_position[p][0] = ntohl(message_with_local_information.players_position[p][0]);
        message_with_local_information.players_position[p][1] = ntohl(message_with_local_information.players_position[p][1]);
    }

    //Coordinates of the player are the center of the local map received
    int x = message_with_local_information.players_position[my_id][0];
    int y = message_with_local_information.players_position[my_id][1];
    

    int i, j, z, k;
    z = 0;
    for(i= x - 2; i <= x + 2; i++, z ++){

        k = 0;
        for(j = y - 2; j <= y + 2; j++, k++ ){

                if( (i >= 0 && i < size)  && (j >= 0 && j < size) ){            //If i and j are "good" indexes

                    map[i][j] = message_with_local_information.local_map[z][k];

                }


        }
    }

    print_map(message_with_local_information.players_position, my_id, map, size);

    printf("%s\n", message_with_local_information.message);

}

void handle_global_info(int socket_fd, char** map, uint32_t size){

    Global_Info_Header global_Info_Header;

    recv_all_in_match(socket_fd, &global_Info_Header, sizeof(global_Info_Header), 0);

    uint32_t my_id = ntohl(global_Info_Header.my_id);

    //Converts back all the coordinates received
    int p;
    for (p = 0; p < MAX_PLAYERS_MATCH; p++) {
        global_Info_Header.players_position[p][0] = ntohl(global_Info_Header.players_position[p][0]);
        global_Info_Header.players_position[p][1] = ntohl(global_Info_Header.players_position[p][1]);
    }

    char* new_global_map = malloc(size*size*sizeof(char));

    if (!new_global_map) {

        perror("malloc");
        close(socket_fd);

        printf("\033[?1049l"); // EXIT_ALT_SCREEN
        fflush(stdout);

        printf("%s", "\nErrore fatale\n");
        
        exit(0);

    }

    recv_all_in_match(socket_fd, new_global_map, size*size*sizeof(char), 0);

    int i, j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){

            if(map[i][j] != 'w')
                map[i][j] = new_global_map[ (i*size) + j ];


        }
    }

    free(new_global_map);

    print_map(global_Info_Header.players_position, my_id, map, size);

    printf("%s\n", "\nMuoversi con w, a, s, d");

}


void handle_match_ending(int socket_fd){

    char ending_message[MAX_LENGHT];

    //Receive ending message
    recv_string_in_match(socket_fd, ending_message, MAX_LENGHT, 0);

    printf("%s", ending_message);

    sleep(30);

    disable_terminal_game_mode();

}
