#include <SFML/Graphics.hpp>
#include "main.h"
#include "movement.h"
#include "collision.h"
#include<iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

enum game_state {
	START,
	CONTROLS,
	CREDITS,
	SETUP,
	PLAY,
	BOUNCING,
	WIN,
	LOSE,
};

int main()
{
	int bg = 0;
	int wincounter = 0;
	int score = 0;
	int winningScore;
	enum game_state game_state = START;
	int collisionType = 0;

	state cs;
	state ps;

	//images stuff
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Bouncy Boi");
	sf::Image icon;
	icon.loadFromFile("images/bouncyboi.png");
	window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	sf::Texture bouncyboiTexture1;
	sf::Sprite bouncyboi1;
	drawImage("images/bouncyboi.png", bouncyboiTexture1, bouncyboi1);

	sf::Sprite bouncyboi2;
	sf::Texture bouncyboiTexture2;
	drawImage("images/verybouncyboi.png", bouncyboiTexture2, bouncyboi2);

	sf::Sprite background1;
	sf::Texture backgroundTexture1;
	drawImage("images/background1.png", backgroundTexture1, background1);

	sf::Sprite background2;
	sf::Texture backgroundTexture2;
	drawImage("images/background2.png", backgroundTexture2, background2);

	sf::Sprite backgrounds[2] = { background1, background2 };

	sf::Sprite platform;
	sf::Texture platformTexture;
	drawImage("images/platform.png", platformTexture, platform);

	sf::Sprite moon;
	sf::Texture moonTexture;
	drawImage("images/moon.png", moonTexture, moon);
	moon.setPosition(sf::Vector2f(0, HEIGHT - (moon.getTexture()->getSize().y) * 4));

	sf::Sprite lose;
	sf::Texture loseTexture;
	drawImage("images/potato.jpg", loseTexture, lose);

	sf::Sprite win1;
	sf::Texture winTexture1;
	drawImage("images/win.png", winTexture1, win1);

	sf::Sprite win2;
	sf::Texture winTexture2;
	drawImage("images/win1.png", winTexture2, win2);

	sf::Sprite powerupSprite;
	sf::Texture powerupTexture;
	drawImage("images/star.png", powerupTexture, powerupSprite);
	powerupSprite.setScale(4, 4);

	//Text stuff
	sf::Font font;
	font.loadFromFile("fonts/ARCADECLASSIC.ttf");

	const char* creditsText[3] = { "Brought  to  you  by",
	"Allison  Lu     Programming  and  Art",
	"Cynthia  Peng     Bouncy  Boi  sprite" };

	std::vector<sf::Text> credits(3);
	for (int i = 0; i < 3; i++) {
		formatText(creditsText[i], credits[i], font);
		credits[i].setPosition(sf::Vector2f(100, 100 + (30 * i)));
	}

	const char* controlsText[10] = { "CONTROLS",
	"WASD     Move  the  platform",
	"Tab     Return  to  Title  Screen",
	"Z     Continue",
	"",
	"GAME  OBJECTIVE",
	"Keep  the  rabbit  bouncing!",
	"For  how  long  who  knows!",
	"Be  careful  though!" ,
	"He  may  eat  a  small  white  star  and  speed  up!"};

	std::vector<sf::Text> controls(10);
	for (int i = 0; i < 10; i++) {
		formatText(controlsText[i], controls[i], font);
		controls[i].setPosition(sf::Vector2f(100, 100 + (30 * i)));
	}

	sf::Text scoreText;
	formatText("Score " + std::to_string(score), scoreText, font);
	scoreText.setPosition(sf::Vector2f(75, 50));

	while (window.isOpen())
	{
		ticks++;
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Z) {
					switch (game_state) {
					case START:
						game_state = CREDITS;
						break;
					case CREDITS:
						game_state = CONTROLS;
						break;
					case CONTROLS:
						game_state = SETUP;
						break;
					}
				}

		}

		//SET UP
		switch (game_state) {
		case START:
		{
			if (ticks % 250 == 0) {
				if (bg == 1) {
					bg = 0;
				}
				else {
					bg = 1;
				}
			}
			break;
		}
		case SETUP:
		{
			score = 0;
			cs.boi.size = bouncyboi1.getTexture()->getSize().x * 4;
			cs.boi.row = cs.boi.size - 3;
			cs.boi.col = WIDTH / 2;
			cs.boi.speedR = 0.3;
			cs.boi.speedC = 0;
			cs.boi.bounced = false;

			cs.player.row = HEIGHT - 27;
			cs.player.col = WIDTH / 2 - platform.getTexture()->getSize().x / 2;
			cs.player.width = platform.getTexture()->getSize().x * 4;
			cs.player.height = platform.getTexture()->getSize().y * 4;

			cs.buffAmount = 0;
			cs.currentBuffs = 0;
			cs.buffdurations[0] = 0;

			winningScore = (rand() % 7) + 5;
			break;
		}

		case PLAY:
		{
			//slow movespeed once buff is over
			for (int i = cs.currentBuffs - 1; i >= 0; i--) {
				if (cs.buffdurations[i] > 0) {
					cs.buffdurations[i]--;
				}
				if (cs.buffdurations[i] == 0) {
					if (cs.boi.speedR > 1 || cs.boi.speedR < -1) {
						cs.boi.speedR = cs.boi.speedR / 2;
					}
					if (cs.boi.speedC > 1 || cs.boi.speedC < -1) {
						cs.boi.speedC = cs.boi.speedC / 2;
					}
					for (int j = i; j < cs.currentBuffs - 1; j++) {
						cs.buffdurations[j] = cs.buffdurations[j + 1];
					}
					cs.currentBuffs--;
				}
			}

			if (moveBunny(cs.boi.row, cs.boi.speedR, cs.boi.col, cs.boi.speedC, cs.boi.size)) {
				game_state = LOSE;
			}

			float speed = 0.9;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				movePlatform(&cs.player.row, 0, &cs.player.col, -speed, cs.player.width, cs.player.height);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				movePlatform(&cs.player.row, 0, &cs.player.col, speed, cs.player.width, cs.player.height);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				movePlatform(&cs.player.row, -speed, &cs.player.col, 0, cs.player.width, cs.player.height);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				movePlatform(&cs.player.row, speed, &cs.player.col, 0, cs.player.width, cs.player.height);
			}

			collisionType = collision(cs.player.row, cs.player.col, cs.player.width, cs.boi.row, cs.boi.col, cs.boi.size);
			if (collisionType) {
				game_state = BOUNCING;
			}

			if (rand() % 1000 > 990 && cs.buffAmount < MAX_BUFFS) {
				struct powerUp boost;
				boost.row = rand() % (HEIGHT / 2) + 10;
				boost.col = rand() % WIDTH;
				cs.buffs[cs.buffAmount] = boost;
				cs.buffAmount++;
			}

			for (int i = 0; i < cs.buffAmount; i++) {
				struct powerUp holder = cs.buffs[i];
				if (overlap(holder.row, holder.col, cs.boi.row, cs.boi.col, cs.boi.size)) {
					cs.boi.speedR = cs.boi.speedR * 1.3;
					cs.boi.speedC = cs.boi.speedC * 1.3;

					//add buff to slow down checker
					cs.buffdurations[cs.currentBuffs] = 35;
					cs.currentBuffs++;
					//remove
					for (int j = i; j < cs.buffAmount - 1; j++) {
						cs.buffs[j] = cs.buffs[j + 1];
					}
					cs.buffAmount--;
				}
			}


			break;
		}

		case BOUNCING:
		{
			for (int i = cs.currentBuffs - 1; i >= 0; i--) {
				if (cs.buffdurations[i] > 0) {
					cs.buffdurations[i]--;
				}
				if (cs.buffdurations[i] == 0) {
					if (cs.boi.speedR > 1 || cs.boi.speedR < -1) {
						cs.boi.speedR = cs.boi.speedR / 2;
					}
					if (cs.boi.speedC > 1 || cs.boi.speedC < -1) {
						cs.boi.speedC = cs.boi.speedC / 2;
					}
					for (int j = i; j < cs.currentBuffs - 1; j++) {
						cs.buffdurations[j] = cs.buffdurations[j + 1];
					}
					cs.currentBuffs--;
				}
			}
			cs.boi.speedR = -cs.boi.speedR;
			if (collisionType == 1) {
				if (cs.boi.speedC == 0) {
					cs.boi.speedC = cs.boi.speedR;
				}
				else if (cs.boi.speedC > 0) {
					cs.boi.speedC = -cs.boi.speedC;
				}
			}
			else if (collisionType == 2) {
				cs.boi.speedC = 0;
			}
			else if (collisionType == 3) {
				if (cs.boi.speedC == 0) {
					cs.boi.speedC = -cs.boi.speedR;
				}
				else if (cs.boi.speedC < 0) {
					cs.boi.speedC = -cs.boi.speedC;
				}
			}
			collisionType = 0;
			score++;
			if (score >= winningScore) {
				game_state = WIN;
			}
			break;
		}

		case WIN:
		{
			if (ticks % 250 == 0) {
				if (wincounter == 1) {
					wincounter = 0;
				}
				else {
					wincounter = 1;
				}
			}
		}

		default:
		{
			break;
		}
		}

		window.clear();

		//DRAW
		switch (game_state) {
		case START:
		{
			window.draw(backgrounds[bg]);

			break;
		}

		case CREDITS:
		{
			for (auto& text : credits) {
				window.draw(text);
			}
			break;
		}

		case CONTROLS:
		{
			for (auto& text : controls) {
				window.draw(text);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
				game_state = START;
			}
			break;
		}

		case SETUP:
			game_state = PLAY;
			break;

		case PLAY:
		{
			for (int i = 0; i < cs.buffAmount; i++) {
				struct powerUp holder = cs.buffs[i];
				powerupSprite.setPosition(sf::Vector2f(holder.col, holder.row));
				window.draw(powerupSprite);
			}
			window.draw(moon);

			bouncyboi1.setPosition(sf::Vector2f(cs.boi.col, cs.boi.row));
			window.draw(bouncyboi1);
			platform.setPosition(sf::Vector2f(cs.player.col, cs.player.row));
			window.draw(platform);

			scoreText.setString("Score " + std::to_string(score));

			window.draw(scoreText);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
				game_state = START;
			}
			break;
		}

		case BOUNCING:
		{    
			for (int i = 0; i < cs.buffAmount; i++) {
				struct powerUp holder = cs.buffs[i];
				powerupSprite.setPosition(sf::Vector2f(holder.col, holder.row));
				window.draw(powerupSprite);
			}

			window.draw(moon);
			bouncyboi2.setPosition(sf::Vector2f(cs.boi.col, cs.boi.row));
			window.draw(bouncyboi2);
			platform.setPosition(sf::Vector2f(cs.player.col, cs.player.row));
			window.draw(platform);

			sf::sleep(sf::milliseconds(110));

			if (cs.boi.bounced) {
				cs.boi.bounced = false;
				game_state = PLAY;
			}
			else {
				cs.boi.bounced = true;
			}
			break;
		}
		case WIN:

			if (wincounter == 0) {
				window.draw(win1);
			}
			else {
				window.draw(win2);
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
				game_state = START;
			}
			break;

		case LOSE:
		{
			window.draw(lose);
			sf::Text hold;
			formatText("Game  Over", hold, font);
			hold.setCharacterSize(65);
			hold.setPosition(sf::Vector2f(315, 100));
			window.draw(hold);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) {
				game_state = START;
			}
			break;
		}
		}

		window.display();
	}

	return 0;
}

////hhhh something is wrong here
void formatText(std::string input, sf::Text& text, sf::Font& font) {
	text.setString(input);
	text.setFont(font); // font is a sf::Font
	text.setCharacterSize(24); // in pixels, not points!
	text.setFillColor(sf::Color::White);
}

void drawImage(const char* path, sf::Texture& texture, sf::Sprite& sprite) {
	texture.loadFromFile(path);
	sprite.setTexture(texture);
	sprite.setScale(4, 4);
}