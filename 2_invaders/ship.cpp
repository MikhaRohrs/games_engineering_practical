#include "ship.h"
#include "game.h"
using namespace sf;
using namespace std;

Ship::Ship() {};

Ship::Ship(IntRect ir) : Sprite()
{
    _sprite = ir;
    setTexture(sprite_sheet);
    setTextureRect(_sprite);
};

void Ship::Update(const float& dt) {}

//Define the ship deconstructor. 
//Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;

Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f pos) : Ship(ir)
{
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(pos);
}

void Invader::Update(const float& dt)
{
    Ship::Update(dt);
}