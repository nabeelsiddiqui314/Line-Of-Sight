#include "stdafx.h"
#include "Application.h"

Application::Application(int rows, int columns, int cellSize) : m_cellSize(cellSize) {
	m_tileMap = std::make_unique<TileMap>(rows, columns, cellSize);
}

void Application::update(const sf::RenderWindow& window) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_tileMap->setCellColor(TileMap::Pos(floor(sf::Mouse::getPosition(window).x / m_cellSize), floor(sf::Mouse::getPosition(window).y / m_cellSize)), sf::Color::Blue);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
		m_tileMap->clear();
	}
}

void Application::render(sf::RenderWindow& window) {
	m_tileMap->render(window);
}
