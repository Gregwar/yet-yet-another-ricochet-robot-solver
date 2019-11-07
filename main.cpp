#include <iostream>
#include <stdio.h>
#include "robot_ricochet.h"

int main()
{
    RobotRicochet::Map map;
    map.readFromFile("map.bin");
    RobotRicochet::Robots robots = map.initialRobots;

    RobotRicochet::solve(map, ROBOT_GREEN);
}