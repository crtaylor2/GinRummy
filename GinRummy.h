#ifndef GINRUMMY_H
#define GINRUMMY_H

#include "Card.h"

#include <vector>
#include <string>

class GinRummy
{
public:
    GinRummy();

private:
    std::vector<Card> deck;

    std::vector<Card> player1cards;
    std::vector<Card> player2cards;

    unsigned int player1score;
    unsigned int player2score;

    bool player1turn;

    Card discard;

    static int LineLength;

    void DrawGame();
    void UserInput(char input);

    std::string CardToString(const Card& card);
};

#endif // GINRUMMY_H
