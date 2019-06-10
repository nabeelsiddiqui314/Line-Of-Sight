#pragma once
#include <SFML/Graphics.hpp>

class Application
{
public:
	Application() = default;
public:
	void update();
	void render(sf::RenderWindow& window);
};

