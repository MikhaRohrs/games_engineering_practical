#include <iostream>
#include <SFML/Graphics.hpp>
#include "ship.h"
#include "game.h"
using namespace sf;
using namespace std;

Texture spriteSheet;
Sprite invader;

vector<Ship*> ships;

void Reset()
{
	Invader::movingRight = true;
	Invader::speed = 20.0f;
}

void Load()
{
	if(!spriteSheet.loadFromFile("res/img/invaders_sheet.png"))
	{
		cerr << "Failed to load sprite sheet!" << endl;
	}

	// Create invaders
	for(int row = 0; row < invaders_rows; ++row)
	{
		auto rect = IntRect(Vector2(0, 0), Vector2(32, 32));
		for (int column = 0; column < invaders_columns; ++column)
		{
			const Vector2f position = { static_cast<float>(column) * 64, static_cast<float>(row) * 32 + 10 };
			auto inv = new Invader(rect, position);
			ships.push_back(inv);
		}
	}

	auto player = new Player;
	ships.push_back(player);

	Reset();
}

void Update(RenderWindow& window)
{
	static Clock clock;
	const float deltaTime = clock.restart().asSeconds();

	Event event{};
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
		{
			window.close();
			return;
		}
	}

	if (Keyboard::isKeyPressed(Keyboard::Escape))
	{
		window.close();
	}

	for(auto &ship : ships)
	{
		ship->Update(deltaTime);
	}
}

void Render(RenderWindow& window)
{
	window.draw(invader);

	for (const auto ship : ships)
	{
		window.draw(*ship);
	}
}

int main()
{
	RenderWindow window(VideoMode(gameWidth, gameHeight), "SPACE INVADERS");
	Load();
	while (window.isOpen())
	{
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
}