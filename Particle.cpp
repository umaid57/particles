#include "particle.h"
//call the default constructor for Matrix m_A in the initilization list
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : m_A(2, numPoints)
{

	m_ttl = TTL;
	m_numPoints = numPoints;

	//generate random number between [0:PI]
	m_radiansPerSec = ((float)rand() / (RAND_MAX)) * M_PI;

	//set the center of and resize the View object cartesianPlane to the size of the screen and invert the y axis
	m_cartesianPlane.setCenter(0,0);
	m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
	
	//use RenderTarget member Vector2f mapPixelToCoords (const Vector2i &point, const View &view) const
	//Convert a point from target coordinates to world coordinates.
	m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);
	
	//random velocity for the vertices
	m_vy = rand() % (500 - 100 + 1) + 100;
	m_vx = rand() % (500 - 100 + 1) + 100;
	if (rand() % 2 == 1) m_vx *= -1;
	
	//center color is white and outside color is random
	m_color1 = sf::Color::White;
	m_color2 = sf::Color::Color(rand() % 256, rand() % 256, rand() % 256);
	
	//initialize the angle of the vertices to a random number and the delta to the amount of rotation from each vertex
	float theta = fmod(static_cast<float>(rand()) / RAND_MAX, M_PI / 2);
	float dtheta = static_cast<float>(2 * M_PI) / (numPoints - 1);

	//add the number of vertices to the matrix
	for (int j = 0; j < numPoints; j++) {
		float dx, dy;
		float r = static_cast<float>(rand() % (80 - 20 + 1)) + 20;
		
		dx = r * cos(theta);
		dy = r * sin(theta);
		
		m_A(0, j) = m_centerCoordinate.x + dx;
		m_A(1, j) = m_centerCoordinate.y + dy;

		theta += dtheta;
	}

}

void Particle::draw(RenderTarget& target, RenderStates states) const
{
	//triangle vertex array with numPoints + 1 vertices to account for the center
	VertexArray lines(TriangleFan, m_numPoints + 1);

	Vector2f center;
	//Vector2i 	mapCoordsToPixel (const Vector2f &point, const View &view) const
	//Convert a point from world coordinates to target coordinates.
	center = (Vector2f)target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
	lines[0].position = center;
	lines[0].color = m_color1;

	//draw the rest of the points from the matrix to the VertexArray
	for (int j = 1; j <= m_numPoints; j++) {
		lines[j].position = (Vector2f)target.mapCoordsToPixel(Vector2f(m_A(0, j - 1), m_A(1, j - 1)), m_cartesianPlane);
		lines[j].color = m_color2;
	}

	target.draw(lines);

}




void Particle::rotate(double theta)
{
	
	Vector2f temp = m_centerCoordinate;
	translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

	RotationMatrix R(theta);
	m_A = R * m_A;
	
	translate(temp.x, temp.y);
	
}

void Particle::scale(double c)
{
	Vector2f temp = m_centerCoordinate;
	translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

	ScalingMatrix S(c);
	m_A = S * m_A;
	
	translate(temp.x, temp.y);
}

void Particle::translate(double xShift, double yShift)
{
	TranslationMatrix T(xShift, yShift, m_A.getCols());
	m_A = T + m_A;

	m_centerCoordinate.x += xShift;
	m_centerCoordinate.y += yShift;

}

void Particle::update(float dt)
{
	m_ttl -= dt;
	rotate(dt * m_radiansPerSec);

	scale(SCALE);

	float dx, dy;
	dx = dt * m_vx;
	m_vy -= dt * G;
	dy = dt * m_vy;

	translate(dx, dy);
}




bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
	int score = 0;
	cout << "Testing RotationMatrix constructor...";
	double theta = M_PI / 4.0;
	RotationMatrix r(M_PI / 4);
	if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
		&& almostEqual(r(0, 1), -sin(theta))
		&& almostEqual(r(1, 0), sin(theta))
		&& almostEqual(r(1, 1), cos(theta)))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing ScalingMatrix constructor...";
	ScalingMatrix s(1.5);
	if (s.getRows() == 2 && s.getCols() == 2
		&& almostEqual(s(0, 0), 1.5)
		&& almostEqual(s(0, 1), 0)
		&& almostEqual(s(1, 0), 0)
		&& almostEqual(s(1, 1), 1.5))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing TranslationMatrix constructor...";
	TranslationMatrix t(5, -5, 3);
	if (t.getRows() == 2 && t.getCols() == 3
		&& almostEqual(t(0, 0), 5)
		&& almostEqual(t(1, 0), -5)
		&& almostEqual(t(0, 1), 5)
		&& almostEqual(t(1, 1), -5)
		&& almostEqual(t(0, 2), 5)
		&& almostEqual(t(1, 2), -5))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing Particles..." << endl;
	cout << "Testing Particle mapping to Cartesian origin..." << endl;
	if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
	{
		cout << "Failed. Expected (0,0). Received: (" << m_centerCoordinate.x <<
			"," << m_centerCoordinate.y << ")" << endl;
	}
	else
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	cout << "Applying one rotation of 90 degrees about the origin..." << endl;
	Matrix initialCoords = m_A;
	rotate(M_PI / 2.0);
	bool rotationPassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1,
			j), initialCoords(0, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
				rotationPassed = false;
		}
	}
	if (rotationPassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Applying a scale of 0.5..." << endl;
	initialCoords = m_A;
	scale(0.5);
	bool scalePassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !
			almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ")==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
				scalePassed = false;
		}
	}
	if (scalePassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Applying a translation of (10, 5)..." << endl;
	initialCoords = m_A;
	translate(10, 5);
	bool translatePassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !
			almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ")==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
				translatePassed = false;
		}
	}
	if (translatePassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Score: " << score << " / 7" << endl;
}
