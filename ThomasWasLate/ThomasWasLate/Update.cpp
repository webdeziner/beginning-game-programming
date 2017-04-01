#include "stdafx.h"
#include "Engine.h"
#include <SFML\Graphics.hpp>
#include <sstream>

using namespace sf;

void Engine::update(float dtAsSeconds) {
	if (m_NewLevelRequired) {
		m_Thomas.spawn(Vector2f(0, 0), GRAVITY);
		m_Bob.spawn(Vector2f(0, 0), GRAVITY);
		m_TimeRemaining = 10;
		m_NewLevelRequired = false;

		loadLevel();
	}
	if (m_Playing) {
		// Update characters
		m_Thomas.update(dtAsSeconds);
		m_Bob.update(dtAsSeconds);
		// Detect collisions
		if (detectCollisions(m_Thomas) && detectCollisions(m_Bob)) {
			// New level required
			m_NewLevelRequired = true;
			// Play the reach goal sound
			m_SM.playReachGoal();
		}
		else {
			// Run bobs collision detection
			detectCollisions(m_Bob);
		}

		// Able to jump on each other
		if (m_Bob.getFeet().intersects(m_Thomas.getHead())) {
			m_Bob.stopFalling(m_Thomas.getHead().top);
		}
		else if (m_Thomas.getFeet().intersects(m_Bob.getHead())) {
			m_Thomas.stopFalling(m_Bob.getHead().top);
		}

		m_TimeRemaining -= dtAsSeconds;
		if (m_TimeRemaining <= 0) {
			m_NewLevelRequired = true;
		}
	} // END if playing

	// Check if a fire sound needs to be played
	vector<Vector2f>::iterator it;

	// Iterate through the vector of Vector2f objects
	for (it = m_FireEmitters.begin(); it != m_FireEmitters.end(); it++) {
		// Where is this emitter?
		// Store the location in pos
		float posX = (*it).x;
		float posY = (*it).y;
		// is the emiter near the player?
		// Make a 500 pixel rectangle around the emitter
		FloatRect localRect(posX - 250, posY - 250, 500, 500);

		// Is the player inside localRect?
		if (m_Thomas.getPosition().intersects(localRect)) {
			// Play the sound and pass in the location as well
			m_SM.playFire(Vector2f(posX, posY), m_Thomas.getCenter());
		}
	}

	if (m_SplitScreen) {
		m_LeftView.setCenter(m_Thomas.getCenter());
		m_RightView.setCenter(m_Bob.getCenter());
	}
	else {
		if (m_Character1) {
			m_MainView.setCenter(m_Thomas.getCenter());
		}
		else {
			m_MainView.setCenter(m_Bob.getCenter());
		}
	}

	// Time to update the HUD?
	// Increment the number of frames since the last HUD calculation
	m_FramesSinceLastHUDUpdate++;

	// Update the HUD every m_TargetFramesPerHUDUpdate frames
	if (m_FramesSinceLastHUDUpdate > m_TargetFramesPerHUDUpdate) {
		// Update game HUD text
		stringstream ssTime;
		stringstream ssLevel;
		// Update the time text 
		ssTime << (int)m_TimeRemaining;
		m_Hud.setTime(ssTime.str());
		// Update the level text
		ssLevel << "Level:" << m_LM.getCurrentLevel();
		m_Hud.setLevel(ssLevel.str());
		m_FramesSinceLastHUDUpdate = 0;
	}

	// Update the particles
	if (m_PS.running()) {
		m_PS.update(dtAsSeconds);
	}
}
