#pragma once
#include <SFML/Graphics.hpp>

class Ship : public sf::Sprite
{
protected:
    sf::IntRect Sprite;
    //Default constructor is hidden
    Ship();
public:
    // Constructor that takes a sprite
    explicit Ship(sf::IntRect ir);
    // Pure virtual deconstructor -- makes this an abstract class and avoids undefined behaviour!
    virtual ~Ship() = 0;
    // Update, virtual so can be overridden, but not pure virtual
    virtual void Update(const float& deltaTime);
};

class Invader : public Ship
{
public:
    static bool movingRight;
    static float speed;
    Invader(sf::IntRect ir, sf::Vector2f position);
    Invader();
    void Update(const float& deltaTime) override;
};

class Player : public Ship
{
public:
    Player();
    void Update(const float& deltaTime) override;
};