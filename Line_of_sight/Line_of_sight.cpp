#include "stdafx.h"
#include "Application.h"

int main()
{
	sf::RenderWindow window;
	window.create(sf::VideoMode(800, 600), "Line Of Sight");
	Application app(30, 40, 20);
	
	while (window.isOpen()) {
		sf::Event evnt;
		while (window.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		app.update(window);
		app.render(window);
		window.display();
	}
    return 0;
}

