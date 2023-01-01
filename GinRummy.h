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

    std::string CardToString(const Card card);
};

#endif // GINRUMMY_H
