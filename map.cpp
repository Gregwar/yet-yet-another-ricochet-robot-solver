#include "map.h"
#include <iostream>
#include <map>
#include <stdio.h>
#include <unordered_map>

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

/**
 * A possible move and its result
 */
struct Move {
  int robot;
  int move;
  Robots result;
};

std::unordered_map<Robots, std::vector<Move>, KeyHasher> possibleMovesCache;

std::vector<Move> possibleMoves(Map &map, Robots &robots) {

  if (!possibleMovesCache.count(robots)) {
    std::vector<Move> result;

    for (int k = 0; k < ROBOT_COUNTS; k++) {
      for (int m = 0; m < 4; m++) {
        Robots tmp = robots;
        if (map.move(tmp, k, 1 << m)) {
          Move move;
          move.robot = k;
          move.move = m;
          move.result = tmp;
          result.push_back(move);
        }
      }
    }

    return result;
    possibleMovesCache[robots] = result;
  }

  return possibleMovesCache[robots];
}

bool Robots::operator==(const Robots &other) const {
  for (int k = 0; k < ROBOT_COUNTS; k++) {
    if (positions[k].x != other.positions[k].x ||
        positions[k].y != other.positions[k].y) {
      return false;
    }
  }

  return true;
}

void printMove(int robot, int move) {
  std::cout << "Robot: ";
  if (robot == ROBOT_RED)
    std::cout << "red";
  if (robot == ROBOT_GREEN)
    std::cout << "green";
  if (robot == ROBOT_BLUE)
    std::cout << "blue";
  if (robot == ROBOT_YELLOW)
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
}

struct PreviousStateLink {
  int lastRobot;
  int lastMove;
  Robots lastRobots;
};

std::unordered_map<Robots, PreviousStateLink, KeyHasher> previousStates;

void printSolution(PreviousStateLink s) {
  if (s.lastRobot >= 0) {
    printSolution(previousStates[s.lastRobots]);
    printMove(s.lastRobot, s.lastMove);
  }
}

std::vector<Robots> solve(Map &map, int robot, std::vector<Robots> lastStage) {
  std::vector<Robots> newRobots;

  for (Robots robots : lastStage) {
    for (Move move : possibleMoves(map, robots)) {
      if (!previousStates.count(move.result)) {
        PreviousStateLink s;
        s.lastRobot = move.robot;
        s.lastMove = move.move;
        s.lastRobots = robots;
        newRobots.push_back(move.result);
        previousStates[move.result] = s;

        if (move.result.positions[robot] == map.target) {
          std::cout << "Found a solution:" << std::endl;
          printSolution(s);
          exit(0);
        }
      }
    }
  }

  return newRobots;
}

std::vector<Robots> solve(Map &map, int robot) {
  std::vector<Robots> positions;
  positions.push_back(map.initialRobots);
  PreviousStateLink s;
  s.lastRobot = -1;
  previousStates[map.initialRobots] = s;

  int moves = 1;
  while (true) {
    std::cout << "Trying for " << moves << " moves (" << positions.size() << ")"
              << std::endl;
    moves += 1;
    positions = solve(map, robot, positions);
  }
}
