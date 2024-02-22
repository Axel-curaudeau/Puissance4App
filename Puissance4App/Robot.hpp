#pragma once

#include "DobotDll.h"
#include "DobotType.h"
#include <iostream>

class Robot
{
public:
	Robot();
	~Robot();
	void connect();
	void Home();
	void Play(int column);
private:
	int dobotId = -1;
	int remainingPieces = 8;
	Pose columnCoordinates[7];
	Pose pieceCoordinates[8];
	void goTo(Pose position);
	void goTo(Pose position, float z);
	void openGripper();
	void closeGripper();
	void grabPiece();
	void gripper(bool open, bool on);
	void turnOffGripper();
	void wait(float seconds);
};

