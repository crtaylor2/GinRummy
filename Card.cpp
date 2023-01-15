#include "Card.h"

std::string Card::CardToString(const Card& Card)
{
    std::string CardSuit;
    std::string CardValue;
    std::string CardMeld;

    switch(Card.suit)
    {
        case DIAMOND:
            CardSuit = "Diamonds";
            break;
        case CLUB:
            CardSuit = "Clubs";
            break;
        case HEART:
            CardSuit = "Hearts";
            break;
        case SPADE:
            CardSuit = "Spades";
            break;
    }

    switch(Card.value)
    {
        case ACE:
            CardValue = "Ace";
            break;
        case TWO:
            CardValue = "Two";
            break;
        case THREE:
            CardValue = "Three";
            break;
        case FOUR:
            CardValue = "Four";
            break;
        case FIVE:
            CardValue = "Five";
            break;
        case SIX:
            CardValue = "Six";
            break;
        case SEVEN:
            CardValue = "Seven";
            break;
        case EIGHT:
            CardValue = "Eight";
            break;
        case NINE:
            CardValue = "Nine";
            break;
        case TEN:
            CardValue = "Ten";
            break;
        case JACK:
            CardValue = "Jack";
            break;
        case QUEEN:
            CardValue = "Queen";
            break;
        case KING:
            CardValue = "King";
            break;
    }

    if(Card.isMeld)
        CardMeld = "#";

    return CardValue + " of " + CardSuit + CardMeld;
}

int Card::CardPoints(Card card)
{
    switch(card.value)
    {
    case ACE:
        return 1;
    case TWO:
        return 2;
    case THREE:
        return 3;
    case FOUR:
        return 4;
    case FIVE:
        return 5;
    case SIX:
        return 6;
    case SEVEN:
        return 7;
    case EIGHT:
        return 8;
    case NINE:
        return 9;
    case TEN:
    case JACK:
    case QUEEN:
    case KING:
        return 10;
    }

    return 0;
}
