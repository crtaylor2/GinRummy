#include "GinRummy.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <sstream>

int GinRummy::LineLength = 80;

GinRummy::GinRummy()
{
    PlayerScore = 0;
    ComputerScore = 0;

    PlayerTurn = true;
    ShowComputerHand = false;
    SortByRuns = true;

    DealNewRound();
    Play();
}

//////////////////////////////////////////////////////////////////////
/// Creates the decks and deals the cards to the players
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::DealNewRound()
{
    Deck.clear();
    PlayerCards.clear();
    ComputerCards.clear();
    Discard.clear();

    //Create 52 cards and place them in the Deck
    for(int s = DIAMOND; s <= SPADE; ++s)
    {
        for(int n = ACE; n <= KING; ++n)
        {
            Card card;
            card.suit = (Suit)s;
            card.value = (Value)n;
            card.isMeld = false;
            Deck.push_back(card);
        }
    }

    //Shuffle the Deck
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(Deck.begin(), Deck.end(), g);

    //Deal each player 10 cards
    for(int d = 0; d < 10; ++d)
    {
        PlayerCards.push_back(Deck.back());
        Deck.pop_back();

        ComputerCards.push_back(Deck.back());
        Deck.pop_back();
    }

    //Turn over top card card place in the Discard pile
    Discard.push_back(Deck.back());
    Deck.pop_back();
}

//////////////////////////////////////////////////////////////////////
/// Repeatedly makes calls to draw the game and to request user input
/// until the user elects to quit the game
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::Play()
{
    while(true)
    {
        DrawGame();
        UserInput();
    }
}

//////////////////////////////////////////////////////////////////////
/// Prints the current state of the game
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
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

    FindUnmatchedMeld(PlayerCards);
    FindUnmatchedMeld(ComputerCards);

    for(Card& card : Discard)
        card.isMeld = false;

    if(SortByRuns)
    {
        std::sort(PlayerCards.begin(), PlayerCards.end(), Card::CompareForRuns);
        std::sort(ComputerCards.begin(), ComputerCards.end(), Card::CompareForRuns);
    }
    else
    {
        std::sort(PlayerCards.begin(), PlayerCards.end(), Card::CompareForSets);
        std::sort(ComputerCards.begin(), ComputerCards.end(), Card::CompareForSets);
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
            Middle = "(F) - Take Face Down (" + std::to_string(Deck.size()) + ")";
        else if(idx == 6)
            Middle = "(D) - Take Discard (" + std::to_string(Discard.size()) + ")";
        PrintLine(Left, Middle, Right);
    }

    if(PlayerCards.size() == 10) //15
        std::cout << std::endl;

    std::cout << std::endl; //16

    PrintLine("(R) - Sort for Runs", "(S) - Show Hand"); //17
    PrintLine("(P) - Sort for Sets", "(H) - Hide Hand"); //18
    std::cout << std::endl; //19
    PrintLine("(Dn) - Discard card #n", "(K) - Knock"); //20
    PrintLine("(P) - Pass", "(G) - Gin"); //21
    PrintLine("(C) - Computer Play", "(Q) - Quit"); //22
    std::cout << border << std::endl; //23
    std::cout << "Enter command: "; //24
}

//////////////////////////////////////////////////////////////////////
/// Accepts input from the user and responds accordingly
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::UserInput()
{
    std::string Input;
    std::cin >> Input;

    std::transform(Input.begin(), Input.end(), Input.begin(), ::toupper);

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
    else if(Input == "C")
    {
        if(!PlayerTurn)
        {
            if(PickupDiscard(ComputerCards))
            {
                ComputerCards.push_back(Discard.back());
                Discard.pop_back();
            }
            else
            {
                ComputerCards.push_back(Deck.back());
                Deck.pop_back();
            }

            FindUnmatchedMeld(ComputerCards);

            int idx = IndexToDiscard(ComputerCards);
            Discard.push_back(ComputerCards.at(idx));
            ComputerCards.erase(ComputerCards.begin() + idx);

            if(ComputerCards.size() < 11 && SumUnmatchedMeld(ComputerCards) == 0)
            {
                ComputerScore += FindUnmatchedMeldWithPartner(PlayerCards, ComputerCards) + 20;
                DealNewRound();
            }
            else if(Knock(ComputerCards))
            {
                int ComputerHandUnmatched = SumUnmatchedMeld(ComputerCards);
                int PlayerHandUnmatched = FindUnmatchedMeldWithPartner(PlayerCards, ComputerCards);

                if(ComputerHandUnmatched < PlayerHandUnmatched)
                    ComputerScore += PlayerHandUnmatched - ComputerHandUnmatched;
                else
                    PlayerScore += ComputerHandUnmatched - PlayerHandUnmatched + 10;

                DealNewRound();
            }

            PlayerTurn = true;
        }
    }
    else if(Input == "K")
    {
        if(PlayerCards.size() < 11 && SumUnmatchedMeld(PlayerCards) < 11)
        {
            int PlayerHandUnmatched = SumUnmatchedMeld(PlayerCards);
            int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerCards, PlayerCards);

            if(PlayerHandUnmatched < ComputerHandUnmatched)
                PlayerScore += ComputerHandUnmatched - PlayerHandUnmatched;
            else
                ComputerScore += PlayerHandUnmatched - ComputerHandUnmatched + 10;

            DealNewRound();
        }
    }
    else if(Input == "G")
    {
        if(PlayerCards.size() < 11 && SumUnmatchedMeld(PlayerCards) == 0)
        {
            PlayerScore += FindUnmatchedMeldWithPartner(ComputerCards, PlayerCards) + 20;

            DealNewRound();
        }
    }
    else
    {
        std::cout << "Unexpected Input of " << Input << std::endl;
    }
}

//////////////////////////////////////////////////////////////////////
/// Prints a console line of text with two strings. The total length
/// of the line will be LineLength long with the first parameter being
/// left aligned and the second parameter being right aligned
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::PrintLine(const std::string& Left, const std::string& Right) const
{
    int SpaceCount = LineLength - (Left.length() + Right.length());
    std::cout << Left << std::string(SpaceCount, ' ') << Right << std::endl;
}

//////////////////////////////////////////////////////////////////////
/// Prints a console line of text with three strings. The total length
/// of the line will be LineLength long with the first parameter being
/// left aligned, second parameter being center aligned, and the third
/// parameter being right aligned
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
/// Reviews a Hand for the optimal method of determining which cards
/// are (and are not) meld. The function will reset the isMeld member
/// on the Card class.
///
/// Returns: int (the sum of the unmelded card's values in the Hand)
//////////////////////////////////////////////////////////////////////
int GinRummy::FindUnmatchedMeld(std::vector<Card> &Hand) const
{
    for(Card& card : Hand)
        card.isMeld = false;

    std::vector<Card> RunsThenSets = Hand;
    std::vector<Card> SetsThenRuns = Hand;
    std::vector<Card> HoldMeld;
    int RunsThenSetsCount;
    int SetsThenRunsCount;

    // Attempt 1 - runs then sets
    SearchForRuns(RunsThenSets);
    RemoveMeld(RunsThenSets, HoldMeld);
    SearchForSets(RunsThenSets);
    AddMeld(RunsThenSets, HoldMeld);
    RunsThenSetsCount = SumUnmatchedMeld(RunsThenSets);

    // Attempt 2 - sets thenn runs
    SearchForSets(SetsThenRuns);
    RemoveMeld(SetsThenRuns, HoldMeld);
    SearchForRuns(SetsThenRuns);
    AddMeld(SetsThenRuns, HoldMeld);
    SetsThenRunsCount = SumUnmatchedMeld(SetsThenRuns);

    if(RunsThenSetsCount < SetsThenRunsCount)
    {
        Hand = RunsThenSets;
        return RunsThenSetsCount;
    }
    else
    {
        Hand = SetsThenRuns;
        return SetsThenRunsCount;
    }
}

//////////////////////////////////////////////////////////////////////
/// Reviews a Hand for the optimal method of determining which cards
/// are (and are not) meld. This function will attempt to meld the
/// contents in Hand with those in PartnerHand as well. The function
/// will reset the isMeld member on the Card class for the Hand vector
/// but will not modify the PartnerHand vector (as this function relies
/// on the fact that PartnerHand has already been melded.
///
/// Returns: int (the sum of the unmelded card's values in the Hand)
//////////////////////////////////////////////////////////////////////
int GinRummy::FindUnmatchedMeldWithPartner(std::vector<Card> &Hand, const std::vector<Card> &PartnerHand) const
{
    return FindUnmatchedMeld(Hand);
}

//////////////////////////////////////////////////////////////////////
/// Loops through a Hand of cards and keeps a running total of the
/// unmelded cards. This function relies upon the isMeld member of the
/// Cards being set correctly and will NOT try to find any meld combos
///
/// Returns: int (the sum of the unmelded card's values in the Hand)
//////////////////////////////////////////////////////////////////////
int GinRummy::SumUnmatchedMeld(const std::vector<Card> &Hand) const
{
    int Count = 0;
    for(const Card& card : Hand)
        if(!card.isMeld)
            Count = Count + Card::CardPoints(card);

    return Count;
}

//////////////////////////////////////////////////////////////////////
/// Loops through a Hand of cards and identifies runs of three or more
/// cards of the same suit.
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::SearchForRuns(std::vector<Card> &Hand) const
{
    std::sort(Hand.begin(), Hand.end(), Card::CompareForRuns);
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

//////////////////////////////////////////////////////////////////////
/// Loops through a Hand of cards and identifies sets of three or more
/// cards with the same value
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::SearchForSets(std::vector<Card> &Hand) const
{
    std::sort(Hand.begin(), Hand.end(), Card::CompareForSets);
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

//////////////////////////////////////////////////////////////////////
/// Loops through a Hand of cards and removes all of the melded cards
/// from that hand (and places them in Meld).
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////
/// Adds the cards in the Meld vector into the Hand vector
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::AddMeld(std::vector<Card> &Hand, std::vector<Card> &Meld) const
{
    Hand.insert(Hand.end(), Meld.begin(), Meld.end());
    Meld.clear();
}

//////////////////////////////////////////////////////////////////////
/// Should the user pickup the discarded card? The goal of the function
/// is to make the decision based upon whether or not the discarded
/// card will improve the user's probability of gin.
///
/// Returns: bool (true is pickup discard, false is pickup face down)
//////////////////////////////////////////////////////////////////////
bool GinRummy::PickupDiscard(const std::vector<Card>& Hand) const
{
    // for now at least force the computer to pickup the discard if they can meld it
    std::vector<Card> HandAfterDiscard = Hand;
    HandAfterDiscard.push_back(Discard.back());
    FindUnmatchedMeld(HandAfterDiscard);
    HandAfterDiscard.erase(HandAfterDiscard.begin() + IndexToDiscard(HandAfterDiscard));
    if(SumUnmatchedMeld(HandAfterDiscard) < SumUnmatchedMeld(Hand))
        return true;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 1);
    return distrib(gen);
}

//////////////////////////////////////////////////////////////////////
/// Which card should the user discard? The goal of the function is to
/// discard the card that is least like to be melded, with ties broken
/// against the card's value and randomly, if needed.
///
/// Returns: int (index of the vector of the card to discard)
//////////////////////////////////////////////////////////////////////
int GinRummy::IndexToDiscard(const std::vector<Card>& Hand) const
{
    if(SumUnmatchedMeld(Hand) == 0)
        return 0;

    while(true)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, Hand.size() - 1);
        int idx = distrib(gen);
        if(!Hand.at(idx).isMeld)
            return idx;
    }
}

//////////////////////////////////////////////////////////////////////
/// Should the user knock or play on to gin? The goal of the function
/// is to make this decision randomly with a bias toward being more
/// likely to making this decision with a lower meld count. Knocking
/// can only be done if the unmatched meld in the hand is 10 or less.
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
bool GinRummy::Knock(const std::vector<Card>& Hand) const
{
    return false;
}

//////////////////////////////////////////////////////////////////////
/// Computes the probability of gin for the Hand
///
/// Returns: double (probability of gin)
//////////////////////////////////////////////////////////////////////
double GinRummy::ProbabilityOfGin(const std::vector<Card>& Hand) const
{
    return 0.5;
}

//////////////////////////////////////////////////////////////////////
/// Computes the probability of meld for each card in the hand (and
/// sets the value accordingly in the vector)
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::CalculateProbabilityOfMeld(std::vector<Card>& Hand) const
{
}
