#include <iostream>
#include <SFML/Graphics.hpp>
#include "ship.h"
using namespace sf;
using namespace std;

Texture sprite_sheet;
Sprite invader;

vector<Ship*> ships;

void load()
{
	if(!sprite_sheet.loadFromFile("res/img/invaders_sheet.png"))
	{
		cerr << "Failed to load spritesheet!" << endl;
	}
	invader.setTexture(sprite_sheet);
	invader.setTextureRect(IntRect(Vector2(0, 0), Vector2(32, 32)));

	Invader* inv = new Invader(sf::IntRect(Vector2(0, 0), Vector2(32, 32)), { 100,100 });
	ships.push_back(inv);
}

void render(RenderWindow& window)
{
	window.draw(invader);
}

int main()
{
	RenderWindow window(VideoMode(500, 500), "SPACE INVADERS");
	load();
	while (window.isOpen())
	{
		window.clear();
		render(window);
		window.display();
	}
}