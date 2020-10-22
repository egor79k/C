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
	sf::Color color;
};


sf::Color operator* (sf::Color col, double k)
{
	return sf::Color (col.r * k, col.g * k, col.b * k, col.a * k);
}


void RenderSphere (const Sphere &S, const Lamp &L)
{
	const Vector3<int> camera = {0, 0, S.radius * 2};
	Vector3<int> r;
	const double I_flare_k = 0.03;

	for (r.y = -S.radius; r.y < S.radius; ++r.y)
		for (r.x = -S.radius; r.x < S.radius; ++r.x)
		{
			if (r.y * r.y + r.x * r.x < S.radius * S.radius)
			{
				r.z = sqrt (S.radius * S.radius - r.x * r.x - r.y * r.y);
				Vector3<int> ldv = L.pos - r; // Lamp Direction Vector (from current point)
				double I_diffuse = ldv ^ r;
				double I_flares = (ldv - 2 * (ldv.project (r))) ^ (camera - r);
				double I = ((I_diffuse + 1) / 2) + (pow ((I_flares + 1) / 2, 24) + I_flare_k) / 3;
				if (I > 1) I = 1;
				matrix[r.y + S.center.y][r.x + S.center.x] = S.color * I;
			}
		}

}


int main ()
{
	const Sphere Sph_1 = {200, {Window_side / 2, Window_side / 2, 0}, {50, 255, 50, 255}};
	const int Max_lamp_x = 300;
	Lamp lamp = {{-Max_lamp_x, -100, 300}, {255, 255, 255, 255}};

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
		RenderSphere (Sph_1, lamp);
		image.update ((sf::Uint8 *) &matrix[0][0], Window_side, Window_side, 0, 0);
		++lamp.pos.x;
		if (lamp.pos.x >= Max_lamp_x)
			lamp.pos.x = -Max_lamp_x;

		window.draw (sprite);
		window.display();
	}

	return 0;
}