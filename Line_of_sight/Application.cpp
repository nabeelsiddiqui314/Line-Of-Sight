#include "stdafx.h"
#include "Application.h"

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

Application::Application(int rows, int columns, int cellSize)
	: m_rows(rows),
	m_columns(columns),
	m_cellSize(cellSize), 
	m_tileMap(rows, columns, cellSize) {
	m_cells = std::make_unique<Cell[]>(rows * columns);
	m_wallLines.setPrimitiveType(sf::PrimitiveType::Lines);
}

void Application::update(const sf::RenderWindow& window) {
	if (sf::IntRect(window.getPosition().x, window.getPosition().y, window.getSize().x, window.getSize().y).contains(sf::Mouse::getPosition()) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_didClick) {
		m_tileMap.setCellColor(TileMap::Pos(floor(sf::Mouse::getPosition(window).x / m_cellSize), floor(sf::Mouse::getPosition(window).y / m_cellSize)), sf::Color::Blue);
		this->tileToPolygonAlgorithm();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		m_tileMap.clear();
		m_walls.clear();
	}
	m_didClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	m_wallLines.resize(m_walls.size() * 2);
	for (std::size_t i = 0; i < m_wallLines.getVertexCount(); i+=2) {
		m_wallLines[i].position = sf::Vector2f(m_walls[i / 2].first.x, m_walls[i / 2].first.y);
		m_wallLines[i + 1].position = sf::Vector2f(m_walls[i / 2].second.x, m_walls[i / 2].second.y);
		m_wallLines[i].color = sf::Color::Black;
		m_wallLines[i + 1].color = sf::Color::Black;
	}
}

void Application::render(sf::RenderWindow& window) {
	m_tileMap.render(window);
	window.draw(m_wallLines);
}

void Application::tileToPolygonAlgorithm() {
	m_walls.clear();
	for (std::size_t x = 0; x < m_columns; x++) {
		for (std::size_t y = 0; y < m_rows; y++) {
			for (std::size_t i = 0; i < 4; i++)
				m_cells[x + y * m_columns].edgeID[i] = -1;
		}
	}

	auto hasObstacle = [&](const TileMap::Pos& pos) {
		if (pos.x > 0 && pos.y > 0 && pos.x < m_columns &&  pos.y < m_rows)
			return m_tileMap.getCellColor(pos) != sf::Color::White;
		else
			return false;
	};

	for (int x = 0; x < m_columns; x++) {
		for (int y = 0; y < m_rows; y++) {
			if (hasObstacle({ x, y })) {
				if (!hasObstacle({ x - 1, y })) {
					if (hasObstacle({ x, y - 1 }) && m_cells[x + (y - 1) * m_columns].edgeID[WEST] != -1) {
						m_walls[m_cells[x + (y - 1) * m_columns].edgeID[WEST]].second.y += m_cellSize;
						m_cells[x + y * m_columns].edgeID[WEST] = m_cells[x + (y - 1) * m_columns].edgeID[WEST];
					}
					else {
						m_cells[x + y * m_columns].edgeID[WEST] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos(x * m_cellSize, y * m_cellSize),
							TileMap::Pos(x * m_cellSize, (y + 1) * m_cellSize)));
					}
				}


				if (!hasObstacle({ x, y - 1 })) {
					if (hasObstacle({ x - 1, y }) && m_cells[(x - 1) + y * m_columns].edgeID[NORTH] != -1) {
						m_walls[m_cells[(x - 1) + y * m_columns].edgeID[NORTH]].second.x += m_cellSize;
						m_cells[x + y * m_columns].edgeID[NORTH] = m_cells[(x - 1) + y * m_columns].edgeID[NORTH];
					}
					else {
						m_cells[x + y * m_columns].edgeID[NORTH] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos(x * m_cellSize, y * m_cellSize),
							TileMap::Pos((x + 1) * m_cellSize, y * m_cellSize)));
					}
				}

				if (!hasObstacle({ x + 1, y })) {
					if (hasObstacle({ x, y - 1 }) && m_cells[x + (y - 1) * m_columns].edgeID[EAST] != -1) {
						m_walls[m_cells[x + (y - 1) * m_columns].edgeID[EAST]].second.y += m_cellSize;
						m_cells[x + y * m_columns].edgeID[EAST] = m_cells[x + (y - 1) * m_columns].edgeID[EAST];
					}
					else {
						m_cells[x + y * m_columns].edgeID[EAST] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos((x + 1) * m_cellSize, y * m_cellSize),
							TileMap::Pos((x + 1) * m_cellSize, (y + 1) * m_cellSize)));
					}
				}

				if (!hasObstacle({ x, y + 1 })) {
					if (hasObstacle({ x - 1, y }) && m_cells[(x - 1) + y * m_columns].edgeID[SOUTH] != -1) {
						m_walls[m_cells[(x - 1) + y * m_columns].edgeID[SOUTH]].second.x += m_cellSize;
						m_cells[x + y * m_columns].edgeID[SOUTH] = m_cells[(x - 1) + y * m_columns].edgeID[SOUTH];
					}
					else {
						m_cells[x + y * m_columns].edgeID[SOUTH] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos(x * m_cellSize, (y + 1) * m_cellSize),
							TileMap::Pos((x + 1) * m_cellSize, (y + 1) * m_cellSize)));
					}
				}
			}
		}
	}
}
