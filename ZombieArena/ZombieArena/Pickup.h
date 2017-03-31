#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup {
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	// Sprite for the pickup
	Sprite m_Sprite;
	// Arena
	IntRect m_Arena;
	// Pickup value
	int m_Value;
	// Type
	int m_Type;
	// Handle spawn and disappearing
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

public:
	Pickup::Pickup(int type);

	// Prepare
	void setArena(IntRect arena);
	void spawn();

	// Position
	FloatRect getPosition();

	// The sprite
	Sprite getSprite();

	// Update
	void update(float elapsedTime);

	// Has spawned?
	bool isSpawned();

	// We got it?
	int gotIt();

	// Upgrade pickups
	void upgrade();
};