#ifndef CARD_H
#define CARD_H

#include <string>

class Card {

public:
    enum Suit {
        DIAMOND,
        CLUB,
        HEART,
        SPADE
    };

    enum Value {
        ACE,
        TWO,
        THREE,
        FOUR,
        FIVE,
        SIX,
        SEVEN,
        EIGHT,
        NINE,
        TEN,
        JACK,
        QUEEN,
        KING
    };

    enum MeldStatus
    {
        NOTMELD,
        RUNMELD,
        SETMELD
    };

    Card(Suit s, Value v);

    Suit suit;
    Value value;
    MeldStatus meld;
    double probOfMeld;

    bool isMeld() const;

    static std::string CardToString(const Card& Card);
    static int CardPoints(Card card);

    static bool CompareForSets(Card Card1, Card Card2);
    static bool CompareForRuns(Card Card1, Card Card2);
};

#endif // CARD_H
