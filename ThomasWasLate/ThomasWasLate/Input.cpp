#include "stdafx.h"
#include "Engine.h"

void Engine::input() {
	Event event;
	while (m_Window.pollEvent(event)) {
		if (event.type == Event::KeyPressed) {
			// Close the game
			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				m_Window.close();
			}
			// Start the game
			if (Keyboard::isKeyPressed(Keyboard::Return)) {
				m_Playing = true;
			}
			// Switch characters
			if (Keyboard::isKeyPressed(Keyboard::Q)) {
				m_Character1 = !m_Character1;
			}
			// Switch full or split screen
			if (Keyboard::isKeyPressed(Keyboard::E)) {
				m_SplitScreen = !m_SplitScreen;
			}
		}
	}

	if (m_Thomas.handleInput()) {
		m_SM.playJump();
	}
	if (m_Bob.handleInput()) {
		m_SM.playJump();
	}
}