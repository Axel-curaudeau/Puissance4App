#pragma once
#include <SFML/Graphics.hpp>

class Button
{
public:
	Button();

	/// <summary>
	/// Get the button size
	/// </summary>
	/// <returns></returns>
	sf::Vector2f getButtonSize();

	/// <summary>
	/// Set the button color
	/// </summary>
	/// <param name="color">Color of the button using a Color type from SFML library</param>
	void setButtonColor(sf::Color color);

	/// <summary>
	/// Set the button size
	/// </summary>
	/// <param name="size">Size vector containing the width and height of the button</param>
	void setButtonSize(sf::Vector2f size);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
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

