#include "Robot.hpp"
#include <iostream>

Robot::Robot()
{
	char dobotId[64];
	int dobotSearchCount = SearchDobot(dobotId, 64);

	std::cout << "Dobot count: " << dobotSearchCount << std::endl;

	for (int i = 0; i < dobotSearchCount; i++) {
		std::cout << "Dobot ID: " << dobotId << std::endl;
	}

}