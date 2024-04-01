#include "uiController.hpp"
#include <opencv2/opencv.hpp>
#include "Robot.hpp"
#include "Board.hpp"
#include "Negamax.hpp"
#include "Camera.hpp"
#include "BoardDetector.hpp"
#include "TranspositionTable.hpp"

using namespace std;

int main()
{
	sf::Vector2u windowSize(1280, 720);
	uiController uiController(windowSize);
	StateMachine stateMachine;
	Camera* camera = new Camera();
	TranspositionTable* transpositionTable = new TranspositionTable();
	Robot robot;
	int moveNumber = 0;
	bool isConnected = false;
	
	while (uiController.getWindow().isOpen())
	{
		if (!isConnected)
		{
			//TODO: à tester
			isConnected = robot.connect();
			if (isConnected)
			{
				robot.Home();
			}
		}

		StateMachine::State newState = uiController.tick(stateMachine.getState());
		if (newState != stateMachine.getState()) {
			stateMachine.ChangeState(newState);
		}

		cv::Mat frame = camera->getFrame();

		uiController.getGameUI()->getCameraFrame(frame);

		if (frame.empty())
		{
			continue;
		}

		//TODO: à tester
		Board* board = new Board();
		BoardDetector::detectBoard(frame, BoardDetector::Color::RED, board);
		if (!board->isEmpty() && board->isValid())
		{
			board->printBoard();
			uiController.getGameUI()->updateBoard(uiController.getWindow(), *board);

			if (board->getMoveNumber() == moveNumber + 1 && board->getMoveNumber() % 2 == 1)
			{
				moveNumber += 2;
				int bestMove = Negamax::GetBestMove(*board, transpositionTable, 7);
				board->Play(bestMove);
				board->printBoard();
				robot.Play(bestMove);
			}
		}
		/*
		Board board = BoardDetector::detectBoard(frame, BoardDetector::Color::RED);
		uiController.getGameUI()->getCameraFrame(frame);
		if (!board.isEmpty() && board.isValid())
		{
			board.printBoard();
			uiController.getGameUI()->updateBoard(uiController.getWindow(), board);

			if (board.getMoveNumber() == moveNumber + 1 && board.getMoveNumber() % 2 == 1)
			{
				moveNumber += 2;
				int bestMove = Negamax::GetBestMove(board, transpositionTable, 7);
				board.Play(bestMove);
				board.printBoard();
				robot.Play(bestMove);
			}
		}*/
	}
	uiController.stop(stateMachine.getState());
	return 0;
}