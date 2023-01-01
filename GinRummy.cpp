#include "GinRummy.h"

#include <iostream>
#include <random>
#include <algorithm>

GinRummy::GinRummy()
{
    std::cout << "deck has " << deck.size() << " cards" << std::endl;
    //Create 52 cards and place them in the deck
    for(int s = DIAMOND; s <= SPADE; ++s)
    {
        for(int n = ACE; n <= KING; ++n)
        {
            Card card;
            card.suit = (Suit)s;
            card.value = (Value)n;
            std::cout << "created " << CardToString(card) << std::endl;
            deck.push_back(card);
        }
    }
    std::cout << "deck has " << deck.size() << " cards" << std::endl;

    //TODO: Shuffle the deck
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    for(int c = 0; c < deck.size(); ++c)
    {
        std::cout << "card " << c << " is " << CardToString(deck.at(c)) << std::endl;
    }

    //TODO: Deal each player 10 cards

    //TODO: Turn over top card card place in the discard pile
}

std::string GinRummy::CardToString(const Card card)
{
    std::string CardSuit;
    std::string CardValue;

    switch(card.suit)
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

    switch(card.value)
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

    return CardValue + " of " + CardSuit;
}
