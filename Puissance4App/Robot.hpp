#pragma once

#include "DobotDll.h"
#include "DobotType.h"

class Robot
{
public:
	Robot();
	void Home();
	void Play(int column);
private:
	int dobotId;
	int remainingPieces = 8;
	Pose columnCoordinates[7];
	void GoTo(int x, int y);
	void openGripper();
	void CloseGripper();
};

