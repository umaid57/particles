#include "Engine.h"

Engine::Engine()
{
	m_Window.create(sf::VideoMode(1920, 1080), "Particle");
}

void Engine::input()
{


}

void Engine::update(float dtAsSeconds)
{
}

void Engine::draw()
{
}



void Engine::run()
{
	sf::Clock clock;

	while (m_Window.isOpen()) {
		
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		input();

		update(dt);

		draw();

	}
}
