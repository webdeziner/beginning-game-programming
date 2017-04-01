#include "stdafx.h"
#include "Engine.h"

void Engine::draw() {
	m_Window.clear(Color::White);
	m_RippleShader.setUniform("uTime", m_GameTimeTotal.asSeconds());
	if (!m_SplitScreen) {
		// Background
		m_Window.setView(m_BGMainView);
		m_Window.draw(m_BackgroundSprite);
		m_Window.draw(m_BackgroundSprite, &m_RippleShader);
		m_Window.setView(m_MainView);

		// Level
		m_Window.draw(m_VALevel, &m_TextureTiles);

		// Characters
		m_Window.draw(m_Thomas.getSprite());
		m_Window.draw(m_Bob.getSprite());

		// Particles
		if (m_PS.running()) {
			m_Window.draw(m_PS);
		}
	}
	else {

		// Left
		// Background
		m_Window.setView(m_BGLeftView);
		m_Window.draw(m_BackgroundSprite);
		m_Window.draw(m_BackgroundSprite, &m_RippleShader);
		m_Window.setView(m_LeftView);

		// Level
		m_Window.draw(m_VALevel, &m_TextureTiles);

		// Characters
		m_Window.draw(m_Bob.getSprite());
		m_Window.draw(m_Thomas.getSprite());

		// Particles
		if (m_PS.running()) {
			m_Window.draw(m_PS);
		}

		// Right
		// Backgorund
		m_Window.setView(m_BGRightView);
		m_Window.draw(m_BackgroundSprite);
		m_Window.draw(m_BackgroundSprite, &m_RippleShader);
		m_Window.setView(m_RightView);

		// Level
		m_Window.draw(m_VALevel, &m_TextureTiles);

		// Characters
		m_Window.draw(m_Thomas.getSprite());
		m_Window.draw(m_Bob.getSprite());

		// Particles
		if (m_PS.running()) {
			m_Window.draw(m_PS);
		}
	}

	// Draw the HUD 
	// Switch to m_HudView 
	m_Window.setView(m_HudView);
	m_Window.draw(m_Hud.getLevel());
	m_Window.draw(m_Hud.getTime());
	if (!m_Playing)
	{
		m_Window.draw(m_Hud.getMessage());
	}

	// Display
	m_Window.display();
}