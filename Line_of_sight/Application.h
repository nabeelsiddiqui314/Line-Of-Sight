#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "TileMap.h"

class Application
{
public:
	Application(int rows, int columns, int cellSize);
public:
	void update();
	void render(sf::RenderWindow& window);
private:
	std::unique_ptr<TileMap> m_tileMap;
};

