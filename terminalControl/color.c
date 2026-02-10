#include "color.h"

color_t darken_color(color_t old_color, int percentage){

    return (color_t){   .r = old_color.r * (1 - percentage / 100.0),
                        .g = old_color.g * (1 - percentage / 100.0),
                        .b = old_color.b * (1 - percentage / 100.0)};

}