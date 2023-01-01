#ifndef CARD_H
#define CARD_H

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

struct Card {
    Suit suit;
    Value value;
};

#endif // CARD_H
