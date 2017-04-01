#pragma once
#include <SFML\Graphics.hpp>
#include "TextureHolder.h"
#include "Thomas.h"
#include "Bob.h"
#include "LevelManager.h"
#include "SoundManager.h"
#include "HUD.h"
#include "ParticleSystem.h"

using namespace sf;

class Engine {
private:
	TextureHolder th;
	
	// Particles
	ParticleSystem m_PS;

	// Characters
	Thomas m_Thomas;
	Bob m_Bob;

	// Levels
	LevelManager m_LM;
	// Sound
	SoundManager m_SM;
	// The Hud
	Hud m_Hud;
	int m_FramesSinceLastHUDUpdate = 0;
	int m_TargetFramesPerHUDUpdate = 500;

	const int TILE_SIZE = 50;
	const int VERTS_IN_QUAD = 4;
	const int GRAVITY = 300;

	RenderWindow m_Window;
	// Main view
	View m_MainView;
	View m_LeftView;
	View m_RightView;
	// Background views
	View m_BGMainView;
	View m_BGLeftView;
	View m_BGRightView;
	// HUD
	View m_HudView;
	// Background
	Sprite m_BackgroundSprite;
	Texture m_BackgroundTexture;
	// Shaders
	Shader m_RippleShader;
	// Playing?
	bool m_Playing = false;
	// Character 1 or 2
	bool m_Character1 = true;
	// Multiplayer
	bool m_SplitScreen = false;
	// Time left
	float m_TimeRemaining = 10;
	Time m_GameTimeTotal;
	// New level
	bool m_NewLevelRequired = true;

	// The vertex array for the level tiles
	VertexArray m_VALevel;
	// The 2d array with the map for the level
	// A pointer to a pointer
	int** m_ArrayLevel = NULL;
	// Texture for the level tiles
	Texture m_TextureTiles;

	void input();
	void update(float dtAsSeconds);
	void draw();

	void loadLevel();

	bool detectCollisions(Character& character);

	void populateEmitters(vector <Vector2f>& vSoundEmitters, int** arrayLevel);

	// A vector of Vector2f for the fire emitter locations
	vector <Vector2f> m_FireEmitters;

public:
	// Construct
	Engine();
	// Run the game
	void run();
};