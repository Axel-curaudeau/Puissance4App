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

		//Debugging BoardDetection
		cv::Mat frame = camera->getFrame();
		if (frame.empty())
		{
			continue;
		}
		std::vector<cv::Vec3f> circles = BoardDetection::detectCircle(frame);
		cv::Vec3f firstCircle = BoardDetection::searchFirstCircle(frame, circles, cv::Vec3b(0, 255, 255));
		std::vector<cv::Vec3f> boardCircles = BoardDetection::filterCircles(frame, circles, firstCircle);
		std::vector<cv::Vec3f> sortedCircles = BoardDetection::sortCircles(boardCircles);

		if (sortedCircles.size() != 42)
		{
			continue;
		}

		for (int i = 0; i < sortedCircles.size(); i++)
		{
			cv::circle(frame, cv::Point(sortedCircles[i][0], sortedCircles[i][1]), sortedCircles[i][2], cv::Scalar(0, 255 * i / 42, 0), 2);
		}
		cv::imshow("Circles", frame);

		Board board = BoardDetection::detectColors(frame, sortedCircles, cv::Vec3b(0, 0, 255), cv::Vec3b(0, 255, 255));
		if (board.isEmpty())
		{
			continue;
		}
		board.printBoard();
	}
	uiController.stop(stateMachine.getState());
	return 0;
	
}