#include "stdafx.h"
#include "Engine.h"


Engine::Engine() {
	// Resolution
	Vector2f resolution;
	//resolution.x = VideoMode::getDesktopMode().height;
	//resolution.y = VideoMode::getDesktopMode().width;
	resolution.x = 960;
	resolution.y = 540;
	m_Window.create(VideoMode(resolution.x, resolution.y), "Thomas was late", Style::Resize);
	// Full screen
	m_MainView.setSize(resolution);
	m_HudView.reset(FloatRect(0, 0, resolution.x, resolution.y));
	// Split screen
	m_LeftView.setViewport(FloatRect(0.001f, 0.001f, 0.498f, 0.998f));
	m_RightView.setViewport(FloatRect(0.5f, 0.001f, 0.499f, 0.998f));
	// Split screen BG
	m_BGLeftView.setViewport(FloatRect(0.001f, 0.001f, 0.498f, 0.998f));
	m_BGRightView.setViewport(FloatRect(0.5f, 0.001f, 0.499f, 0.998f));

	// Modern graphics?
	if (!sf::Shader::isAvailable())
	{
		// Time to get a new PC 
		m_Window.close();
	}
	else {
		m_RippleShader.loadFromFile("shaders/vertShader.vert", "shaders/rippleShader.frag");
	}

	// BG Texture
	m_BackgroundTexture = TextureHolder::GetTexture("graphics/background.png");
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	// Level texture
	m_TextureTiles = TextureHolder::GetTexture("graphics/tiles_sheet.png");

	// Initialize the particle system
	m_PS.init(1000);
}

void Engine::run() {
	Clock clock;
	while (m_Window.isOpen()) {
		Time dt = clock.restart();
		m_GameTimeTotal += dt;
		float dtAsSeconds = dt.asSeconds();

		input();
		update(dtAsSeconds);
		draw();
	}
}