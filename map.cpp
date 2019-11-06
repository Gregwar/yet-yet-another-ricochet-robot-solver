#include "map.h"
#include <iostream>
#include <stdio.h>

Map::Map() {}

void Map::readFromFile(const char *filename) {
  FILE *f = fopen(filename, "rb");
  if (f) {
    fread(data, MAP_WIDTH, MAP_HEIGHT, f);
  } else {
    std::cerr << "Can't read file " << filename << std::endl;
  }

  // Searching for robots
  int initials = 0;
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      uint8_t part = (data[x][y] >> 4) & 0xf;

      if (part == MAP_ROBOT_RED) {
        initialRobots.positions[ROBOT_RED].x = x;
        initialRobots.positions[ROBOT_RED].y = y;
        initials++;
      }
      if (part == MAP_ROBOT_GREEN) {
        initialRobots.positions[ROBOT_GREEN].x = x;
        initialRobots.positions[ROBOT_GREEN].y = y;
        initials++;
      }
      if (part == MAP_ROBOT_BLUE) {
        initialRobots.positions[ROBOT_BLUE].x = x;
        initialRobots.positions[ROBOT_BLUE].y = y;
        initials++;
      }
      if (part == MAP_ROBOT_YELLOW) {
        initialRobots.positions[ROBOT_YELLOW].x = x;
        initialRobots.positions[ROBOT_YELLOW].y = y;
        initials++;
      }
      if (part == MAP_ROBOT_TARGET) {
        target.x = x;
        target.y = y;
        initials++;
      }
    }
  }

  if (initials++ != 5) {
    std::cout << "/!\\ Something is missing (found " << initials << ") parts"
              << std::endl;
  }
}

void Map::print() {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      std::cout << (int)(data[x][y] & 0xff) << " ";
    }
    std::cout << std::endl;
  }
}

Position Map::move(Robots &robots, int robot, int move) {
  Position position = robots.positions[robot];
  int moves = 0;
  bool isMoving = true;
//   std::cout << position.x << " " << position.y << std::endl;

  while (isMoving) {
    if (data[position.x][position.y] & move) {
      isMoving = false;
    } else {
      Position tmp = position;
      if (move == MAP_LEFT)
        tmp.x--;
      if (move == MAP_RIGHT)
        tmp.x++;
      if (move == MAP_DOWN)
        tmp.y++;
      if (move == MAP_UP)
        tmp.y--;

      for (int k = 0; k < ROBOT_COUNTS; k++) {
        if (k != robot && tmp == robots.positions[k]) {
          isMoving = false;
        }
      }

      if (isMoving) {
        position = tmp;
      }
    }
  }

  robots.positions[robot] = position;
  return position;
}

bool Position::operator==(const Position &other) {
  return x == other.x && y == other.y;
}

bool Robots::operator<(const Robots &other) {
  for (int k = 0; k < ROBOT_COUNTS; k++) {
    if (positions[k].x != other.positions[k].x) {
      return positions[k].x < other.positions[k].x;
    }

    if (positions[k].y != other.positions[k].y) {
      return positions[k].y < other.positions[k].y;
    }
  }

  return false;
}
