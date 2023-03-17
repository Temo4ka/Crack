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

    sf::Clock clock = {};
    sf::View   view = {};
    sf::Music music = {};

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
            drawMiniGame(&window, &background, &crack, &hero);
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

void drawMiniGame(sf::RenderWindow* window, Background* background, Table* table, Hero* hero) {
    (*window).draw((*background).getSprite());
    (*window).draw(  (*table)   .getSprite());
    (*window).draw(   (*hero)   .getSprite());
}

void drawScene(sf::RenderWindow* window, Background* background, ProgressBar* progressBar) {
    (*window).draw((*background).getSprite());
    (*window).draw((*progressBar).getSprite());
}

ErrorCode CRACK() {
    size_t BinFileSize = fsize(INPUT_BINFILE);

    FILE* streamIn = fopen(INPUT_BINFILE, "rb");
    if (streamIn == nullptr) return openBinFileErr;

    FILE* streamOut = fopen(OUTPUT_BINFILE, "wb");
    if (streamOut == nullptr) return openBinFileErr;

    char* buffer = (char*) calloc(BinFileSize + 1, sizeof(char));
    fread(buffer, sizeof(char), BinFileSize, streamIn);
    buffer[BinFileSize] = '\0';

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