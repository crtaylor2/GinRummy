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
            card.isMeld = false;
            std::cout << "created " << Card::CardToString(card) << std::endl;
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
        std::cout << "card " << c << " is " << Card::CardToString(Deck.at(c)) << std::endl;
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
    Discard.push_back(Deck.back());
    Deck.pop_back();

    //Initialize variables
    PlayerScore = 0;
    ComputerScore = 0;

    PlayerTurn = true;
    ShowComputerHand = false;
    SortByRuns = true;

    DrawGame();
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

    if(SortByRuns)
    {
        std::sort(PlayerCards.begin(), PlayerCards.end(), SortBySuit);
        std::sort(ComputerCards.begin(), ComputerCards.end(), SortBySuit);
    }
    else
    {
        std::sort(PlayerCards.begin(), PlayerCards.end(), SortByValue);
        std::sort(ComputerCards.begin(), ComputerCards.end(), SortByValue);
    }

    for(int idx = 0; idx < PlayerCards.size(); ++idx) //5-14, maybe 15
    {
        std::string Left = std::to_string(idx+1) + ". " + Card::CardToString(PlayerCards.at(idx));
        std::string Right;
        std::string Middle;
        if(idx < ComputerCards.size())
        {
            if(ShowComputerHand)
                Right = std::to_string(idx+1) + ". " + Card::CardToString(ComputerCards.at(idx));
            else
                Right = "Card " + std::to_string(idx+1);
        }
        if(idx == 1)
            Middle = "DISCARD PILE";
        else if(idx == 2)
            Middle = !Discard.empty() ? Card::CardToString(Discard.back()) : "";
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

    std::string Input;
    std::cin >> Input;
    UserInput(Input);
}

void GinRummy::UserInput(std::string Input)
{
    if(Input == "R")
    {
        SortByRuns = true;
    }
    else if(Input == "P")
    {
        SortByRuns = false;
    }
    else if(Input == "S")
    {
        ShowComputerHand = true;
    }
    else if(Input == "H")
    {
        ShowComputerHand = false;
    }
    else if(Input == "Q")
    {
        exit(0);
    }
    else if(Input == "M")
    {
        CalculateUnmatchedMeld(PlayerCards);
        CalculateUnmatchedMeld(ComputerCards);
    }
    else if(Input == "D")
    {
        if(PlayerTurn && PlayerCards.size() < 11 && !Discard.empty())
        {
            PlayerCards.push_back(Discard.back());
            Discard.pop_back();
        }
    }
    else if(Input == "F")
    {
        if(PlayerTurn && PlayerCards.size() < 11 && !Deck.empty())
        {
            PlayerCards.push_back(Deck.back());
            Deck.pop_back();
        }
    }
    else if(!Input.empty() && Input.at(0) == 'D')
    {
        if(PlayerTurn && PlayerCards.size() > 10)
        {
            int idx = std::stoi(Input.substr(1)) - 1;
            if(idx >= 0 && idx < PlayerCards.size())
            {
                Discard.push_back(PlayerCards.at(idx));
                PlayerCards.at(idx) = PlayerCards.back();
                PlayerCards.pop_back();
                PlayerTurn = false;
            }
        }
    }
    // add else if for C
    // check to see if computer's turn
    // will picking up the discard reduce the unmatched meld in the computers hand?
    // if yes, pick it up
    // if no, pick up the face down card
    // discard the highest (or maybe random) unmatched card
    // set turn to the player
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

int GinRummy::CalculateUnmatchedMeld(std::vector<Card> &Hand) const
{
    for(Card& card : Hand)
        card.isMeld = false;

    std::vector<Card> RunsThenPairs = Hand;
    std::vector<Card> PairsThenRuns = Hand;
    std::vector<Card> HoldMeld;
    int RunsThenPairsCount;
    int PairsThenRunsCount;

    // Attempt 1 - runs then pairs
    SearchForRuns(RunsThenPairs);
    RemoveMeld(RunsThenPairs, HoldMeld);
    SearchForPairs(RunsThenPairs);
    AddMeld(RunsThenPairs, HoldMeld);
    RunsThenPairsCount = CountUnmatchedMeld(RunsThenPairs);

    // Attempt 2 - pairs thenn runs
    SearchForPairs(PairsThenRuns);
    RemoveMeld(PairsThenRuns, HoldMeld);
    SearchForRuns(PairsThenRuns);
    AddMeld(PairsThenRuns, HoldMeld);
    PairsThenRunsCount = CountUnmatchedMeld(PairsThenRuns);

    if(RunsThenPairsCount < PairsThenRunsCount)
    {
        Hand = RunsThenPairs;
        return RunsThenPairsCount;
    }
    else
    {
        Hand = PairsThenRuns;
        return PairsThenRunsCount;
    }
}

int GinRummy::CountUnmatchedMeld(const std::vector<Card> &Hand) const
{
    int Count = 0;
    for(const Card& card : Hand)
        if(!card.isMeld)
            Count = Count + Card::CardPoints(card);

    return Count;
}

void GinRummy::SearchForRuns(std::vector<Card> &Hand) const
{
    std::sort(Hand.begin(), Hand.end(), SortBySuit);
    for(int idx = 2; idx < Hand.size(); ++idx)
    {
        if((Hand.at(idx).suit == Hand.at(idx - 1).suit && Hand.at(idx - 1).suit == Hand.at(idx - 2).suit) &&
           (Hand.at(idx).value == (Hand.at(idx - 1).value + 1) && (Hand.at(idx - 1).value + 1) == (Hand.at(idx - 2).value + 2)) )
        {
            Hand.at(idx).isMeld = true;
            Hand.at(idx - 1).isMeld = true;
            Hand.at(idx - 2).isMeld = true;
        }
    }
}

void GinRummy::SearchForPairs(std::vector<Card> &Hand) const
{
    std::sort(Hand.begin(), Hand.end(), SortByValue);
    for(int idx = 2; idx < Hand.size(); ++idx)
    {
        if(Hand.at(idx).value == Hand.at(idx - 1).value && Hand.at(idx - 1).value == Hand.at(idx - 2).value)
        {
            Hand.at(idx).isMeld = true;
            Hand.at(idx - 1).isMeld = true;
            Hand.at(idx - 2).isMeld = true;
        }
    }
}

void GinRummy::RemoveMeld(std::vector<Card> &Hand, std::vector<Card> &Meld) const
{
    std::vector<Card> NonMeldHold;
    for(Card& card : Hand)
    {
        if(card.isMeld)
            Meld.push_back(card);
        else
            NonMeldHold.push_back(card);
    }
    Hand = NonMeldHold;
}

void GinRummy::AddMeld(std::vector<Card> &Hand, std::vector<Card> &Meld) const
{
    Hand.insert(Hand.end(), Meld.begin(), Meld.end());
    Meld.clear();
}
