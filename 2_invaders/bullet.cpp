#include "bullet.h"
#include "game.h"

using namespace sf;
using namespace std;

Bullet Bullet::bullets[256];
unsigned char Bullet::bulletPointer;

void Bullet::Update(const float& deltaTime)
{
	for(auto &b : bullets)
	{
		b._Update(deltaTime);
	}
}

void Bullet::Render(sf::RenderWindow& window)
{
	for (const auto& b : bullets)
	{
		window.draw(b);
	}
}

void Bullet::Fire(const sf::Vector2f& position, const bool mode)
{
	//_mode = mode;
	bullets[++bulletPointer].setPosition(position);
}

void Bullet::Init()
{
	
	
}

void Bullet::_Update(const float& deltaTime)
{
	move(Vector2f(0, deltaTime * 200.0f * (_mode ? 1.0f : -1.0f)));
}
