#include "stdafx.h"
#include "TileMap.h"


TileMap::TileMap(int rows, int columns, int cellSize) 
	: m_columns(columns) {
	m_verticies.setPrimitiveType(sf::Quads);
	m_verticies.resize(rows * columns * 4);

	for (std::size_t x = 0; x < columns; x++) {
		for (std::size_t y = 0; y < rows; y++) {
			std::size_t index = (x + y * columns) * 4;
			sf::Vector2f pos(x * cellSize, y * cellSize);

			m_verticies[index].position = {pos.x, pos.y};
			m_verticies[index + 1].position = {pos.x + cellSize, pos.y};
			m_verticies[index + 2].position = {pos.x + cellSize, pos.y + cellSize};
			m_verticies[index + 3].position = {pos.x, pos.y + cellSize};

			this->clear();
		}
	}
}

void TileMap::clear() { 
	for (std::size_t i = 0; i < m_verticies.getVertexCount(); i++) {
		m_verticies[i].color = sf::Color::White;
	}
}

void TileMap::setCellColor(const Pos& pos, const sf::Color& color) {
	for (std::size_t i = 0; i < 4; i++) {
		m_verticies[(pos.x + pos.y * m_columns) * 4 + i].color = color;
	}
}

void TileMap::render(sf::RenderWindow& window) {
	window.draw(m_verticies);
}
