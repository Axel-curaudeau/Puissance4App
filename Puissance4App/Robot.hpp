#pragma once

#include "DobotDll.h"
#include "DobotType.h"

class Robot
{
public:
	Robot();
	void GoTo(int x, int y);
	void Home();
	void Close();
private:
	int dobotId;
};

