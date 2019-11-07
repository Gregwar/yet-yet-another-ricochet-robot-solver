#pragma once

#include <unordered_map>
#include <map>
#include <stdint.h>
#include <vector>
#include <iostream>

namespace RicochetRobot
{

// Map dimension
#define MAP_WIDTH 16
#define MAP_HEIGHT 16

// Robots
#define ROBOT_COUNTS 4
#define ROBOT_RED 0
#define ROBOT_GREEN 1
#define ROBOT_BLUE 2
#define ROBOT_YELLOW 3
#define ROBOT_ANY 4

// Bit logic wall
#define MAP_NONE 0
#define MAP_LEFT (1 << 0)
#define MAP_RIGHT (1 << 1)
#define MAP_UP (1 << 2)
#define MAP_DOWN (1 << 3)

// What is on a specific box (only for initialization)
#define MAP_ROBOT_NONE 0
#define MAP_ROBOT_RED 1
#define MAP_ROBOT_GREEN 2
#define MAP_ROBOT_BLUE 3
#define MAP_ROBOT_YELLOW 4
#define MAP_ROBOT_TARGET 5

// A simple position on the grid
struct Position {
  int x, y;

  bool operator==(const Position &other);
};

// All the robots positions
struct Robots {
  Position positions[ROBOT_COUNTS];
  bool operator==(const Robots &other) const;
};

// Key hasher for robots positions
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

// The map object representing the walls and initial robots
struct Map {
  Map();

  Position target;
  Robots initialRobots;
  uint8_t data[MAP_WIDTH][MAP_HEIGHT];

  void readFromFile(const char *filename);
  void print();

  bool move(Robots &robots, int robot, int move);
};

// Start the solve!
std::vector<Robots> solve(Map &map, int robot);

}