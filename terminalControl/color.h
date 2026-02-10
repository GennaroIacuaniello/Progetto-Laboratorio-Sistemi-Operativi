#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

typedef struct{

	uint8_t r;
	uint8_t g;
	uint8_t b;
} color_t;

#define COLOR_WHITE			(color_t){ .r = 240, .g = 240, .b = 240}
#define COLOR_BLACK			(color_t){ .r = 15, .g = 15, .b = 15}

#define COLOR_RED			(color_t){ .r = 255, .g = 0, .b = 0}
#define COLOR_GREEN			(color_t){ .r = 0, .g = 255, .b = 0}
#define COLOR_BLUE 			(color_t){ .r = 0, .g = 0, .b = 255}
#define COLOR_YELLOW		(color_t){ .r = 255, .g = 255, .b = 0}
#define COLOR_CYAN			(color_t){ .r = 0, .g = 255, .b = 255}
#define COLOR_MAGENTA		(color_t){ .r = 255, .g = 0, .b = 255}

#define COLOR_ORANGE		(color_t){ .r = 255, .g = 170, .b = 0}
#define COLOR_LIME		 	(color_t){ .r = 170, .g = 255, .b = 0}
#define COLOR_AQUAMARINE	(color_t){ .r = 0, .g = 255, .b = 170}
#define COLOR_LIGHT_BLUE	(color_t){ .r = 0, .g = 170, .b = 255}
#define COLOR_PINK			(color_t){ .r = 255, .g = 0, .b = 170}
#define COLOR_PURPLE		(color_t){ .r = 170, .g = 0, .b = 255}

color_t darken_color(color_t old_color, int percentage); // 0 <= percentage <= 100. 100 makes it black, 0 leaves it as is

#endif
