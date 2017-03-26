#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include <sstream>

using namespace sf;

int main()
{

	// Texture holder
	TextureHolder holder;

	// Game states
	enum class State {
		PAUSED, LEVELING_UP, GAME_OVER, PLAYING
	};

	// Game starts with game over
	State state = State::GAME_OVER;

	// Screen resolution
	Vector2f resolution;
	//resolution.x = VideoMode::getDesktopMode().width;
	//resolution.y = VideoMode::getDesktopMode().height;
	resolution.x = 960;
	resolution.y = 540;
	// Main window
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Resize);
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	// Clock for timing
	Clock clock;
	// PLAYING state time
	Time gameTimeTotal;

	// Get mouse coordinates
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;

	// New player
	Player player;
	// Arena
	IntRect arena;
	// Background
	VertexArray background;
	Texture textureBackground = TextureHolder::GetTexture("graphics/background_sheet.png");
	// Prepare the horde
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;

	// Text
	sf::Text messageText;
	sf::Text scoreText;
	// Font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");
	messageText.setFont(font);

	// Message
	messageText.setString("Press Enter to start!");
	// Size
	messageText.setCharacterSize(35);
	// Color
	messageText.setFillColor(Color::White);
	// Position
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(960 / 2.0f, 540 / 2.0f);

	// Main game loop
	while (window.isOpen()) {
		/*
		Handle inputs
		*/
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyPressed) {
				// Paus the game
				if (event.key.code == Keyboard::Return && state == State::PLAYING) {
					state = State::PAUSED;
				}
				// Restart the game
				else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
					state = State::PLAYING;
					clock.restart();
				}
				// Start new game
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
					state = State::LEVELING_UP;
				}

				if (state == State::PLAYING) {

				}
			}
		} // END while event polling

		// Handle quiting
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}
		// Handle controls (w,a,s,d)
		if (state == State::PLAYING) {
			// W
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player.moveUp();
			}
			else {
				player.stopUp();
			}
			// A
			if (Keyboard::isKeyPressed(Keyboard::A)) {
				player.moveLeft();
			}
			else {
				player.stopLeft();
			}
			// S
			if (Keyboard::isKeyPressed(Keyboard::S)) {
				player.moveDown();
			}
			else {
				player.stopDown();
			}
			// D
			if (Keyboard::isKeyPressed(Keyboard::D)) {
				player.moveRight();
			}
			else {
				player.stopRight();
			}
		} // END handle controls

		// Handle leveling up
		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				// Prepare level
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;
				// Size
				int tileSize = createBackground(background, arena);
				// Spawn player
				player.spawn(arena, resolution, tileSize);
				// Create the horde
				numZombies = 10;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;
				// Reset clock
				clock.restart();
			}

		} // END leveling up

		/*
		Update frame
		*/
		if (state == State::PLAYING) {
			Time dt = clock.restart();

			// Total game time
			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();
			// Get mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse to coordinates
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

			// Update player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Save player position
			Vector2f playerPosition(player.getCenter());

			// Center view around player
			mainView.setCenter(player.getCenter());

			// Loop zombies
			for (int i = 0; i < numZombies; i++) {
				if (zombies[i].isAlive()) {
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

		} // END playing

		/*
		Draw scene
		*/
		if (state == State::PLAYING) {
			window.clear();

			// Display main window
			window.setView(mainView);

			// Draw background
			window.draw(background, &textureBackground);

			// Draw zombies
			for (int i = 0; i < numZombies; i++) {
				window.draw(zombies[i].getSprite());
			}

			// Draw player
			window.draw(player.getSprite());
		} // END playing

		if (state == State::LEVELING_UP) {

		}
		if (state == State::PAUSED) {

		}
		if (state == State::GAME_OVER) {

		}

		// Show scene
		window.display();

	} // END game loop

	delete[] zombies;

    return 0;
}

