#ifndef MAIN_H
#define MAIN_H

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

const int MAX_BUFFS = 3;

int ticks = 0;

struct boi {
    float row;
    float col;
    float size;
    float speedR;
    float speedC;
    bool bounced;
};

struct platform {
    float row;
    float col;
    float width;
    float height;
};

struct powerUp {
    float row;
    float col;
    int size;
};

struct state {
    int currentState;
    int nextState;
    struct boi boi;
    struct platform player;
    int buffAmount;
    struct powerUp buffs[MAX_BUFFS];
    int buffdurations[MAX_BUFFS];
    int currentBuffs;
};

void drawImage(const char* path, sf::Texture& texture, sf::Sprite& sprite);
void formatText(std::string input, sf::Text& controlLine, sf::Font& font);
#endif