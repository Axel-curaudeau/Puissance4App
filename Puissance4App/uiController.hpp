#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "StateMachine.hpp"
#include <iostream>
#include "GameUI.hpp"

/// <summary>
/// This class is the controller of the UI, it will manage the window and the different states of the game (main menu, game, etc)
/// </summary>
class uiController {
public:
	uiController(sf::Vector2u windowSize);
	StateMachine::State tick(StateMachine::State actualState);
	sf::RenderWindow& getWindow();
	sf::Font* font;
	void stop(StateMachine::State actualState);
	GameUI* getGameUI();

private:
	sf::RenderWindow window;
	std::string fontPath = ".\\Ressources\\Hanged_Letters.ttf";
	MainMenu* mainMenu;
	GameUI* gameUI;
};