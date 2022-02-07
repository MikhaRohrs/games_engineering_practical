#include <SFML/Graphics.hpp>
#include <algorithm>

using namespace sf;
using namespace std;

constexpr Keyboard::Key controls[4] =
{
	Keyboard::W, // Player1 UP
	Keyboard::S, // Player1 DOWN
	Keyboard::Up, // Player2 UP
	Keyboard::Down // Player2 DOWN
};

constexpr Vector2f paddle_size(25.0f, 100.0f);
constexpr float ball_radius = 10.0f;
constexpr int game_width = 800;
constexpr int game_height = 600;
constexpr float paddle_speed = 400.0f;

Vector2f ball_velocity;
bool player2_serves = false;
bool ai = true;
int score1 = 0; // Score for player 1 (ball hits right wall)
int score2 = 0; // Score for player 2 (ball hits left wall)

CircleShape ball;
RectangleShape paddles[2];

Font font;
Text score_text;

void reset()
{
	paddles[0].setPosition(Vector2(paddle_size.x / 2.0f, game_height / 2.0f));
	paddles[1].setPosition(Vector2(game_width - paddle_size.x / 2.0f, game_height / 2.0f));

	ball.setPosition(Vector2(game_width / 2.0f, game_height / 2.0f));

	ball_velocity = {(player2_serves ? 100.0f : -100.0f), 60.0f};

	score_text.setString(to_string(score1) + " : " + to_string(score2));
	score_text.setPosition(Vector2((game_width * 0.5f) - (score_text.getLocalBounds().width * 0.5f), 0.0f));
}

void load()
{
	for (auto& p : paddles)
	{
		p.setSize(paddle_size - Vector2f(3, 3));
		p.setOrigin(paddle_size / 2.0f);
	}

	ball.setRadius(ball_radius);
	ball.setOrigin(Vector2f(ball_radius / 2.0f, ball_radius / 2.0f));

	reset();

	font.loadFromFile("res/fonts/RobotoMono-Regular.ttf");
	score_text.setFont(font);
	score_text.setCharacterSize(24);
}

void update(RenderWindow& window)
{
	// Reset clock, recalculate delta time
	static Clock clock;
	const float dt = clock.restart().asSeconds();

	// Check and consume events
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
	// Prevent left paddle from going off screen
	if (paddles[0].getPosition().y - (paddle_size.y * 0.5f) < 0)
	{
		direction1++;
	}
	if (paddles[0].getPosition().y + (paddle_size.y * 0.5f) > game_height)
	{
		direction1--;
	}
	paddles[0].move(Vector2(0.0f, direction1 * paddle_speed * dt));

	// Handle right paddle movement
	float direction2 = 0.0f;
	const float by = ball.getPosition().y;
	if (!ai) // If right paddle is controlled by player 2 (arrow keys)
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
	else // If right paddle is controlled by basic ai
	{
		if (by < paddles[1].getPosition().y)
		{
			direction2--;
		}
		if (by > paddles[1].getPosition().y)
		{
			direction2++;
		}
	}
	// Prevent right paddle from going off screen
	if (paddles[1].getPosition().y - (paddle_size.y * 0.5f) < 0)
	{
		direction2++;
	}
	if (paddles[1].getPosition().y + (paddle_size.y * 0.5f) > game_height)
	{
		direction2--;
	}
	direction2 = clamp(direction2, -1.0f, 1.0f);
	paddles[1].move(Vector2(0.0f, direction2 * paddle_speed * dt));

	ball.move(ball_velocity * dt);
	
	// Check for ball collision
	const float bx = ball.getPosition().x;
	if (by > game_height) // Bottom wall
	{
		ball_velocity.x *= 1.1f;
		ball_velocity.y *= -1.1f;
		ball.move(Vector2(0.0f, -10.0f));
	}
	else if(by < 0) // Top wall
	{
		ball_velocity.x *= 1.1f;
		ball_velocity.y *= -1.1f;
		ball.move(Vector2(0.0f, 10.0f));
	}
	else if (bx > game_width) // Right wall
	{
		score1++;
		player2_serves = true;
		reset();
	}
	else if(bx < 0) // Left wall
	{
		score2++;
		player2_serves = false;
		reset();
	}
	else if ( // Left paddle
		(bx < paddle_size.x && //Ball is inline or behind paddle
		by > paddles[0].getPosition().y - (paddle_size.y * 0.5f) && // AND ball is below top edge of paddle
		by < paddles[0].getPosition().y + (paddle_size.y * 0.5f))) // AND ball is above bottom edge of paddle
	{
		ball_velocity.x *= -1.1f;
		ball_velocity.y *= 1.1f;
		ball.move(Vector2(10.0f, 0.0f));
	}
	else if ( // Right paddle
		bx > game_width - paddle_size.x &&
		by > paddles[1].getPosition().y - (paddle_size.y * 0.5f) &&
		by < paddles[1].getPosition().y + (paddle_size.y * 0.5f))
	{
		ball_velocity.x *= -1.1f;
		ball_velocity.y *= 1.1f;
		ball.move(Vector2(-10.0f, 0.0f));
	}
}

void render(RenderWindow& window)
{
	window.draw(paddles[0]);
	window.draw(paddles[1]);
	window.draw(ball);
	window.draw(score_text);
}

int main()
{
	RenderWindow window(VideoMode(game_width, game_height), "PONG");
	load();
	while (window.isOpen())
	{
		window.clear();
		update(window);
		render(window);
		window.display();
	}
}
