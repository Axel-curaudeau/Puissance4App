#include "uiController.hpp"
#include <opencv2/opencv.hpp>
#include "Robot.hpp"
#include "Board.hpp"
#include "Negamax.hpp"

using namespace std;

int main()
{
	/*Robot robot;
	robot.connect();
	robot.Home();
	robot.Play(0);*/

	//Test Board
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
	


	/*
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
	*/
}