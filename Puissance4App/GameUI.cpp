#include "GameUI.hpp"


GameUI::GameUI()
{
}

GameUI::~GameUI()
{
	if (webcamThread != nullptr)
	{
		webcamThreadRunning = false;
		webcamThread->join();
		delete webcamThread;
		webcamThread = nullptr;
	}
	delete webcamImage;
	delete webcamTexture;
	delete webcamSprite;
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

	webcamImage = new sf::Image();
	webcamTexture = new sf::Texture();
	webcamSprite = new sf::Sprite();
}

void GameUI::draw(sf::RenderWindow& window)
{
	sf::Vector2u windowSize = window.getSize();

	gameGrid.setSize(sf::Vector2f(windowSize.y * 0.5 * 7 / 6, windowSize.y * 0.5));
	gameGrid.setPosition(sf::Vector2f(windowSize.x / 2 - gameGrid.getSize().x / 2, windowSize.y - gameGrid.getSize().y));
	
	webcamSprite->setPosition(sf::Vector2f(windowSize.x / 2 - webcamSprite->getGlobalBounds().width / 2 - 10, 10));
	
	window.draw(gameGrid);
	backButton.draw(window);
	
	window.draw(*webcamSprite);
}

void GameUI::getCameraFrame(cv::Mat frame)
{
	if (frame.empty())
	{
		return;
	}
	int frameWidth = frame.cols;
	int frameHeight = frame.rows;
	sf::Uint8* pixels = new sf::Uint8[4 * frameWidth * frameHeight];
	for (int i = 0; i < frameWidth; i++)
	{
		for (int j = 0; j < frameHeight; j++)
		{
			cv::Vec3b color = frame.at<cv::Vec3b>(j, i);
			pixels[(i + j * frameWidth) * 4] = color[2];
			pixels[(i + j * frameWidth) * 4 + 1] = color[1];
			pixels[(i + j * frameWidth) * 4 + 2] = color[0];
			pixels[(i + j * frameWidth) * 4 + 3] = 255;
		}
	}
	webcamImage->create(frameWidth, frameHeight, pixels);
	delete[] pixels;
	webcamTexture->loadFromImage(*webcamImage);
	webcamSprite->setTexture(*webcamTexture);
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
