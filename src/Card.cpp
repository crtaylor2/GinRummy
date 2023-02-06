#include "Card.h"

#include <algorithm>

Card::Card(Suit s, Value v)
{
    suit = s;
    value = v;
    meld = NOTMELD;
    probOfMeld = 0.0;
}

std::string Card::CardToString(const Card& Card)
{
    std::string CardString;

    switch(Card.value)
    {
        case ACE:
            CardString = "Ace";
            break;
        case TWO:
            CardString = "Two";
            break;
        case THREE:
            CardString = "Three";
            break;
        case FOUR:
            CardString = "Four";
            break;
        case FIVE:
            CardString = "Five";
            break;
        case SIX:
            CardString = "Six";
            break;
        case SEVEN:
            CardString = "Seven";
            break;
        case EIGHT:
            CardString = "Eight";
            break;
        case NINE:
            CardString = "Nine";
            break;
        case TEN:
            CardString = "Ten";
            break;
        case JACK:
            CardString = "Jack";
            break;
        case QUEEN:
            CardString = "Queen";
            break;
        case KING:
            CardString = "King";
            break;
    }

    CardString += " of ";

    switch(Card.suit)
    {
        case DIAMOND:
            CardString += "Diamonds";
            break;
        case CLUB:
            CardString += "Clubs";
            break;
        case HEART:
            CardString += "Hearts";
            break;
        case SPADE:
            CardString += "Spades";
            break;
    }

    if(Card.isMeld())
    {
        std::transform(CardString.begin(), CardString.end(), CardString.begin(), ::toupper);
    }

    return CardString;
}

bool Card::isMeld() const
{
    return meld == RUNMELD || meld == SETMELD;
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

bool Card::CompareForRuns(Card Card1, Card Card2)
{
    if(Card1.suit != Card2.suit)
        return Card1.suit < Card2.suit;
    else
        return Card1.value < Card2.value;
}

bool Card::CompareForSets(Card Card1, Card Card2)
{
    if(Card1.value != Card2.value)
        return Card1.value < Card2.value;
    else
        return Card1.suit < Card2.suit;
}
