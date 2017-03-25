// SFMLTemplate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <sstream>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	VideoMode vm(960, 540);
	RenderWindow window(vm, "Timber!!!", Style::Resize);

	// Background
	Texture textureBackground;
	textureBackground.loadFromFile("graphics/background.png");
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0, 0);

	// Tree
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(405, 0);

	// Bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 400);
	// Is active
	bool beeActive = false;
	// Speed
	float beeSpeed = 0.0f;

	// Cloud
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 125);
	spriteCloud3.setPosition(0, 250);
	// Active
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;
	// Speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;
	
	// Time
	Clock clock;

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 200;
	float timeBarHeight = 40;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((960 / 2) - timeBarStartWidth / 2, 490);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// Track puased
	bool paused = true;

	// Score
	int score = 0;

	// Text
	sf::Text messageText;
	sf::Text scoreText;
	// Font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	messageText.setFont(font);
	scoreText.setFont(font);

	// Message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");
	// Size
	messageText.setCharacterSize(35);
	scoreText.setCharacterSize(50);
	// Color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	// Position
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(960 / 2.0f, 540 / 2.0f);
	scoreText.setPosition(20, 20);

	// Tree branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	// Loop branches
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-1000, -1000);
		branches[i].setOrigin(110, 10);
	}

	updateBranches(1);
	updateBranches(2);
	updateBranches(3);
	updateBranches(4);
	updateBranches(5);

	// Player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(290, 360);
	// Player start
	side playerSide = side::LEFT;

	// Gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(300, 430);

	// Axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(350, 415);

	// Axe with tree
	const float AXE_POSITION_LEFT = 350;
	const float AXE_POSITION_RIGHT = 537.5;

	// Flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(405, 360);

	// Log variables
	bool logActive = false;
	float logSpeedX = 500;
	float logSpeedY = -750;

	// Player input
	bool acceptInput = false;

	// Sounds
	// Chop
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	// Death
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);
	// Out of time
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);


	while (window.isOpen())
	{
		/*
		Handle player input
		*/
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				acceptInput = true;
				// Hide axe
				spriteAxe.setPosition(1000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			paused = false;
			score = 0;
			timeRemaining = 5;

			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}
			// Hide gravestone
			spriteRIP.setPosition(675, 2000);
			// Move player
			spritePlayer.setPosition(290, 360);

			acceptInput = true;
		}

		if (acceptInput) {
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				playerSide = side::RIGHT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(600, 360);
				// Update branches
				updateBranches(score);
				// Set flying log
				spriteLog.setPosition(405, 360);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				chop.play();
			}

			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				playerSide = side::LEFT;
				score++;
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(290, 360);
				// Update branches
				updateBranches(score);
				// Set flying log
				spriteLog.setPosition(405, 360);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
				chop.play();

			}

		}

		/*
		Update scene
		*/
		if (!paused) {
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				paused = true;
				messageText.setString("Out of time!!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(960 / 2.0f, 540 / 2.0f);

				outOfTime.play();
			}

			if (!beeActive) {
				// Speed
				srand((int)time(0));
				beeSpeed = (rand() % 100) + 100;
				// Position
				srand((int)time(0) * 5);
				float height = (rand() % 250) + 250;
				spriteBee.setPosition(1000, height);
				beeActive = true;
			}
			else {
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
				if (spriteBee.getPosition().x < -100) {
					beeActive = false;
				}
			}

			// Cloud 1
			if (!cloud1Active) {
				// Speed
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);
				// Position
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 960) {
					cloud1Active = false;
				}
			}

			// Cloud 2
			if (!cloud2Active) {
				// Speed
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);
				// Position
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 960) {
					cloud2Active = false;
				}
			}

			// Cloud 3
			if (!cloud3Active) {
				// Speed
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);
				// Position
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 960) {
					cloud3Active = false;
				}
			}

			// Update score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			// Update branches
			for (int i = 0; i < NUM_BRANCHES; i++) {
				float height = i * 75;
				if (branchPositions[i] == side::LEFT) {
					// Move
					branches[i].setPosition(305, height);
					// Flip
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					// Move
					branches[i].setPosition(665, height);
					// Flip
					branches[i].setRotation(0);
				}
				else {
					branches[i].setPosition(1500, height);
				}
			}

			// Handle flying log
			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()), spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 1000) {
					logActive = false;
					spriteLog.setPosition(405, 360);
				}
			}

			// Squished to death
			if (branchPositions[5] == playerSide) {
				paused = true;
				acceptInput = false;
				spriteRIP.setPosition(262.5, 380);
				// Hide player
				spritePlayer.setPosition(1000, 330);
				// Change message
				messageText.setString("SQUISHED!");
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(960 / 2.0F, 540 / 2.0f);

				death.play();
			}

		}
		
		/*
		Draw scene
		*/

		// Clear everything from the last frame
		window.clear();

		// Draw scene
		window.draw(spriteBackground);
		// Clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		// Branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		// Tree
		window.draw(spriteTree);
		// Player
		window.draw(spritePlayer);
		// Axe
		window.draw(spriteAxe);
		// Flying log
		window.draw(spriteLog);
		// Gravestone
		window.draw(spriteRIP);
		// Bee
		window.draw(spriteBee);
		// Score
		window.draw(scoreText);
		// Timebar
		window.draw(timeBar);

		if (paused) {
			window.draw(messageText);
		}

		// Display scene
		window.display();
	}
    return 0;
}

void updateBranches(int seed) {
	// Move down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}
	// Spawn new
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}