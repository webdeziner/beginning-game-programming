#pragma once
#include <SFML\Graphics.hpp>

using namespace sf;

class Player {
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	// Player position
	Vector2f m_Position;
	// Player sprite
	Sprite m_Sprite;
	// Player texture
	Texture m_Texture;
	// Screen resolution
	Vector2f m_Resolution;
	// Current arena
	IntRect m_Arena;
	// Tile size
	int m_TileSize;
	// Moving direction
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;
	// Player health;
	int m_Health;
	// Player max health
	int m_MaxHealth;
	// Player last hit
	Time m_LastHit;
	// Speed
	float m_Speed;

public:
	Player();
	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// End game reset
	void resetPlayerStats();

	// Zoombie hit
	bool hit(Time timeHit);

	// Last hit from zoombie
	Time getLastHitTime();

	// Player position
	FloatRect getPosition();

	// Player center
	Vector2f getCenter();

	// Player angle
	float getRotation();

	// Copy sprite
	Sprite getSprite();

	// Player movement
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	// Stop movement
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();

	// Update frames
	void update(float elapsedTime, Vector2i mousePosition);

	// Player speed boost
	void upgradeSpeed();

	// Player health
	void upgradeHealth();

	// Increase health
	void increaseHealthLevel(int amount);

	// Current health
	int getHealth();
};