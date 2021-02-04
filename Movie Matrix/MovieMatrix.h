/*MovieMatrix program is inspired by the movie "Matrix".
* Characters gradually appear on screen and a string of characters is randomized
* every half a second. The program will shut down after 20 seconds. Gradual character introduction is made
* by removing active columns from columnPositions and adding them to activePositions
*/
#pragma once
#include <array>
#include <vector>
#include <stdlib.h>
#include "Engine.h"

class Position
{
public:
    Position(int x, int y) { this->x = x; this->y = y; }
    int x, y;
};

class MovieMatrix :
    public Engine
{
public:
    MovieMatrix(int, int, int, int);
    bool Start();
    bool Update(float);
    ~MovieMatrix();
private:
    float timer;
    float timerCharacterChange;
    float timerColorChange;
    float timerEnding;
    short color;
    bool wrapAround;
    std::vector<Position> columnPositions;
    std::vector<Position> activePositions;
    short columnLength;
    wchar_t* characters;
    int screenHeight;
    int screenWidth;
    int appearanceCounter;
};
