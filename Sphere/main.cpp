#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "Vector3.hpp"


const int Window_side = 500;

sf::Color matrix[Window_side][Window_side] = {};

struct Sphere
{
	int radius;
	Vector3<int> center;
	sf::Color color;
};


struct Lamp
{
	Vector3<int> pos;
	sf::Color color; // OR double intensivity;
};


sf::Color operator* (sf::Color col, double k)
{
	return sf::Color (col.r * k, col.g * k, col.b * k, col.a * k);
}


void RenderSphere (const Sphere &S, const Vector3<int> &Lamp)
{
	Vector3<int> r;

	for (r.y = -S.radius; r.y < S.radius; ++r.y)
		for (r.x = -S.radius; r.x < S.radius; ++r.x)
		{
			if (r.y * r.y + r.x * r.x < S.radius * S.radius)
			{
				r.z = sqrt (S.radius * S.radius - r.x * r.x - r.y * r.y);
				double cos = (((Lamp - r) ^ r) + 1) / 2;
				matrix[r.y + S.center.y][r.x + S.center.x] = S.color * cos;
			}
		}

}


int main ()
{
	const Sphere Sph_1 = {150, {Window_side / 2, Window_side / 2, 0}, {0, 255, 0, 255}};
	const int Max_lamp_x = 300;
	Vector3<int> Lamp = {-Max_lamp_x, -100, 250};

	sf::Texture image;
	image.create (Window_side, Window_side);

	sf::Sprite sprite (image);
	sprite.setPosition (0, 0);


	sf::RenderWindow window(sf::VideoMode (Window_side, Window_side), "Sphere");

	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		window.clear (sf::Color::Black);
		RenderSphere (Sph_1, Lamp);
		image.update ((sf::Uint8 *) &matrix[0][0], Window_side, Window_side, 0, 0);
		++Lamp.x;
		if (Lamp.x >= Max_lamp_x)
			Lamp.x = -Max_lamp_x;

		window.draw (sprite);
		window.display();
	}

	return 0;
}