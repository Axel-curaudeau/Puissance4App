#include "Robot.hpp"
#include <iostream>

Robot::Robot()
{
	char dobotPortName[64];
	int dobotSearchCount = SearchDobot(dobotPortName, 64);

	if (dobotSearchCount == 0) {
		std::cerr << "No dobot found" << std::endl;
		return;
	}
	else if (dobotSearchCount > 1) {
		std::cerr << "Multiple dobot found" << std::endl;
		return;
	}

	int connectDobotResult = ConnectDobot(dobotPortName, 115200, nullptr, nullptr, &dobotId);
	if (connectDobotResult != DobotConnect_NoError) {
		std::cerr << "Failed to connect to dobot. Error : ";
		switch (connectDobotResult) {
			case DobotConnect_NotFound:
				std::cerr << "Dobot not found";
				break;
			case DobotConnect_Occupied:
				std::cerr << "Dobot port occupied";
				break;
		}
		return;
	}

	std::cout << "Connected to dobot" << std::endl;
}

void Robot::Home()
{
	std::cout << "Resetting dobot position" << std::endl;
	HOMECmd homeCmd;
	uint64_t queuedCmdIndex;
	int resultDobotHome = SetHOMECmd(dobotId, &homeCmd, false, &queuedCmdIndex);
	if (resultDobotHome != DobotCommunicate_NoError) {
		std::cerr << "Failed to reset dobot position" << std::endl;
		return;
	}
	std::cout << "Dobot position successfully reset" << std::endl;
}
