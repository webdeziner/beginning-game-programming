#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet {
private:
	// Bullet position
	Vector2f m_Position;
	// Shape
	RectangleShape m_BulletShape;
	// Fired?
	bool m_InFlight = false;
	// Speed
	float m_BulletSpeed = 1000;
	// Disctance
	float m_BulletDistanceX;
	float m_BulletDistanceY;
	// Boundaries
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

public:
	Bullet();

	void stop();

	bool isInFlight();

	void shoot(float startX, float startY, float targetX, float targetY);

	FloatRect getPosition();

	RectangleShape getShape();

	void update(float elapsedTime);
};