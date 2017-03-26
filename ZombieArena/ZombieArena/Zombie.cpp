#include "stdafx.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

void Zombie::spawn(float startX, float startY, int type, int seed) {
	switch (type) {
	case 0:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		m_Speed = 40;
		m_Health = 5;
		break;
	case 1:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		m_Speed = 70;
		m_Health = 1;
		break;
	case 2:
		m_Sprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		m_Speed = 20;
		m_Health = 3;
		break;
	}
	
	// Modify the speed a little
	srand((int)time(0) * seed);
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	modifier /= 110;
	m_Speed *= modifier;

	// Start position
	m_Position.x = startX;
	m_Position.y = startY;
	// Center the sprite
	m_Sprite.setOrigin(25, 25);
	m_Sprite.setPosition(m_Position);
}

bool Zombie::hit() {
	m_Health--;
	if (m_Health < 0) {
		// DEAD ZOMBIE!
		m_Alive = false;
		m_Sprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	// Injured
	return false;
}

bool Zombie::isAlive() {
	return m_Alive;
}
FloatRect Zombie::getPosition() {
	return m_Sprite.getGlobalBounds();
}
Sprite Zombie::getSprite() {
	return m_Sprite;
}

void Zombie::update(float elapsedTime, Vector2f playerLocation) {
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;
	if (playerX > m_Position.x) {
		m_Position.x = m_Position.x + m_Speed * elapsedTime;
	}
	if (playerY > m_Position.y) {
		m_Position.y = m_Position.y + m_Speed * elapsedTime;
	}
	if (playerX < m_Position.x) {
		m_Position.x = m_Position.x - m_Speed * elapsedTime;
	}
	if (playerX < m_Position.y) {
		m_Position.y = m_Position.y - m_Speed * elapsedTime;
	}
	m_Sprite.setPosition(m_Position);
	float angle = (atan2(playerY - m_Position.y, playerX - m_Position.x) * 180) / 3.141;
	m_Sprite.setRotation(angle);
}