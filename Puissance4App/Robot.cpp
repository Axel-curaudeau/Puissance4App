#include "Robot.hpp"
#include <iostream>

Robot::Robot()
{
	char dobotPortName[64];
	int dobotSearchCount = SearchDobot(dobotPortName, 64);

	std::cout << "Dobot count: " << dobotSearchCount << std::endl;

	if (dobotSearchCount == 0) {
		std::cerr << "No dobot found" << std::endl;
		return;
	}
	else if (dobotSearchCount > 1) {
		std::cerr << "Multiple dobot found" << std::endl;
		return;
	}

	int dobotId;
	if (ConnectDobot(dobotPortName, 115200, nullptr, nullptr, &dobotId) != DobotConnect_NoError) {
		std::cerr << "Failed to connect to dobot" << std::endl;
		return;
	}

	std::cout << "Connected to dobot" << std::endl;
}