#include "uiController.hpp"
#include <opencv2/opencv.hpp>
#include "Robot.hpp"
#include "Board.hpp"
#include "Negamax.hpp"
#include "Camera.hpp"
#include "BoardDetection.hpp"
#include "TranspositionTable.hpp"

using namespace std;

int main()
{
	//Robot robot;
	//robot.connect();
	//robot.Home();

	//Test Board
	/*
	try {
		
		std::vector<int> moves = {
			0, 1, 2, 3, 4, 5, 6,
			0, 1, 2, 3, 4, 5, 6,
			0, 1, 2, 3, 4, 5, 6,
			1, 0, 3, 2, 5, 4, 6,
			0, 1, 2, 3, 4, 5, 6,
			0, 1, 2, 3, 4, 5, 6 };
		TranspositionTable* transpositionTable = new TranspositionTable();
		for (int i = 20; i >= 0; i--)
		{
			Board board;
			for (int j = 0; j < i; j++)
			{
				board.Play(moves[j]);
			}
			board.printBoard();
			cout << "Starting negamax with depth = " << i << endl;
			int time = clock();
			int bestMove = Negamax::GetBestMove(board, transpositionTable, i);
			cout << "Best move: " << bestMove << "found in " << (clock() - time) / CLOCKS_PER_SEC << " seconds" << endl;
			//board.Play(bestMove);
			//board.printBoard();
		}
		
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	*/

	TranspositionTable* transpositionTable = new TranspositionTable();
	Board board;
	board.Play(0); board.Play(1);
	board.Play(0); board.Play(1);
	board.Play(0); board.Play(1);
	board.Play(2); board.Play(3);
	board.Play(2); board.Play(3);
	board.Play(2); board.Play(3);
	board.Play(4); board.Play(5);
	board.Play(4); board.Play(5);
	board.Play(4); board.Play(5);
	board.Play(6); board.Play(0);
	board.Play(6); board.Play(0);
	board.Play(6); board.Play(0);
	board.Play(1); board.Play(2);
	board.Play(1); board.Play(4);
	board.Play(3); board.Play(6);
	int value = Negamax::Evaluate(board);
	cout << "Value: " << value << endl;
	board.printBoard();
	cout << "Starting negamax" << endl;
	int time = clock();
	int bestMove = Negamax::GetBestMove(board, transpositionTable, 100);
	cout << "Best move: " << bestMove << " found in " << (clock() - time) / CLOCKS_PER_SEC << " seconds" << endl;
	
	
	
	
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

		//Debugging BoardDetection
		cv::Mat frame = camera->getFrame();

		uiController.getGameUI()->getCameraFrame(frame);

		if (frame.empty())
		{
			continue;
		}

		std::vector<cv::Vec3f> circles = BoardDetection::detectCircle(frame);
		cv::Vec3f firstCircle = BoardDetection::searchFirstCircle(frame, circles, BoardDetection::Color::RED);
		std::vector<cv::Vec3f> boardCircles = BoardDetection::filterCircles(frame, circles, firstCircle);
		std::vector<cv::Vec3f> sortedCircles = BoardDetection::sortCircles(boardCircles);

		if (sortedCircles.size() != 42)
		{
			continue;
		}

		cv::Mat debugFrame = frame.clone();

		/*
		for (int i = 0; i < 7; i++)
		{
			cv::circle(debugFrame, cv::Point(sortedCircles[i * 6][0], sortedCircles[i * 6][1]), sortedCircles[i * 6][2], cv::Scalar(0, 255, 0), 2);
			cv::circle(debugFrame, cv::Point(sortedCircles[(i * 6) + 1][0], sortedCircles[(i * 6) + 1][1]), sortedCircles[(i * 6) + 1][2], cv::Scalar(255, 0, 0), 2);
			cv::circle(debugFrame, cv::Point(sortedCircles[(i * 6) + 2][0], sortedCircles[(i * 6) + 2][1]), sortedCircles[(i * 6) + 2][2], cv::Scalar(0, 0, 255), 2);
			cv::circle(debugFrame, cv::Point(sortedCircles[(i * 6) + 3][0], sortedCircles[(i * 6) + 3][1]), sortedCircles[(i * 6) + 3][2], cv::Scalar(255, 255, 0), 2);
			cv::circle(debugFrame, cv::Point(sortedCircles[(i * 6) + 4][0], sortedCircles[(i * 6) + 4][1]), sortedCircles[(i * 6) + 4][2], cv::Scalar(255, 0, 255), 2);
			cv::circle(debugFrame, cv::Point(sortedCircles[(i * 6) + 5][0], sortedCircles[(i * 6) + 5][1]), sortedCircles[(i * 6) + 5][2], cv::Scalar(0, 255, 255), 2);
		}
		*/

		Board board = BoardDetection::detectColors(frame, sortedCircles);
		if (board.isEmpty())
		{
			continue;
		}
		board.printBoard();

		uiController.getGameUI()->getCameraFrame(debugFrame);
		
	}
	uiController.stop(stateMachine.getState());
	return 0;
	
}