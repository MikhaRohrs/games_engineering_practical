#include "ship.h"
#include "bullet.h"
#include "game.h"

using namespace sf;
using namespace std;

constexpr Keyboard::Key CONTROLS[3] =
{
    Keyboard::A, // Player LEFT
    Keyboard::D, // Player RIGHT
    Keyboard::Space // Player FIRE
};

Ship::Ship() {};

bool Invader::movingRight;
float Invader::speed;

Ship::Ship(IntRect ir) : Sprite()
{
    Sprite = ir;
    setTexture(spriteSheet);
    setTextureRect(Sprite);
};

void Ship::Update(const float& deltaTime) {}

// Define the ship deconstructor. 
// Although we set this to pure virtual, we still have to define it.
Ship::~Ship() = default;


/****     INVADER     ****/
Invader::Invader() : Ship() {}

Invader::Invader(sf::IntRect ir, sf::Vector2f position) : Ship(ir)
{
    setOrigin(Vector2f(16.f, 16.f));
    setPosition(position);
}


void Invader::Update(const float& deltaTime)
{
    Ship::Update(deltaTime);

    move(Vector2f(deltaTime * (movingRight ? 1.0f : -1.0f) * speed, 0.0f));
    if ((movingRight && getPosition().x > gameWidth - 16) || (!movingRight && getPosition().x < 16))
    {
        movingRight = !movingRight;
        for (auto& ship : ships)
        {
            if(dynamic_cast<Player*>(ship) == nullptr)
            {
	            ship->move(Vector2(0.0f, 24.0f));
            }
        }
    }
}


/****     PLAYER     ****/
Player::Player() : Ship(IntRect(Vector2(160, 32), Vector2(32, 32)))
{
    setPosition({ gameWidth * 0.5f, gameHeight - 32.0f });
}

void Player::Update(const float& deltaTime)
{
    Ship::Update(deltaTime);
    float direction = 0.0f;
    if(Keyboard::isKeyPressed(CONTROLS[0]))
    {
        direction -= 100.0f;
    }
    if(Keyboard::isKeyPressed(CONTROLS[1]))
    {
        direction += 100.0f;
    }
    if((getPosition().x > gameWidth - 16))
    {
        direction = 0.0f;
        move(Vector2(-1.0f, 0.0f));
    }
    else if((getPosition().x < 16))
    {
        direction = 0.0f;
        move(Vector2(1.0f, 0.0f));
    }
    move(Vector2(direction * deltaTime, 0.0f));

    /*static vector<Bullet*> bullets;
    if(Keyboard::isKeyPressed(CONTROLS[2]))
    {
        bullets.push_back(new Bullet(getPosition(), false));
    }
    for (const auto bullet : bullets)
    {
        bullet->Update(deltaTime);
    }*/
}