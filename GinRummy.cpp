#include "GinRummy.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

int GinRummy::LineLength = 80;

bool SortBySuit(Card Card1, Card Card2)
{
    if(Card1.suit != Card2.suit)
        return Card1.suit < Card2.suit;
    else
        return Card1.value < Card2.value;
}

bool SortByValue(Card Card1, Card Card2)
{
    if(Card1.value != Card2.value)
        return Card1.value < Card2.value;
    else
        return Card1.suit < Card2.suit;
}

GinRummy::GinRummy()
{
    std::cout << "Deck has " << Deck.size() << " cards" << std::endl;
    //Create 52 cards and place them in the Deck
    for(int s = DIAMOND; s <= SPADE; ++s)
    {
        for(int n = ACE; n <= KING; ++n)
        {
            Card card;
            card.suit = (Suit)s;
            card.value = (Value)n;
            std::cout << "created " << CardToString(card) << std::endl;
            Deck.push_back(card);
        }
    }
    std::cout << "Deck has " << Deck.size() << " cards" << std::endl;

    //Shuffle the Deck
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(Deck.begin(), Deck.end(), g);

    for(int c = 0; c < Deck.size(); ++c)
    {
        std::cout << "card " << c << " is " << CardToString(Deck.at(c)) << std::endl;
    }

    //Deal each player 10 cards
    for(int d = 0; d < 10; ++d)
    {
        PlayerCards.push_back(Deck.back());
        Deck.pop_back();

        ComputerCards.push_back(Deck.back());
        Deck.pop_back();
    }

    std::cout << "size of player 1s hand is " << PlayerCards.size() << std::endl;
    std::cout << "size of player 2s hand is " << ComputerCards.size() << std::endl;
    std::cout << "size of Deck is " << Deck.size() << std::endl;

    //Turn over top card card place in the Discard pile
    Discard = Deck.back();
    Deck.pop_back();

    //Initialize variables
    PlayerScore = 0;
    ComputerScore = 0;

    PlayerTurn = true;
    ShowComputerHand = false;

    DrawGame();
}

std::string GinRummy::CardToString(const Card& Card)
{
    std::string CardSuit;
    std::string CardValue;

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

    return CardValue + " of " + CardSuit;
}

void GinRummy::DrawGame()
{
    static const std::string border(LineLength, 'x');

    std::cout << border << std::endl; //1
    std::cout << std::endl; //2

    std::ostringstream LineLeft; //start to build line 3
    LineLeft << "YOU (" << PlayerScore << " points)";
    if(PlayerTurn)
        LineLeft << " - your turn";
    std::ostringstream LineRight;
    if(!PlayerTurn)
        LineRight << "your turn - ";
    LineRight << "(" << ComputerScore << " points) COMPUTER";
    PrintLine(LineLeft.str(), LineRight.str()); //3

    std::cout << std::endl; //4

    for(int idx = 0; idx < PlayerCards.size(); ++idx) //5-14, maybe 15
    {
        std::string Left = CardToString(PlayerCards.at(idx));
        std::string Right;
        std::string Middle;
        if(ShowComputerHand)
            Right = CardToString(ComputerCards.at(idx));
        else
            Right = "Card " + std::to_string(idx+1);
        if(idx == 1)
            Middle = "DISCARD PILE";
        else if(idx == 2)
            Middle = CardToString(Discard);
        else if(idx == 5)
            Middle = "(F) - Take Face Down";
        else if(idx == 6)
            Middle = "(D) - Take Discard";
        PrintLine(Left, Middle, Right);
    }

    if(PlayerCards.size() == 10) //15
        std::cout << std::endl;

    std::cout << std::endl; //16

    PrintLine("(R) - Sort for Runs", "(S) - Show Hand"); //17
    PrintLine("(P) - Sort for Pairs", "(H) - Hide Hand"); //18
    std::cout << std::endl; //19
    PrintLine("(Dn) - Discard card #n", "(K) - Knock"); //20
    PrintLine("(M) - Unmatched Meld Count", "(G) - Gin"); //21
    PrintLine("(C) - Computer Play", "(Q) - Quit"); //22
    std::cout << border << std::endl; //23
    std::cout << "Enter command: "; //24

    char Input;
    std::cin >> Input;
    UserInput(Input);
}

void GinRummy::UserInput(char Input)
{
    if(toupper(Input) == 'R')
    {
        std::sort(PlayerCards.begin(), PlayerCards.end(), SortBySuit);
        std::sort(ComputerCards.begin(), ComputerCards.end(), SortBySuit);
    }
    else if(toupper(Input) == 'P')
    {
        std::sort(PlayerCards.begin(), PlayerCards.end(), SortByValue);
        std::sort(ComputerCards.begin(), ComputerCards.end(), SortByValue);
    }
    else if(toupper(Input) == 'S')
    {
        ShowComputerHand = true;
    }
    else if(toupper(Input) == 'H')
    {
        ShowComputerHand = false;
    }
    else if(toupper(Input) == 'Q')
    {
        exit(0);
    }
    else
    {
        std::cout << "Unexpected Input of " << Input << std::endl;
    }

    DrawGame();
}

void GinRummy::PrintLine(const std::string& Left, const std::string& Right) const
{
    int SpaceCount = LineLength - (Left.length() + Right.length());
    std::cout << Left << std::string(SpaceCount, ' ') << Right << std::endl;
}

void GinRummy::PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const
{
    static const int HalfLineLength = LineLength / 2;
    int LeftSpaceCount = HalfLineLength - (Left.length() + (Middle.length() / 2));
    int RightSpaceCount = HalfLineLength - (Right.length() + (Middle.length() / 2));

    // Integer math will result in non-exact lengths so we add/subtract as needed to get to the
    // required line length
    int TotalLineLength = Left.length() + Middle.length() + Right.length() + LeftSpaceCount + RightSpaceCount;
    if(TotalLineLength > LineLength)
        --LeftSpaceCount;
    else if(TotalLineLength < LineLength)
        ++LeftSpaceCount;

    // If LineLength isn't big enough to print the strings, the space counts can become negative and cause
    // exceptions to be thrown in the std::string constructors below so we always print at least one space
    if(LeftSpaceCount <= 0)
        LeftSpaceCount = 1;

    if(RightSpaceCount <= 0)
        RightSpaceCount = 1;

    std::cout << Left << std::string(LeftSpaceCount, ' ') << Middle << std::string(RightSpaceCount, ' ') << Right << std::endl;
}
