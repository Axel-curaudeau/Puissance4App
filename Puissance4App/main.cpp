#include "uiController.hpp"
#include <opencv2/opencv.hpp>
#include "Robot.hpp"
#include "Board.hpp"
#include "Negamax.hpp"
#include "Camera.hpp"
#include "BoardDetection.hpp"

using namespace std;

int main()
{
	/*
	Robot robot;
	robot.connect();
	robot.Home();
	robot.Play(3);
	*/

	//Test Board
	/*
	try {
		Board board;
		board.Play(0);
		board.printBoard();
		cout << "Starting negamax" << endl;
		int bestMove = Negamax::GetBestMove(board);
		cout << "Best move: " << bestMove << endl;
		board.Play(bestMove);
		board.printBoard();
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	*/
	
	
	sf::Vector2u windowSize(1280, 720);
	uiController uiController(windowSize);
	StateMachine stateMachine;
	Camera* camera = new Camera();

	while (uiController.getWindow().isOpen())
	{
		StateMachine::State newState = uiController.tick(stateMachine.getState());
		if (newState != stateMachine.getState()) {
			stateMachine.ChangeState(newState);
		}

		uiController.getGameUI()->getImageFromCamera(camera);

		cv::Mat frame = camera->getFrame();
		std::vector<cv::Vec3f> circles = BoardDetection::detectCircle(frame);
		cv::Vec3f firstCircle = BoardDetection::searchFirstCircle(frame, circles);
	}
	uiController.stop(stateMachine.getState());
	return 0;
	
}