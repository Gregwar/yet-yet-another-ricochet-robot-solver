#include <iostream>
#include <stdio.h>
#include "ricochet_robot.h"

int main()
{
    RicochetRobot::Map map;
    map.readFromFile("map.bin");
    RicochetRobot::Robots robots = map.initialRobots;

    RicochetRobot::solve(map, ROBOT_GREEN);
}