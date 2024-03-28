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

	bool isConnected = robot.connect();
	while (!isConnected)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		isConnected = robot.connect();
	}
	robot.Home();
	
	while (uiController.getWindow().isOpen())
	{
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

		Board board = BoardDetector::detectBoard(frame, BoardDetector::Color::RED);
		uiController.getGameUI()->getCameraFrame(frame);
		if (!board.isEmpty())
		{
			board.printBoard();
			if (board.getMoveNumber() == moveNumber + 1 && board.getMoveNumber() % 2 == 1)
			{
				moveNumber += 2;
				int bestMove = Negamax::GetBestMove(board, transpositionTable, 8);
				board.Play(bestMove);
				board.printBoard();
				robot.Play(bestMove);
			}
		}
	}
	uiController.stop(stateMachine.getState());
	return 0;
}