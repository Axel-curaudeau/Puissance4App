#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include "Button.hpp"
#include "StateMachine.hpp"

static bool webcamThreadRunning = true;

class GameUI
{
public:
	GameUI();
	~GameUI();
	GameUI(sf::Font* font);
	void draw(sf::RenderWindow& window);
	void getWebcamImage();
	StateMachine::State handleEvent(sf::Event event);

private:
	Button backButton;

	std::thread* webcamThread = nullptr;

	sf::RectangleShape gameGrid;
	sf::Image webcamImage;
	sf::Texture webcamTexture;
	sf::Sprite webcamSprite;

};

