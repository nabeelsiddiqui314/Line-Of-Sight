#pragma once
#include <SFML/Graphics.hpp>

class TileMap
{
public:
	struct Pos {
		int x, y;

		Pos() = default;
		Pos(int x_, int y_) : x(x_), y(y_) {}
	};
public:
	TileMap(int rows, int columns, int cellSize);
public:
	void clear();
	void setCellColor(const Pos& pos, const sf::Color& color);
	void render(sf::RenderWindow& window);
private:
	sf::VertexArray m_verticies;
	int m_columns;
};

