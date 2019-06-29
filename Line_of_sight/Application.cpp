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
	m_light.setPrimitiveType(sf::PrimitiveType::TriangleFan);

	for (std::size_t x = 1; x < m_columns - 1 ; x++) {
		m_tileMap.setCellColor(TileMap::Pos(x, 1), sf::Color::Green);
		m_tileMap.setCellColor(TileMap::Pos(x, m_rows - 2), sf::Color::Green);
	}
	for (std::size_t y = 1; y < m_rows - 1; y++) {
		m_tileMap.setCellColor(TileMap::Pos(1, y), sf::Color::Green);
		m_tileMap.setCellColor(TileMap::Pos(m_columns - 2, y), sf::Color::Green);
	}
}

void Application::update(const sf::RenderWindow& window) {
	if (sf::IntRect(window.getPosition().x, window.getPosition().y, window.getSize().x, window.getSize().y).contains(sf::Mouse::getPosition()) && sf::Mouse::isButtonPressed(sf::Mouse::Left) && !m_didClick) {
		m_tileMap.setCellColor(TileMap::Pos(floor(sf::Mouse::getPosition(window).x / m_cellSize), floor(sf::Mouse::getPosition(window).y / m_cellSize)), sf::Color::Green);
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
	this->LOSAlgorithm(window);
}

void Application::render(sf::RenderWindow& window) {
	m_tileMap.render(window);
	window.draw(m_wallLines);
	window.draw(m_light);
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
			if (hasObstacle(TileMap::Pos (x, y))) {
				if (!hasObstacle(TileMap::Pos(x - 1, y ))) {
					if (hasObstacle(TileMap::Pos(x, y - 1 )) && m_cells[x + (y - 1) * m_columns].edgeID[WEST] != -1) {
						m_walls[m_cells[x + (y - 1) * m_columns].edgeID[WEST]].second.y += m_cellSize;
						m_cells[x + y * m_columns].edgeID[WEST] = m_cells[x + (y - 1) * m_columns].edgeID[WEST];
					}
					else {
						m_cells[x + y * m_columns].edgeID[WEST] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos(x * m_cellSize, y * m_cellSize),
							TileMap::Pos(x * m_cellSize, (y + 1) * m_cellSize)));
					}
				}


				if (!hasObstacle(TileMap::Pos(x, y - 1 ))) {
					if (hasObstacle(TileMap::Pos(x - 1, y )) && m_cells[(x - 1) + y * m_columns].edgeID[NORTH] != -1) {
						m_walls[m_cells[(x - 1) + y * m_columns].edgeID[NORTH]].second.x += m_cellSize;
						m_cells[x + y * m_columns].edgeID[NORTH] = m_cells[(x - 1) + y * m_columns].edgeID[NORTH];
					}
					else {
						m_cells[x + y * m_columns].edgeID[NORTH] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos(x * m_cellSize, y * m_cellSize),
							TileMap::Pos((x + 1) * m_cellSize, y * m_cellSize)));
					}
				}

				if (!hasObstacle(TileMap::Pos(x + 1, y ))) {
					if (hasObstacle(TileMap::Pos(x, y - 1 )) && m_cells[x + (y - 1) * m_columns].edgeID[EAST] != -1) {
						m_walls[m_cells[x + (y - 1) * m_columns].edgeID[EAST]].second.y += m_cellSize;
						m_cells[x + y * m_columns].edgeID[EAST] = m_cells[x + (y - 1) * m_columns].edgeID[EAST];
					}
					else {
						m_cells[x + y * m_columns].edgeID[EAST] = m_walls.size();
						m_walls.emplace_back(std::make_pair(TileMap::Pos((x + 1) * m_cellSize, y * m_cellSize),
							TileMap::Pos((x + 1) * m_cellSize, (y + 1) * m_cellSize)));
					}
				}

				if (!hasObstacle(TileMap::Pos(x, y + 1 ))) {
					if (hasObstacle(TileMap::Pos(x - 1, y )) && m_cells[(x - 1) + y * m_columns].edgeID[SOUTH] != -1) {
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

void Application::LOSAlgorithm(const sf::RenderWindow& window) {
	std::vector<TileMap::Pos> points;
	for (std::size_t i = 0; i < m_walls.size(); i++) {
		points.emplace_back(m_walls[i].first);
		points.emplace_back(m_walls[i].second);
	}

	//remove duplicates
	std::sort(points.begin(), points.end(), [&](const TileMap::Pos& first, const TileMap::Pos& second) {
		return first.x + first.y * m_columns < second.x + second.y * m_columns;
	});
	points.erase(std::unique(points.begin(), points.end()), points.end());

	TileMap::Pos src = TileMap::Pos(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	auto castRay = [&](const TileMap::Pos& destination) {
		std::vector<TileMap::Pos> intersectionPoints;
		for (auto& wall : m_walls) {
			float tA = (wall.first.y - wall.second.y) * (src.x - wall.first.x) + (wall.second.x - wall.first.x) * (src.y - wall.first.y);
			float tB = (src.y - destination.y) * (src.x - wall.first.x) + (destination.x - src.x) * (src.y - wall.first.y);
			float denominator = (wall.second.x - wall.first.x) * (src.y - destination.y) - (src.x - destination.x) * (wall.second.y - wall.first.y);
			if (denominator != 0) {
				tA /= denominator;
				tB /= denominator;
				if (0 <= tA && tA <= 1 && 0 <= tB && tB <= 1) {
					intersectionPoints.emplace_back(src.x + tA * (destination.x - src.x), src.y + tA * (destination.y - src.y));
				}
			}
			else {
				intersectionPoints.emplace_back(destination);
			}
		}
		std::sort(intersectionPoints.begin(), intersectionPoints.end(), [&](const TileMap::Pos& first, const TileMap::Pos& second) {
			return pow(first.x - src.x, 2) + pow(first.y - src.y, 2) < pow(second.x - src.x, 2) + pow(second.y - src.y, 2);
		});
		if (intersectionPoints.size() > 0)
			return intersectionPoints[0];
		return src;
	};

	std::vector<std::pair<float, TileMap::Pos>> POIs;

	for (std::size_t i = 0; i < points.size(); i++) {
		TileMap::Pos poi = castRay(points[i]);
		float angle = atan2f(poi.y - src.y, poi.x - src.x);
		float deviation = 0.001f;

		TileMap::Pos minusAngleRay = castRay({ src.x + 5000 * cosf(angle - deviation), src.y + 5000 * sinf(angle - deviation) });
		TileMap::Pos plusAngleRay  = castRay({ src.x + 5000 * cosf(angle + deviation), src.y + 5000 * sinf(angle + deviation) });

		POIs.emplace_back(std::make_pair(angle, poi));
		POIs.emplace_back(std::make_pair(angle - deviation, minusAngleRay));
		POIs.emplace_back(std::make_pair(angle + deviation, plusAngleRay));
	}
	std::sort(POIs.begin(), POIs.end(), [](const std::pair<float, TileMap::Pos>& left, const std::pair<float, TileMap::Pos>& right) {
		return left.first < right.first;
	});

	m_light.resize(POIs.size() + 1);
	if (POIs.size() > 0) {
		m_light[0].position = src; // centre of fan
		m_light[0].color = sf::Color::Yellow;
		for (std::size_t i = 1; i < POIs.size(); i++) {
			m_light[i].position = POIs[i].second;
			m_light[i].color = sf::Color::Yellow;
		}
		m_light[POIs.size()].position = m_light[1].position; //last point = the first fan point for 360 coverage
		m_light[POIs.size()].color = sf::Color::Yellow;
	}
}