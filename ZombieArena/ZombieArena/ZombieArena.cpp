#include "stdafx.h"
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <sstream>
#include <fstream>

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
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	//resolution.x = 960;
	//resolution.y = 540;
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
	Zombie* zombies = NULL;

	// 100 Bullets
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastPressed;


	// Crosshair
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// Score
	int score = 0;
	int hiScore = 0;

	// Home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Ammo icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(10, 490);

	// Font
	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	// Paused screen
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(78);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(200, 200);
	pausedText.setString("Press enter \n to continue");

	// Game over screen
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(63);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(125, 425);
	gameOverText.setString("Press enter to play");

	// Level up screen
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(40);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(75, 125);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size (next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(23);
	ammoText.setColor(Color::White);
	ammoText.setPosition(100, 490);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(23);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(10, 0);

	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open()) {
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hiscore
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(23);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(700, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Zombies left
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(23);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(750, 490);
	zombiesRemainingText.setString("Zombies: 100");

	// Wave
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(23);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(625, 490);
	waveNumberText.setString("Wave: 0");

	// Health
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(225, 490);

	// Last HUD update
	int framesSinceLastHUDUpdate = 0;

	// Update inverval
	int fpsMeasurementFrameInterval = 1000;

	// Hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Failing sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

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
					wave = 0;
					score = 0;

					// Reset gun and ammo
					currentBullet = 0;
					bulletsSpare = 24;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					// Reset player
					player.resetPlayerStats();
				}

				if (state == State::PLAYING) {
					if (event.key.code == Keyboard::R) {
						if (bulletsSpare >= clipSize) {
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0) {
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else {
							reloadFailed.play();
						}
					}
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

			if (Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0) {

					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99) {
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					shoot.play();
					bulletsInClip--;
				}
			}
		} // END handle controls

		// Handle leveling up
		if (state == State::LEVELING_UP) {
			if (event.key.code == Keyboard::Num1) {
				// Fire rate
				fireRate++;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num2) {
				// Clip size
				clipSize += clipSize;
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num3) {
				// More health
				player.upgradeHealth();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num4) {
				// More speed
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num5) {
				// Better health pickup
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			if (event.key.code == Keyboard::Num6) {
				// Better ammo pickups
				ammoPickup.upgrade();
				state = State::PLAYING;
			}

			if (state == State::PLAYING) {
				// Waves
				wave++;
				// Prepare level
				arena.width = 500 * wave;
				arena.height = 500 * wave;
				arena.left = 0;
				arena.top = 0;
				// Size
				int tileSize = createBackground(background, arena);
				// Spawn player
				player.spawn(arena, resolution, tileSize);

				// Pickups!!!
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Create the horde
				numZombies = 5 * wave;
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;

				// powerup sound
				powerup.play();

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

			// Map crosshair to mouse
			spriteCrosshair.setPosition(mouseWorldPosition);

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
			// Update bullet
			for (int i = 0; i < 100; i++) {
				if (bullets[i].isInFlight()) {
					bullets[i].update(dtAsSeconds);
				}
			}

			// Update pickup
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Collision, shooting zombie
			for (int i = 0; i < 100; i++) {
				for (int j = 0; j < numZombies; j++) {
					if (bullets[i].isInFlight() && zombies[j].isAlive()) {
						if (bullets[i].getPosition().intersects(zombies[j].getPosition())) {
							bullets[i].stop();
							// HIT
							if (zombies[j].hit()) {
								score += 10;
								if (score >= hiScore) {
									hiScore = score;
								}
								numZombiesAlive--;
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}

							splat.play();
						}
					}
				}
			} // END shooting zombie

			// ew, it touched me
			for (int i = 0; i < numZombies; i++) {
				if (player.getPosition().intersects(zombies[i].getPosition()) && zombies[i].isAlive()) {
					if (player.hit(gameTimeTotal)) {
						hit.play();
					}
					if (player.getHealth() <= 0) {
						state = State::GAME_OVER;

						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();
					}
				}
			}

			// Great a health pickup
			if (player.getPosition().intersects(healthPickup.getPosition()) && healthPickup.isSpawned()) {
				player.increaseHealthLevel(healthPickup.gotIt());
				pickup.play();
			}
			// Ammo! awesome
			if (player.getPosition().intersects(ammoPickup.getPosition()) && ammoPickup.isSpawned()) {
				bulletsSpare += ammoPickup.gotIt();
				reload.play();
			}

			// Health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
			framesSinceLastHUDUpdate++;

			// HUD update
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) {
				// HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				// Ammo
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				// Score
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// HI score
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update hi score
				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				// Reset
				framesSinceLastHUDUpdate = 0;
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

			for (int i = 0; i < 100; i++){
				if (bullets[i].isInFlight()) {
					window.draw(bullets[i].getShape());
				}
			}

			// Draw player
			window.draw(player.getSprite());

			// Draw pickup
			if (healthPickup.isSpawned()) {
				window.draw(healthPickup.getSprite());
			}
			if (ammoPickup.isSpawned()) {
				window.draw(ammoPickup.getSprite());
			}

			// Draw the crosshair
			window.draw(spriteCrosshair);

			// Draw HUD
			window.setView(hudView);
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);


		} // END playing

		if (state == State::LEVELING_UP) {
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}
		if (state == State::PAUSED) {
			window.draw(pausedText);
		}
		if (state == State::GAME_OVER) {
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		// Show scene
		window.display();

	} // END game loop

	delete[] zombies;

    return 0;
}

