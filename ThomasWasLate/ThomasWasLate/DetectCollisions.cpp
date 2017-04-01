#include "stdafx.h"
#include "Engine.h"

bool Engine::detectCollisions(Character& character) {
	bool reachedGoal = false;
	FloatRect detectionZone = character.getPosition();
	FloatRect block;

	block.width = TILE_SIZE;
	block.height = TILE_SIZE;

	int startX = (int)(detectionZone.left / TILE_SIZE) - 1;
	int startY = (int)(detectionZone.top / TILE_SIZE) - 1;
	int endX = (int)(detectionZone.left / TILE_SIZE) + 2;

	int endY = (int)(detectionZone.top / TILE_SIZE) + 3;

	if (startX < 0) {
		startX = 0;
	}
	if (startY < 0) {
		startY = 0;
	}
	if (endX >= m_LM.getLevelSize().x) {
		endX = m_LM.getLevelSize().x;
	}
	if (endY >= m_LM.getLevelSize().y) {
		endY = m_LM.getLevelSize().y;
	}

	FloatRect level(0, 0, m_LM.getLevelSize().x * TILE_SIZE, m_LM.getLevelSize().y * TILE_SIZE);

	if (!character.getPosition().intersects(level)) {
		// respawn the character 
		character.spawn(m_LM.getStartPosition(), GRAVITY);
	}

	for (int x = startX; x < endX; x++) {
		for (int y = startY; y < endY; y++) {
			// Initialize the starting position of the current block 
			block.left = x * TILE_SIZE;
			block.top = y * TILE_SIZE;

			// Has character been burnt or drowned? 
			// Use head as this allows him to sink a bit 
			if (m_ArrayLevel[y][x] == 2 || m_ArrayLevel[y][x] == 3) {
				if (character.getHead().intersects(block)) {
					character.spawn(m_LM.getStartPosition(), GRAVITY);
					// Which sound should be played? 
					// Fire
					if (m_ArrayLevel[y][x] == 2) {
						// Play a sound 
						m_SM.playFallInFire();
					}
					// Water
					else {
						// Play a sound 
						m_SM.playFallInWater();
					}
				}
			}

			// Is character colliding with a regular block 
			if (m_ArrayLevel[y][x] == 1) {

				if (character.getRight().intersects(block)) {
					character.stopRight(block.left);
				}
				else if (character.getLeft().intersects(block)) {
					character.stopLeft(block.left);
				}

				if (character.getFeet().intersects(block)) {
					character.stopFalling(block.top);
				}
				else if (character.getHead().intersects(block)) {
					character.stopJump();
				}
			}

			// More collision detection here once we have  
			// learned about particle effects 
			if (!m_PS.running()) {
				if (m_ArrayLevel[y][x] == 2 || m_ArrayLevel[y][x] == 3) {
					if (character.getFeet().intersects(block)) {
						// position and start the particle system
						m_PS.emitParticles(character.getCenter());
					}
				}
			}

			// Has the character reached the goal? 
			if (m_ArrayLevel[y][x] == 4) {
				// Character has reached the goal 
				reachedGoal = true;
			}

		}

	}
	return reachedGoal;
}