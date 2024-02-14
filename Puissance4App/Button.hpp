#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button();

	sf::Vector2f getButtonSize() { return buttonSize; }

	void setButtonColor(sf::Color color);
	void setButtonSize(sf::Vector2f size);
	void setButtonPosition(sf::Vector2f position);
	void setButtonTextColor(sf::Color color);
	void setButtonText(std::string text);
	void setButtonFont(sf::Font* font);
	void setButtonTextSize(int size);
	void draw(sf::RenderWindow& window);
	bool mouseIsInsideButton(sf::Vector2f mousePosition);

private:
	sf::RectangleShape buttonRect;
	sf::Color buttonColor;
	sf::Text buttonText;
	sf::Vector2f buttonSize;
	sf::Color buttonTextColor;
	sf::Vector2f buttonPosition;
};

