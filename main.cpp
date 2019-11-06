#include <iostream>
#include <stdio.h>
#include "map.h"

int main()
{
    Map map;

    map.readFromFile("map.bin");
    map.print();
    Robots robots = map.initialRobots;

    // map.move(robots, ROBOT_BLUE, MAP_RIGHT);
    // map.move(robots, ROBOT_GREEN, MAP_DOWN);
    // map.move(robots, ROBOT_GREEN, MAP_RIGHT);
    // map.move(robots, ROBOT_BLUE, MAP_UP);
    // map.move(robots, ROBOT_BLUE, MAP_RIGHT);

    // if (robots.positions[ROBOT_BLUE] == map.target) {
    //     std::cout << "REACHED!" << std::endl;
    // }

    for (int moves=1; moves<15; moves++) {
        std::cout << "Trying with " << moves << " moves..." << std::endl;
        if (!canReach(map, robots, ROBOT_YELLOW, moves)) {
            std::cout << "No solution!" << std::endl;
        } else {
            break;
        }
    }
}