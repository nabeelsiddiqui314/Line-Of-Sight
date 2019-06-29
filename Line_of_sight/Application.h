#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "TileMap.h"

class Application
{
private:
	typedef std::pair<TileMap::Pos, TileMap::Pos> Wall;
private:
	struct Cell {
		int edgeID[4];
	};
public:
	Application(int rows, int columns, int cellSize);
public:
	void update(const sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
private:
	void tileToPolygonAlgorithm();
	void LOSAlgorithm(const sf::RenderWindow& window);
private:
	int m_rows;
	int m_columns;
	int m_cellSize;
	TileMap m_tileMap;
	std::unique_ptr<Cell[]> m_cells;
	std::vector<Wall> m_walls;
	sf::VertexArray m_wallLines;
	sf::VertexArray m_light;
	bool m_didClick = false;
};