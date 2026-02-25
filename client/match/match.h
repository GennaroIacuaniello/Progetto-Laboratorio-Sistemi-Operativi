#ifndef MATCH_H
#define MATCH_H

#include <arpa/inet.h>

#define MAX_PLAYERS_MATCH 4

typedef struct Message_with_local_information{

    uint32_t players_position[MAX_PLAYERS_MATCH][2];        //Every row has 2 cells, if a player is in the local map, its cells are not size of the map + 1

    uint32_t my_id;                                         //Id_in_match of the player who is receiving the message, used to know its data from the array players_position 

    char local_map[5][5];

    char message[25];

}Message_with_local_information;

typedef struct Global_Info_Header{
      
    uint32_t players_position[MAX_PLAYERS_MATCH][2];      //Every row has 2 cells, if a player is in the global map, its cells are not size of the map + 1

    uint32_t my_id;                                       //Id_in_match of the player who is receiving the message, used to know its data from the array players_position 

    uint32_t size;                                        //Global map size

} Global_Info_Header;


void handle_being_in_match(int socket_fd);
void print_map(uint32_t players_positions[MAX_PLAYERS_MATCH][2], uint32_t my_id, char** map, uint32_t size);
void handle_local_info(int socket_fd, char** map, uint32_t size);
void handle_global_info(int socket_fd, char** map, uint32_t size);

void handle_match_ending(int socket_fd);

void init_terminal();
void enable_terminal_game_mode();       //Enables "game mode" in the terminal
void disable_terminal_game_mode();      //Disables "game mode" in the terminal


#endif