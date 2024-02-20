#include "Robot.hpp"
#include <iostream>

Robot::Robot()
{
	columnCoordinates[0] = { 252, -111, 88, 0 };
	columnCoordinates[1] = { 253, -79, 88, 0 };
	columnCoordinates[2] = { 254, -38, 88, 0 };
	columnCoordinates[3] = { 254, -6, 88, 0 };
	columnCoordinates[4] = { 255, 32, 88, 0 };
	columnCoordinates[5] = { 255, 66, 88, 0 };
	columnCoordinates[6] = { 256, 107, 88, 0 };

	pieceCoordinates[0] = { -58, -220, -124, -90 };
	pieceCoordinates[1] = { -13, -220, 200, -90 };
	pieceCoordinates[2] = { 28, -220, 200, -90 };
	pieceCoordinates[3] = { 75, -220, 200, -90 };
	pieceCoordinates[4] = { -50, 226, 200, -90 };
	pieceCoordinates[5] = { -11, 226, 200, -90 };
	pieceCoordinates[6] = { 33, 226, 200, -90 };
	pieceCoordinates[7] = { 78, 226, 200, -90 };
}

Robot::~Robot()
{
}

void Robot::connect()
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
	int resultDobotHome = SetHOMECmd(dobotId, &homeCmd, true, &queuedCmdIndex);
	if (resultDobotHome != DobotCommunicate_NoError) {
		std::cerr << "Failed to reset dobot position" << std::endl;
		return;
	}
	std::cout << "Dobot position successfully reset" << std::endl;
}

void Robot::Play(int column)
{
	if (remainingPieces == 0) {
		std::cerr << "No more pieces" << std::endl;
		return;
	}

	if (column < 0 || column > 6) {
		std::cerr << "Invalid column" << std::endl;
		return;
	}

	std::cout << "Playing in column " << column << std::endl;
	grabPiece();
	goTo(columnCoordinates[column]);
	openGripper();
	turnOffGripper(); 
}

void Robot::goTo(Pose position)
{
	PTPCmd ptpCmd = { 0 };
	ptpCmd.ptpMode = PTPMOVJXYZMode;
	ptpCmd.x = position.x;
	ptpCmd.y = position.y;
	ptpCmd.z = position.z;
	ptpCmd.r = position.r;

	uint64_t queuedCmdIndex;
	int resultDobotPTP = SetPTPCmd(dobotId, &ptpCmd, true, &queuedCmdIndex);
	if (resultDobotPTP != DobotCommunicate_NoError) {
		std::cerr << "Failed to move dobot to position" << std::endl;
		return;
	}
}

void Robot::goTo(Pose position, float zOffset)
{
	Pose newPose = {position.x, position.y, position.z + zOffset, position.r};
	goTo(newPose);
}

void Robot::openGripper()
{
	gripper(false, true);
}

void Robot::closeGripper()
{
	gripper(true, true);
}


void Robot::grabPiece()
{
	goTo(pieceCoordinates[8 - remainingPieces], 200);
	openGripper();
	goTo(pieceCoordinates[8 - remainingPieces]);
	closeGripper();
	goTo(pieceCoordinates[8 - remainingPieces], 200);
	remainingPieces--;
}

void Robot::gripper(bool close, bool on)
{
	uint64_t queuedCmdIndex;
	int resultDobotGripper = SetEndEffectorGripper(dobotId, on, close, true, &queuedCmdIndex);
	if (resultDobotGripper != DobotCommunicate_NoError) {
		std::cerr << "Failed to close gripper" << std::endl;
		return;
	}
}


void Robot::turnOffGripper()
{
	gripper(false, false);
}