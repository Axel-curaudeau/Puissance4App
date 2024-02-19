#include "uiController.hpp"
#include <opencv2/opencv.hpp>
#include "Robot.hpp"

using namespace cv;
using namespace std;

int main()
{
	Robot robot;
	robot.Home();

	sf::Vector2u windowSize(1280, 720);
	uiController uiController(windowSize);
	StateMachine stateMachine;

	while (uiController.getWindow().isOpen())
	{
		StateMachine::State newState = uiController.tick(stateMachine.getState());
		if (newState != stateMachine.getState()) {
			stateMachine.ChangeState(newState);
		}
	}
	uiController.stop(stateMachine.getState());
	return 0;
}