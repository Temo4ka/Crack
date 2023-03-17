#pragma once
#include "config.h"
#include <string>

enum ErrorCode {
	Ok = 0,
	texturesLoadErr  = 1 << 0,
	openBinFileErr   = 1 << 1,
	openMusicFileErr = 1 << 2,
};

enum CrackFlag {
	Inactive = 0,
	Active = 1,
	Completed = 2,
	OFF = 3
};


enum Direction {
	Right = 1,
	Left = -1,
	Stay = 0,
};


struct Position {
	float x = 0;
	float y = 0;
};

class Hero {
	float   speedX    = SPEED;
	float   speedY    =   0  ;
	float frameSpeed  = SPEED / 15;

	float  curFrame  =   0  ;

	float height = HERO_HEIGHT;
	float width  =  HERO_WIDTH;


	sf::Texture heroTexture = {};
	sf::Sprite   heroSprite = {};

	ErrorCode loadTexture(const char* fileName = HERO_IMAGE) {
		sf::Image heroImage = {};
		if (heroImage.loadFromFile(fileName) == 0)
			return texturesLoadErr;
		heroImage.createMaskFromColor(sf::Color(255, 255, 255));

		if (heroTexture.loadFromImage(heroImage) == 0)
			return texturesLoadErr;

		return Ok;
	}

	void moveLeft() {
		heroSprite.setScale(-1.0f, 1.0f);
	}

	void moveRight() {
		heroSprite.setScale(1.0f, 1.0f);
	}

	void UpdateFrame(float time) {
		curFrame += frameSpeed * time;
		if (curFrame > FRAME_NUM) curFrame -= 7;
	}

public:
	Position curPos = {};

	ErrorCode Init() {
		if (loadTexture()) return texturesLoadErr;

		curPos.x = START_X;
		curPos.y = START_Y;

		heroSprite.setTexture(heroTexture);
		heroSprite.setTextureRect(sf::IntRect(0, 0, width, height));

		return Ok;
	}

	void move(float time, Direction direction, CrackFlag *crackFlag) {
		if (direction == Stay) {
			curFrame = 0;
			UpdateFrame(time);
			return;
		}

		if (direction == Right) {
			curPos.x += speedX * time;
			if (curPos.x >= WINDOW_HEIGHT) {
				*crackFlag = Active;
			}
			UpdateFrame(time);
			moveRight();
		}

		if (direction == Left) {
			curPos.x -= speedX * time;
			if (curPos.x <= HERO_WIDTH) {
				curPos.x  = HERO_WIDTH;
				curFrame  = 0;
			}
			UpdateFrame(time);
			moveLeft();
		}
	}

	void jump(float time) {
		curFrame = 0;
		speedY -= G * time;
		curPos.y -= speedY * time;

		if (curPos.y > START_Y) {
			curPos.y = START_Y;
			speedY = 0;
		}
	}

	void UpdateCondition(float time, CrackFlag* crackFlag) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			move(time, Right, crackFlag);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			move(time, Left, crackFlag);
		else
			move(time, Stay, crackFlag);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && curPos.y == START_Y)
			speedY = JUMP_SPEED;

		if (speedY || curPos.y != START_Y) jump(time);
	}

	sf::Sprite getSprite() {
		heroSprite.setTextureRect(sf::IntRect((width + 1) * int(curFrame), 1, width, height));

		heroSprite.setPosition(curPos.x, curPos.y);

		return heroSprite;
	}
};

class Table {
	sf::Texture texture = {};
	sf::Sprite  sprite  = {};
	Position	curPos  = {};
	
	ErrorCode loadTexture(const char *fileName) {
		if (texture.loadFromFile(fileName) == 0)
			return texturesLoadErr;

		return Ok;
	}

public:

	ErrorCode Init(float xPos, float yPos, const char *fileName) {
		if (loadTexture(fileName)) return texturesLoadErr;

		curPos.x = xPos;
		curPos.y = yPos;

		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, TABLE_WIDTH, TABLE_HEIGTH));
		sprite.setPosition(xPos, yPos);

		return Ok;
	}

	sf::Sprite getSprite() {

		sprite.setPosition(curPos.x, curPos.y);
		return sprite;
	}
};

class ProgressBar {
	sf::Texture texture;
	sf::Sprite   sprite;

	float curWidth =      1    ;
	float  height  = BAR_HEIGHT;
	float  speedX   =    SPEED  ;

	Position pos = {};

	ErrorCode loadTexture(const char* fileName) {
		if (texture.loadFromFile(fileName) == 0)
			return texturesLoadErr;

		return Ok;
	}

public:

	ErrorCode Init(const char* fileName) {
		if (loadTexture(fileName)) return texturesLoadErr;

		pos.x = BAR_START_X;
		pos.y = BAR_START_Y;

		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(0, 0, curWidth, height));
		sprite.setPosition(pos.x, pos.y);

		return Ok;
	}

	void Update(float time, CrackFlag *crackFlag) {
		if (BAR_START_X + curWidth >= WINDOW_HEIGHT && *crackFlag != OFF) {
			*crackFlag = Completed;
			return;
		}

		curWidth += time * speedX;
		sprite.setTextureRect(sf::IntRect(0, 0, curWidth, height));
	}

	sf::Sprite getSprite() {

		sprite.setPosition(pos.x, pos.y);
		return sprite;
	}
};

class Background {
	sf::Texture texture;
	sf::Sprite   sprite;

	ErrorCode loadTexture(const char* fileName) {
		if (texture.loadFromFile(fileName) == 0)
			return texturesLoadErr;

		return Ok;
	}

public:

	ErrorCode Init(const char* fileName) {
		if (loadTexture(fileName)) return texturesLoadErr;

		sprite.setTexture(texture);
		sprite.setPosition(0, 0);

		return Ok;
	}

	sf::Sprite getSprite() {
		return sprite;
	}
};

size_t fsize(const char* filename);

ErrorCode CRACK();

void ConfigText(sf::Text* text, sf::Font* font, const char* string, sf::Color color = sf::Color::White, sf::Text::Style style = sf::Text::Bold);

void ConfigTable(sf::RectangleShape* rectangular, sf::Color outLinecolor = sf::Color::Black, sf::Color color = sf::Color::Red);

void drawMiniGame(sf::RenderWindow* window, Background* background, sf::Text* text, sf::RectangleShape* table, Hero* hero);

void drawScene(sf::RenderWindow* window, Background* background, ProgressBar* progressBar);