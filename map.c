#ifndef _RB_MAP_H
#define _RB_MAP_H

#include <stdint.h>

#define WALL_NONE    0
#define WALL_LEFT    0b1
#define WALL_RIGHT   0b10
#define WALL_UP      0b100
#define WALL_DOWN    0b1000

extern uint8_t rb_map[8][8];

void rb_map_init();

#endif