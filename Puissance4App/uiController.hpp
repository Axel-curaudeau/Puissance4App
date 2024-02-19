#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.hpp"
#include "StateMachine.hpp"
#include <iostream>
#include "GameUI.hpp"

class uiController {
public:
	uiController(sf::Vector2u windowSize);
	StateMachine::State tick(StateMachine::State actualState);
	sf::RenderWindow& getWindow() { return window; }
	sf::Font* font;
	void stop(StateMachine::State actualState);

private:
	sf::RenderWindow window;
	std::string fontPath = ".\\Ressources\\Hanged_Letters.ttf";
	MainMenu mainMenu;
	GameUI game;
};