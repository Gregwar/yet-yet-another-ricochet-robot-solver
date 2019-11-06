#include "map.h"
#include <iostream>
#include <map>
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

bool Map::move(Robots &robots, int robot, int move) {
  Position position = robots.positions[robot];
  bool moved = false;
  bool isMoving = true;
  // std::cout << "Move: " << move << " "  << position.x << " " << position.y <<
  // std::endl;

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
        moved = true;
        position = tmp;
      }
    }
  }

  robots.positions[robot] = position;

  return moved;
}

bool Position::operator==(const Position &other) {
  return x == other.x && y == other.y;
}

bool Robots::operator<(const Robots &other) const {
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

int soluce = 0;

// Number of moves for a given robots position
std::map<Robots, int> robotsToMove;

bool canReach(Map &map, Robots &robots, int robot, int moves, int depth) {
  if (moves > 0) {
    for (int k = 0; k < ROBOT_COUNTS; k++) {
      if (moves == 1 && k != robot && robot != ROBOT_ANY) {
        // Only one move is remaining, we can only use the final robot
        continue;
      }

      for (int move = 0; move < 4; move++) {
        Robots tmp = robots;

        if (map.move(tmp, k, 1 << move)) {
          if (robotsToMove.count(tmp) && robotsToMove[tmp] < depth) {
            continue;
          }
          robotsToMove[tmp] = depth;

          bool reached = false;
          if (robot == ROBOT_ANY) {
            reached = (tmp.positions[k] == map.target);
          } else {
            reached = (tmp.positions[robot] == map.target);
          }

          if (reached) {
              soluce++;
          }

          if ((reached || canReach(map, tmp, robot, moves - 1, depth + 1))
          && (soluce == 0)
          ) {
            std::cout << "Robot: ";
            if (k == ROBOT_RED)
              std::cout << "red";
            if (k == ROBOT_GREEN)
              std::cout << "green";
            if (k == ROBOT_BLUE)
              std::cout << "blue";
            if (k == ROBOT_YELLOW)
              std::cout << "yellow";

            std::cout << " Move: ";
            if (move == 0)
              std::cout << "left";
            if (move == 1)
              std::cout << "right";
            if (move == 2)
              std::cout << "up";
            if (move == 3)
              std::cout << "down";

            std::cout << std::endl;
            return true;
          }
        }
      }
    }
  }

  return false;
}
