#include <SFML/Graphics.hpp>
#include <algorithm>

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = 
{
	Keyboard::W, // Player1 UP
	Keyboard::S, // Player1 DOWN
	Keyboard::Up, // Player2 UP
	Keyboard::Down // Player2 DOWN
};

const Vector2f paddleSize(25.0f, 100.0f);
const float ballRadius = 10.0f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.0f;

Vector2f ballVelocity;
bool server = false;
bool ai = true;

CircleShape ball;
RectangleShape paddles[2];

void Reset()
{
	paddles[0].setPosition(Vector2(paddleSize.x / 2.0f, gameHeight / 2.0f));
	paddles[1].setPosition(Vector2(gameWidth - paddleSize.x / 2.0f, gameHeight / 2.0f));

	ball.setPosition(Vector2(gameWidth / 2.0f, gameHeight / 2.0f));

	ballVelocity = { (server ? 100.0f : -100.0f), 60.0f };
}

void Load()
{
	for (auto& p : paddles)
	{
		p.setSize(paddleSize - Vector2f(3, 3));
		p.setOrigin(paddleSize / 2.0f);
	}

	ball.setRadius(ballRadius);
	ball.setOrigin(Vector2f(ballRadius / 2.0f, ballRadius / 2.0f));

	Reset();
}

void Update(RenderWindow& window)
{
	// Reset clock, recalculate delta time
	static Clock clock;
	float dt = clock.restart().asSeconds();

	// Check and consume events
	Event event;
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

	if (Keyboard::isKeyPressed(Keyboard::Space))
	{
		ai = !ai;
	}

	// Handle left paddle movement
	float direction1 = 0.0f;
	if (Keyboard::isKeyPressed(controls[0]))
	{
		direction1--;
	}
	if (Keyboard::isKeyPressed(controls[1]))
	{
		direction1++;
	}
	if(paddles[0].getPosition().y - (paddleSize.y * 0.5) < 0)
	{
		direction1++;
	}
	if (paddles[0].getPosition().y + (paddleSize.y * 0.5) > gameHeight)
	{
		direction1--;
	}
	paddles[0].move(Vector2(0.0f, direction1 * paddleSpeed * dt));

	// Handle right paddle movement
	float direction2 = 0.0f;
	const float by = ball.getPosition().y;
	if(!ai) // Player 2 uses arrow keys
	{
		if (Keyboard::isKeyPressed(controls[2]))
		{
			direction2--;
		}
		if (Keyboard::isKeyPressed(controls[3]))
		{
			direction2++;
		}
	}
	else // Basic AI
	{
		if(by < paddles[1].getPosition().y)
		{
			direction2--;
		}
		if(by > paddles[1].getPosition().y)
		{
			direction2++;
		}
	}
	if (paddles[1].getPosition().y - (paddleSize.y * 0.5) < 0)
	{
		direction2++;
	}
	if (paddles[1].getPosition().y + (paddleSize.y * 0.5) > gameHeight)
	{
		direction2--;
	}
	direction2 = clamp(direction2, -1.0f, 1.0f);
	paddles[1].move(Vector2(0.0f, direction2 * paddleSpeed * dt));

	ball.move(ballVelocity * dt);

	// Check for ball collision
	const float bx = ball.getPosition().x;
	if (by > gameHeight) // Bottom wall
	{
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(Vector2(0.0f, -10.0f));
	}
	else if (by < 0) // Top wall
	{
		ballVelocity.x *= 1.1f;
		ballVelocity.y *= -1.1f;
		ball.move(Vector2(0.0f, -10.0f));
	}
	else if (bx > gameWidth) // Right wall
	{
		Reset();
	}
	else if (bx < 0) // Left wall
	{
		Reset();
	}
	else if ( // Left paddle
		bx < paddleSize.x && //Ball is inline or behind paddle
		by > paddles[0].getPosition().y - (paddleSize.y * 0.5) && // AND ball is below top edge of paddle
		by < paddles[0].getPosition().y + (paddleSize.y * 0.5) // AND ball is above bottom edge of paddle
		)
	{
		ballVelocity = -ballVelocity;
	}
	else if ( // Right paddle
		bx > gameWidth - paddleSize.x &&
		by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
		by < paddles[1].getPosition().y + (paddleSize.y * 0.5)
		)
	{
		ballVelocity = -ballVelocity;
	}
}

void Render(RenderWindow& window)
{
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
}

int main()
{
	RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
	Load();
	while (window.isOpen())
	{
		window.clear();
		Update(window);
		Render(window);
		window.display();
	}
}