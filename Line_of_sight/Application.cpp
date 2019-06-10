#include "stdafx.h"
#include "Application.h"

Application::Application(int rows, int columns, int cellSize) {
	m_tileMap = std::make_unique<TileMap>(rows, columns, cellSize);
}

void Application::update() {

}

void Application::render(sf::RenderWindow& window) {
	m_tileMap->render(window);
}
