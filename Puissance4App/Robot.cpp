#include "Robot.hpp"

Robot::Robot()
{
	columnCoordinates[0] = { 254, -111, 88, 0 };
	columnCoordinates[1] = { 255, -77, 88, 0 };
	columnCoordinates[2] = { 256, -38, 88, 0 };
	columnCoordinates[3] = { 256, -6, 88, 0 };
	columnCoordinates[4] = { 256, 32, 88, 0 };
	columnCoordinates[5] = { 257, 66, 88, 0 };
	columnCoordinates[6] = { 257, 107, 88, 0 };

	pieceCoordinates[0] = { -58, -220, -126, -90 };
	pieceCoordinates[1] = { -13, -220, -126, -90 };
	pieceCoordinates[2] = { 28, -220, -126, -90 };
	pieceCoordinates[3] = { 75, -220, -126, -90 };
	pieceCoordinates[4] = { -50, 223, -126, 90 };
	pieceCoordinates[5] = { -11, 223, -126, 90 };
	pieceCoordinates[6] = { 33, 223, -126, 90 };
	pieceCoordinates[7] = { 78, 222, -126, 90 };
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

	int resultClearAllAlarms = ClearAllAlarmsState(dobotId);
	if (resultClearAllAlarms != DobotCommunicate_NoError) {
		std::cerr << "Failed to clear all alarms" << std::endl;
		return;
	}
	
	JOGCoordinateParams* jogCoordinateParams = new JOGCoordinateParams;
	int result = GetJOGCoordinateParams(dobotId, jogCoordinateParams);
	if (result != DobotCommunicate_NoError) {
		std::cerr << "Failed to get jog coordinate params" << std::endl;
		return;
	}
	std::cout << "JOGCoordinateParams: velocityX = " << jogCoordinateParams->velocity[0] << ", velocityY = " << jogCoordinateParams->velocity[1] << ", velocityZ = " << jogCoordinateParams->velocity[2] << ", velocityR = " << jogCoordinateParams->velocity[3] << std::endl;
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

	goTo(pieceCoordinates[0], 90);
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
	goTo(columnCoordinates[column], 90);
	goTo(columnCoordinates[column]);
	openGripper();
	wait(0.5);
	turnOffGripper();
	if (remainingPieces > 0) {
		goTo(pieceCoordinates[(8 - remainingPieces)], 90);
	}
	else {
		goTo(pieceCoordinates[0], 90);
	}
}

void Robot::goTo(Pose position)
{
	PTPCmd ptpCmd = { 0 };
	ptpCmd.ptpMode = PTPJUMPXYZMode;
	ptpCmd.x = position.x;
	ptpCmd.y = position.y;
	ptpCmd.z = position.z;
	ptpCmd.r = position.r;

	uint64_t queuedCmdIndex;
	int resultDobotPTP = SetPTPCmd(dobotId, &ptpCmd, true, &queuedCmdIndex);
	if (resultDobotPTP != DobotCommunicate_NoError) {
		std::cerr << "Failed to move dobot to position" << std::endl;
	}
}

void Robot::goTo(Pose position, float z)
{
	Pose newPose = {position.x, position.y, z, position.r};
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
	openGripper();
	goTo(pieceCoordinates[8 - remainingPieces]);
	closeGripper();
	wait(0.5);
	goTo(pieceCoordinates[8 - remainingPieces], 90);
	remainingPieces--;
}

void Robot::gripper(bool close, bool on)
{
	uint64_t queuedCmdIndex;
	int resultDobotGripper = SetEndEffectorGripper(dobotId, on, close, true, &queuedCmdIndex);
	if (resultDobotGripper != DobotCommunicate_NoError) {
		std::cerr << "Failed to close gripper" << std::endl;
	}
}


void Robot::turnOffGripper()
{
	gripper(false, false);
}

void Robot::wait(float seconds)
{
	WAITCmd waitCmd = { 0 };
	waitCmd.timeout = seconds * 1000;

	uint64_t queuedCmdIndex;
	int resultDobotWait = SetWAITCmd(dobotId, &waitCmd, true, &queuedCmdIndex);
	if (resultDobotWait != DobotCommunicate_NoError) {
		std::cerr << "Failed to wait" << std::endl;
	}
}
