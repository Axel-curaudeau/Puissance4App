#include "uiController.hpp"
#include <opencv2/opencv.hpp>

#include "Robot.hpp"

using namespace cv;
using namespace std;

int main()
{
	Robot robot = Robot();
	robot.Home();

	sf::Vector2u windowSize(1280, 720);
	uiController p4ui(windowSize);
	StateMachine stateMachine;

	while (p4ui.getWindow().isOpen())
	{
		StateMachine::State newState = p4ui.tick(stateMachine.getState());
		if (newState != stateMachine.getState()) {
			stateMachine.ChangeState(newState);
		}
	}
	return 0;
}