#include "particle.h"

Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{
	m_ttl = TTL;
	m_numPoints = numPoints;
	m_radiansPerSec = ((float)rand() / RAND_MAX) * M_PI;
	m_cartesianPlane.setCenter(0, 0);
	m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
	m_centerCoordinate = mapPixelToCoords(mouseClickPosition);
	
	m_vy = rand() * (500 - 100) + 100;
	m_vx = rand() * (500 - 100) + 100;
	if (rand() % 2 == 1) m_vx *= -1;
	
	m_color1 = sf::Color::White;
	m_color2 = sf::Color::Color(rand() % 256, 0, rand() % 256);
	
	float theta = fmod(rand(), (M_PI / 2));
	float dtheta = 2 * M_PI / (numPoints - 1);

	for (int j = 0; j < numPoints; j++) {
		float dx, dy;
		int r = rand() * (80 - 20) + 20;
		dx = r * cos(theta);
		dy = r * sin(theta);
		
		m_A(0, j) = m_centerCoordinate.x + dx;
		m_A(1, j) = m_centerCoordinate.y + dy;

		theta += dtheta;
	}

}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
	VertexArray lines(TriangleFan, m_numPoints + 1);

}

void Particle::update(float dt)
{
}

bool Particle::almostEqual(double a, double b, double eps)
{
	return false;
}

void Particle::unitTests()
{
}

void Particle::rotate(double theta)
{
}

void Particle::scale(double c)
{
}

void Particle::translate(double xShift, double yShift)
{
}

Vector2f mapPixelToCoords(Vector2i mousePixel) {

	// formula to map a value n from range [a,b] into range [c,d] is: 
	//((n - a) / (b - a))* (d - c) + c

	float x = ((static_cast<float>(mousePixel.x) - 0) / (1920 - 0)) * 1920 + 0 - 1920 / 2.0;

	float y = ((static_cast<float>(mousePixel.y) - 1080) / (0 - 1080)) * 1080 + 0 - 1080 / 2.0;

	return Vector2f(x, y);

}