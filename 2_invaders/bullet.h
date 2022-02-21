#pragma once
#include <SFML/Graphics.hpp>

class Bullet : public sf::Sprite
{
public:
	static void Update(const float& deltaTime);
	static void Render(sf::RenderWindow& window);
	static void Fire(const sf::Vector2f& position, const bool mode);
	static void Init();

	~Bullet() = default;
protected:
	static unsigned char bulletPointer;
	static Bullet bullets[256];

	void _Update(const float& deltaTime);
	Bullet();
	bool _mode;
};