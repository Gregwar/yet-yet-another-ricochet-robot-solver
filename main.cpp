#include <iostream>
#include <stdio.h>
#include "robot_ricochet.h"

int main()
{
    RobotRicochet::Map map;

    map.readFromFile("map.bin");
    map.print();
    RobotRicochet::Robots robots = map.initialRobots;

    solve(map, ROBOT_GREEN);
}