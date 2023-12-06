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
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				for (int i = 0; i < 5; i++)
				{
					//(RAND()*(Max-Min + 1)+Min)
					int numPoints = rand() % (50 - 25 + 1) + 25;
					Particle particle(m_Window, numPoints, Vector2i(event.mouseButton.x, event.mouseButton.y));
					m_particles.push_back(particle);
				}
			}
		}
	}
}

void Engine::update(float dtAsSeconds)
{
	for (auto particle = m_particles.begin(); particle != m_particles.end(); ) {
		
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

	m_Window.draw(text);

	m_Window.display();
}

void Engine::run()
{
	
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	sf::Font font;
	font.loadFromFile("Octosquares.ttf");

	text.setFont(font);
	text.setCharacterSize(25);
	text.setColor(sf::Color::White);
	text.setPosition({ 10,10 });
	
	sf::Clock clock;
	int frames = 0;
	const  int updateFrequency = 30;
	sf::Time elapsed = sf::Time::Zero;
	
	while (m_Window.isOpen()) {
		sf::Time elapsed1 = clock.restart();
		elapsed += elapsed1;
		frames++;
		float dt = elapsed1.asSeconds();

		if (frames % updateFrequency == 0) {
			 float fps = frames/ elapsed.asSeconds();
			 
			 text.setString("FPS: " + to_string((int)fps));

			elapsed = sf::Time::Zero;
			frames = 0;
		}
		
		input();

		update(dt);

		draw();

	}
}
