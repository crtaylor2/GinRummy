#include "GinRummy.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

int GinRummy::LineLength = 80;

bool SortBySuit(Card card1, Card card2)
{
    if(card1.suit != card2.suit)
        return card1.suit < card2.suit;
    else
        return card1.value < card2.value;
}

bool SortByValue(Card card1, Card card2)
{
    if(card1.value != card2.value)
        return card1.value < card2.value;
    else
        return card1.suit < card2.suit;
}

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

    //Shuffle the deck
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);

    for(int c = 0; c < deck.size(); ++c)
    {
        std::cout << "card " << c << " is " << CardToString(deck.at(c)) << std::endl;
    }

    //Deal each player 10 cards
    for(int d = 0; d < 10; ++d)
    {
        player1cards.push_back(deck.back());
        deck.pop_back();

        player2cards.push_back(deck.back());
        deck.pop_back();
    }

    std::cout << "size of player 1s hand is " << player1cards.size() << std::endl;
    std::cout << "size of player 2s hand is " << player2cards.size() << std::endl;
    std::cout << "size of deck is " << deck.size() << std::endl;

    //Turn over top card card place in the discard pile
    discard = deck.back();
    deck.pop_back();

    //Initialize variables
    player1score = 0;
    player2score = 0;

    player1turn = true;

    DrawGame();
}

std::string GinRummy::CardToString(const Card& card)
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

void GinRummy::DrawGame()
{
    static const std::string border(LineLength, 'x');

    std::cout << border << std::endl; //1
    std::cout << std::endl; //2

    std::ostringstream LineLeft; //start to build line 3
    LineLeft << "YOU (" << player1score << " points)";
    if(player1turn)
        LineLeft << " - your turn";
    std::ostringstream LineRight;
    if(!player1turn)
        LineRight << "your turn - ";
    LineRight << "(" << player2score << " points) COMPUTER";

    int SpaceCount = LineLength - (LineLeft.str().length() + LineRight.str().length());
    std::cout << LineLeft.str() << std::string(SpaceCount, ' ') << LineRight.str() << std::endl; //3

    std::cout << std::endl; //4

    for(int idx = 0; idx < player1cards.size(); ++idx) //5-14, maybe 15
    {
        std::cout << CardToString(player1cards.at(idx)) << std::endl;
    }

    if(player1cards.size() == 10) //15
        std::cout << std::endl;

    std::cout << std::endl; //16

    std::cout << "(R) - Sort for Runs                                             (S) - Show Hand" << std::endl; //17
    std::cout << "(P) - Sort for Pairs                                            (H) - Hide Hand" << std::endl; //18
    std::cout << std::endl; //19
    std::cout << "(Dn) - Discard card #n" << std::endl; //20
    std::cout << "(K) - Knock" << std::endl; //21
    std::cout << "(G) - Gin" << std::endl; //22
    std::cout << "(M) - Unmatched Meld Count" << std::endl; //23
    std::cout << border; //24

    char input;
    std::cin >> input;
    UserInput(input);
}

void GinRummy::UserInput(char input)
{
    if(toupper(input) == 'R')
    {
        std::sort(player1cards.begin(), player1cards.end(), SortBySuit);
        std::sort(player2cards.begin(), player2cards.end(), SortBySuit);
    }
    else if(toupper(input) == 'P')
    {
        std::sort(player1cards.begin(), player1cards.end(), SortByValue);
        std::sort(player2cards.begin(), player2cards.end(), SortByValue);
    }
    else
    {
        std::cout << "Unexpected input of " << input << std::endl;
    }

    DrawGame();
}
