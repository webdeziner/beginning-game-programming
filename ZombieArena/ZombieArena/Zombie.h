#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;

class Zombie {
private:
	// Zombie speed
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	// Zombie health
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;
	// Speed may vary
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	// Zombie position
	Vector2f m_Position;
	// Zombie sprite
	Sprite m_Sprite;
	// Speed
	float m_Speed;
	// Health
	float m_Health;
	// Still alive
	bool m_Alive;

public:
	// Bullet handler
	bool hit();
	// Is alive
	bool isAlive();
	// Spawn
	void spawn(float startX, float startY, int type, int seed);
	// Position
	FloatRect getPosition();
	// Copy for draw
	Sprite getSprite();
	// Update frame
	void update(float elapsedTime, Vector2f playerLocation);
};