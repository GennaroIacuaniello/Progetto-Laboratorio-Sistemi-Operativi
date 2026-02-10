#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include "color.h"

#define UP			'W'
#define LEFT 		'A'
#define DOWN 		'S'
#define RIGHT		'D'
#define EXIT_SCREEN 'Q'

typedef uint64_t cell_t;

typedef struct {
	cell_t **map;
	int size;
} map_t;

void init_map(map_t *game, int size); //makes space in memory to hold the map
void map_choose(map_t *game, int type); //copies a map preset into game chosen by type
void map_draw(map_t game); //renders the map on the terminal

void init_player(map_t *game, int *x, int *y); //drops the player in a random spot on the map
void move_player(map_t *game, int x, int y, int prev_x, int prev_y); //moves the player on the map

int is_wall(cell_t cell);
int is_empty(cell_t cell);

color_t get_cell_color(cell_t cell); //returns the appropriate color to use for a cell

#endif