#define _CRT_SECURE_NO_WARNINGS

#include <sys/stat.h>
#include <sys/types.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Header.h"
#include "DSL.h"

int main()
{
    CrackFlag crackFlag = Inactive;

    sf::    Clock        clock    = {};
    sf::    View         view     = {};
    sf::    Music        music    = {};
    sf::    Font         font     = {};
    sf::    Text         text     = {};
    sf::RectangleShape rectangle;

    ERR_EXE((font.loadFromFile(FONT_PACKAGE) == 0));

    ConfigText (&text, &font, CRACK_STRING);
    ConfigTable(&rectangle);

    if (!music.openFromFile(MUSIC)) ERR_EXE(openMusicFileErr);

    Background background = {};
    ERR_EXE(background.Init(BACKGROUND_TEXTURE));

    Background crackground = {};
    ERR_EXE(crackground.Init(CRACKGROUND_TEXTURE));

    Hero hero = {};
    ERR_EXE(hero.Init());

    Table crack = {};
    ERR_EXE(crack.Init(WINDOW_HEIGHT - TABLE_WIDTH, TABLE_Y_POSITION, TABLE_IMAGE));

    ProgressBar progressBar = {};
    ERR_EXE(progressBar.Init(BAR_IMAGE));

    sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), L"Crack", sf::Style::None);
    view.reset(sf::FloatRect(0, 0, WINDOW_HEIGHT, WINDOW_WIDTH));

    music.setVolume(STANDART_VOLUME);
    music.play();

    while (window.isOpen())
    {
        sf::Event event = {};
        
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 10000;

        hero.UpdateCondition(time, &crackFlag);

        if (crackFlag == Completed) {
            ERR_EXE(crackground.Init(CRACKCOMPLETED_TEXTURE));

            crackFlag = OFF;
            ERR_EXE(CRACK());
        }

        
        window.setView(view);
        if (crackFlag == Inactive)
            drawMiniGame(&window, &background, &text, &rectangle, &hero);
        else {
            progressBar.Update(time, &crackFlag);

            drawScene(&window, &crackground, &progressBar);
        }

        window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }
    }
    music.stop();

    return EXIT_SUCCESS;
}

ErrorCode CRACK() {
    size_t BinFileSize = fsize(INPUT_BINFILE);

    FILE* streamIn = fopen(INPUT_BINFILE, "rb");
    if (streamIn == nullptr) return openBinFileErr;

    FILE* streamOut = fopen(OUTPUT_BINFILE, "wb");
    if (streamOut == nullptr) return openBinFileErr;

    char* buffer = (char*) calloc(BinFileSize + 1, sizeof(char));

    if (buffer == nullptr)
        return openBinFileErr;
    else {
        fread(buffer, sizeof(char), BinFileSize, streamIn);
        buffer[BinFileSize] = '\0';
    }
    const char* cmd = "Access Granted$";

    strcpy(buffer + 114, cmd);

    fwrite(buffer, sizeof(char), BinFileSize, streamOut);

    fclose(streamIn);
    fclose(streamOut);
}

size_t fsize(const char* filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    return -1;
}

void ConfigText(sf::Text *text, sf::Font *font, const char* string, sf::Color color, sf::Text::Style style) {
    text->setFont(*font);
    text->setCharacterSize(TEXT_SIZE);

    text->setFillColor(color);
    text->setString(string);
    text->setPosition(CRACKTEXT_X, CRACKTEXT_Y);

    text->setStyle(style);

    return;
}

void ConfigTable(sf::RectangleShape* rectangular, sf::Color outLinecolor, sf::Color color) {
    rectangular->setSize(sf::Vector2f(TABLE_WIDTH, TABLE_HEIGTH));
    rectangular->setOutlineColor(outLinecolor);
    rectangular->setFillColor(color);
    rectangular->setOutlineThickness(TABLE_THICKNESS);
    rectangular->setPosition(TABLE_X_POSITION, TABLE_Y_POSITION);
}

void drawMiniGame(sf::RenderWindow* window, Background* background, sf::Text* text, sf::RectangleShape* table, Hero* hero) {
    window->draw((*background).getSprite());
    window->draw(*table);
    window->draw(*text);
    window->draw((*hero).getSprite());
}

void drawScene(sf::RenderWindow* window, Background* background, ProgressBar* progressBar) {
    window->draw((*background).getSprite());
    window->draw((*progressBar).getSprite());
}