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
    std::vector<Card> Deck;

    std::vector<Card> PlayersCards;
    std::vector<Card> ComputerCards;

    unsigned int PlayerScore;
    unsigned int ComputerScore;

    bool PlayerTurn;
    bool ShowComputerHand;

    Card Discard;

    static int LineLength;

    void DrawGame();
    void UserInput(char Input);

    void PrintLine(const std::string& Left, const std::string& Right) const;

    std::string CardToString(const Card& Card);
};

#endif // GINRUMMY_H
