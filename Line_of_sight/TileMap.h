#pragma once
#include <SFML/Graphics.hpp>

class TileMap
{
public:
	struct Pos {
		float x, y;

		Pos() = default;
		Pos(float x_, float y_) : x(x_), y(y_) {}

		bool operator==(const TileMap::Pos& other) const {
			return other.x == x && other.y == y;
		}
		 operator sf::Vector2f() const {
			 return {x, y};
		}
	};
public:
	TileMap(int rows, int columns, int cellSize);
public:
	void clear();
	void setCellColor(const Pos& pos, const sf::Color& color);
	void render(sf::RenderWindow& window);

	const sf::Color& getCellColor(const Pos& pos) const;
private:
	sf::VertexArray m_verticies;
	int m_rows;
	int m_columns;
};

