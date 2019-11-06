#pragma once

#include <map>
#include <stdint.h>
#include <vector>

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

#define ROBOT_COUNTS 4
#define ROBOT_RED 0
#define ROBOT_GREEN 1
#define ROBOT_BLUE 2
#define ROBOT_YELLOW 3
#define ROBOT_ANY 4

#define MAP_NONE 0
#define MAP_LEFT (1 << 0)
#define MAP_RIGHT (1 << 1)
#define MAP_UP (1 << 2)
#define MAP_DOWN (1 << 3)

#define MAP_ROBOT_NONE 0
#define MAP_ROBOT_RED 1
#define MAP_ROBOT_GREEN 2
#define MAP_ROBOT_BLUE 3
#define MAP_ROBOT_YELLOW 4
#define MAP_ROBOT_TARGET 5

struct Position {
  int x, y;

  bool operator==(const Position &other);
};

struct Robots {
  Position positions[ROBOT_COUNTS];
  bool operator<(const Robots &other) const;
  bool operator==(const Robots &other) const;
};

struct KeyHasher {
  std::size_t operator()(const Robots &robots) const {
    std::size_t cs = 0;

    for (int k = 0; k < ROBOT_COUNTS; k++) {
      cs <<= 4;
      cs |= robots.positions[k].x;
      cs <<= 4;
      cs |= robots.positions[k].y;
    }

    return cs;
  }
};

struct Map {
  Map();

  Position target;
  Robots initialRobots;
  uint8_t data[MAP_WIDTH][MAP_HEIGHT];

  void readFromFile(const char *filename);
  void print();

  bool move(Robots &robots, int robot, int move);
};

bool canReach(Map &map, Robots &robots, int robot, int moves, int depth = 0);

extern int nbTries;