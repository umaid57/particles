#include "Engine.h"

Engine::Engine()
{
	m_Window.create(sf::VideoMode(1920, 1080), "Particle");
}

void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) m_Window.close();
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			m_Window.close();
		}
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			for (int i = 0; i < 5; i++) 
			{
				//(RAND()*(Max-Min)+Min)
				int numPoints = rand() * (50 - 25) + 25;
				Particle particle(m_Window, numPoints, Vector2i(event.mouseButton.x, event.mouseButton.y));
				m_particles.push_back(particle);
			}
		}

	}
}

void Engine::update(float dtAsSeconds)
{
	for (auto particle = m_particles.begin(); particle != m_particles.end();) {
		
		if (particle->getTTL() > 0.0) {
			
			particle->update(dtAsSeconds);
			particle++;
		
		}
		else {
			
			particle = m_particles.erase(particle);
		
		}

	}
}

void Engine::draw()
{
	m_Window.clear();
	for (auto& particle : m_particles) {
		m_Window.draw(particle);
	}
	m_Window.display();
}

void Engine::run()
{
	sf::Clock clock;
	
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	while (m_Window.isOpen()) {
		
		sf::Time elapsed = clock.restart();
		float dt = elapsed.asSeconds();

		input();

		update(dt);

		draw();

	}
}
