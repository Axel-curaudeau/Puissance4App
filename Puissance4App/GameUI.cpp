#include "GameUI.hpp"

GameUI::GameUI()
{

}

GameUI::~GameUI()
{
	if (webcamThread != nullptr)
	{
		webcamThread->join();
		delete webcamThread;
	}
}

GameUI::GameUI(sf::Font* font)
{
	backButton = Button();
	backButton.setButtonColor(sf::Color::Red);
	backButton.setButtonSize(sf::Vector2f(250, 100));
	backButton.setButtonPosition(sf::Vector2f(10, 10));
	backButton.setButtonFont(font);
	backButton.setButtonTextColor(sf::Color::White);
	backButton.setButtonText("Retour");
	backButton.setButtonTextSize(80);

	gameGrid.setFillColor(sf::Color::Blue);
}

void GameUI::draw(sf::RenderWindow& window)
{
	sf::Vector2u windowSize = window.getSize();

	gameGrid.setSize(sf::Vector2f(windowSize.y * 0.5 * 7 / 6, windowSize.y * 0.5));
	gameGrid.setPosition(sf::Vector2f(windowSize.x / 2 - gameGrid.getSize().x / 2, windowSize.y - gameGrid.getSize().y));
	
	webcamSprite.setPosition(sf::Vector2f(windowSize.x / 2 - webcamSprite.getGlobalBounds().width / 2 - 10, 10));
	
	window.draw(gameGrid);
	backButton.draw(window);
	
	if (webcamThread == nullptr)
	{
		webcamThread = new std::thread(&GameUI::getWebcamImage, this);
	}
	else 
	{
		window.draw(webcamSprite);
	}
	
}

void GameUI::getWebcamImage()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
		return;

	while (true)
	{
		cv::Mat frame;
		cap >> frame;
		cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
		
		sf::Uint8* pixels = new sf::Uint8[frame.cols * frame.rows * 4];
		for (int i = 0; i < frame.cols * frame.rows; i++)
		{
			pixels[i * 4] = frame.data[i * 3];
			pixels[i * 4 + 1] = frame.data[i * 3 + 1];
			pixels[i * 4 + 2] = frame.data[i * 3 + 2];
			pixels[i * 4 + 3] = 255;
		}

		webcamImage.create(frame.cols, frame.rows, pixels);
		webcamTexture.loadFromImage(webcamImage);
		webcamSprite.setTexture(webcamTexture);
	}
}

StateMachine::State GameUI::handleEvent(sf::Event event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (backButton.mouseIsInsideButton(sf::Vector2f(float(event.mouseButton.x), float(event.mouseButton.y))))
		{
			std::cout << "Back button pressed" << std::endl;
			backButton.setButtonColor(sf::Color::Red);
			return StateMachine::State::MainMenu;
		}
	}
	if (event.type == sf::Event::MouseMoved)
	{
		if (backButton.mouseIsInsideButton(sf::Vector2f(float(event.mouseMove.x), float(event.mouseMove.y))))
		{
			backButton.setButtonColor(sf::Color::Green);
		}
		else
		{
			backButton.setButtonColor(sf::Color::Red);
		}
	}
	return StateMachine::State::Game;
}
