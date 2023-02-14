#include "GinRummy.h"

#include <iostream>
#include <random>
#include <algorithm>

int GinRummy::LineLength = 80;

GinRummy::GinRummy()
{
    PlayerScore = 0;
    ComputerScore = 0;

    PlayerTurn = true;
    ShowComputerHand = false;
    SortByRuns = true;

    DealNewRound();
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
    for(int s = Card::DIAMOND; s <= Card::SPADE; ++s)
    {
        for(int n = Card::ACE; n <= Card::KING; ++n)
        {
            Card card((Card::Suit)s, (Card::Value)n);
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
    std::string StatusMessage = "Welcome! Good Luck.";
    while(true)
    {
        DrawGame(StatusMessage);
        StatusMessage = UserInput();
    }
}

//////////////////////////////////////////////////////////////////////
/// Prints the current state of the game
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::DrawGame(const std::string& StatusMessage)
{
    static const std::string border(LineLength, 'x');

    std::cout << border << std::endl; //1
    std::cout << std::endl; //2

    std::string LineLeft; //start to build line 3
    LineLeft = "YOU (" + std::to_string(PlayerScore) + " points)";
    if(PlayerTurn)
        LineLeft += " - your turn";
    std::string LineRight;
    if(!PlayerTurn)
        LineRight += "your turn - ";
    LineRight += "(" + std::to_string(ComputerScore) + " points) COMPUTER";
    PrintLine(LineLeft, LineRight); //3

    std::cout << std::endl; //4

    FindUnmatchedMeld(PlayerCards);
    FindUnmatchedMeld(ComputerCards);

    for(Card& card : Discard)
        card.meld = Card::NOTMELD;

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
        std::string Left = std::to_string(idx+1) + ". " + PlayerCards.at(idx).CardToString();
        std::string Right;
        std::string Middle;
        if(idx < ComputerCards.size())
        {
            if(ShowComputerHand)
                Right = std::to_string(idx+1) + ". " + ComputerCards.at(idx).CardToString();
            else
                Right = "Card " + std::to_string(idx+1);
        }
        if(idx == 1)
            Middle = "DISCARD PILE";
        else if(idx == 2)
            Middle = !Discard.empty() ? Discard.back().CardToString() : "";
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
    PrintLine("(A) - Pass", "(G) - Gin"); //21
    PrintLine("(C) - Computer Play", "(Q) - Quit"); //22
    std::cout << border << std::endl; //23
    std::cout << "**" << StatusMessage << "** Enter Command: "; //24
}

//////////////////////////////////////////////////////////////////////
/// Accepts input from the user and responds accordingly
///
/// Returns: std::string (status message from turns)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::UserInput()
{
    std::string Input;
    std::cin >> Input;

    std::transform(Input.begin(), Input.end(), Input.begin(), ::toupper);

    std::string StatusMessage;

    if(Input == "R")
    {
        StatusMessage = "Sorted hands by runs";
        SortByRuns = true;
    }
    else if(Input == "P")
    {
        StatusMessage = "Sorted hands by sets";
        SortByRuns = false;
    }
    else if(Input == "S")
    {
        StatusMessage = "Showed computer hand";
        ShowComputerHand = true;
    }
    else if(Input == "H")
    {
        StatusMessage = "Hid computer hand";
        ShowComputerHand = false;
    }
    else if(Input == "A")
    {
        StatusMessage = "Pass not implemented yet";
        //TODO Implement pass
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
            StatusMessage = "Chose " + PlayerCards.back().CardToString() + " from discard pile";
        }
        else
        {
            StatusMessage = "Error - unable to choose from discard pile";
        }
    }
    else if(Input == "F")
    {
        if(PlayerTurn && PlayerCards.size() < 11 && !Deck.empty())
        {
            PlayerCards.push_back(Deck.back());
            Deck.pop_back();
            StatusMessage = "Chose " + PlayerCards.back().CardToString() + " from face down pile";
        }
        else
        {
            StatusMessage = "Error - unable to choose from face down pile";
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
                StatusMessage = "You discarded " + Discard.back().CardToString();
                PlayerTurn = false;
            }
            else
            {
                StatusMessage = "Error - unable to discard " + idx;
            }
        }
        else
        {
            StatusMessage = "Error - unable to discard";
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
                StatusMessage = "Computer chose " + ComputerCards.back().CardToString() + " from discards";
            }
            else
            {
                ComputerCards.push_back(Deck.back());
                Deck.pop_back();
                StatusMessage = "Computer chose from the deck";
            }

            FindUnmatchedMeld(ComputerCards);

            int idx = IndexToDiscard(ComputerCards);
            Discard.push_back(ComputerCards.at(idx));
            StatusMessage += " and discarded " + Discard.back().CardToString();
            ComputerCards.erase(ComputerCards.begin() + idx);

            FindUnmatchedMeld(ComputerCards);

            if(ComputerCards.size() < 11 && SumUnmatchedMeld(ComputerCards) == 0)
            {
                int PlayerHandUnmatched = FindUnmatchedMeldWithPartner(PlayerCards, ComputerCards);
                ComputerScore += PlayerHandUnmatched + 20;
                DealNewRound();
                StatusMessage += " Computer ginned and gained " + std::to_string(PlayerHandUnmatched + 20) + " points. Try again.";
            }
            else if(Knock(ComputerCards))
            {
                int ComputerHandUnmatched = SumUnmatchedMeld(ComputerCards);
                int PlayerHandUnmatched = FindUnmatchedMeldWithPartner(PlayerCards, ComputerCards);

                if(ComputerHandUnmatched < PlayerHandUnmatched)
                {
                    ComputerScore += PlayerHandUnmatched - ComputerHandUnmatched;
                    StatusMessage += " Computer knocked and gained " + std::to_string(PlayerHandUnmatched - ComputerHandUnmatched) + " points.";
                }
                else
                {
                    PlayerScore += ComputerHandUnmatched - PlayerHandUnmatched + 10;
                    StatusMessage += " Computer knocked and lost. You gained " + std::to_string(ComputerHandUnmatched - PlayerHandUnmatched + 10) + " points.";
                }
                DealNewRound();
            }
            PlayerTurn = true;
        }
        else
        {
            StatusMessage = "Error - Player Turn";
        }
    }
    else if(Input == "K")
    {
        if(PlayerCards.size() < 11 && SumUnmatchedMeld(PlayerCards) < 11)
        {
            int PlayerHandUnmatched = SumUnmatchedMeld(PlayerCards);
            int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerCards, PlayerCards);

            if(PlayerHandUnmatched < ComputerHandUnmatched)
            {
                PlayerScore += ComputerHandUnmatched - PlayerHandUnmatched;
                StatusMessage = "CONGRATULATIONS! You knocked with " + std::to_string(PlayerHandUnmatched) +
                        " points. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
            }
            else
            {
                ComputerScore += PlayerHandUnmatched - ComputerHandUnmatched + 10;
                StatusMessage = "GOOD TRY! You knocked with " + std::to_string(PlayerHandUnmatched) +
                        " points. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
            }

            DealNewRound();
        }
        else
        {
            StatusMessage = "Error - you are unable to knock";
        }
    }
    else if(Input == "G")
    {
        if(PlayerCards.size() < 11 && SumUnmatchedMeld(PlayerCards) == 0)
        {
            int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerCards, PlayerCards);
            PlayerScore += ComputerHandUnmatched + 20;

            DealNewRound();

            StatusMessage = "CONGRATULATIONS! You ginned. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
        }
        else
        {
            StatusMessage = "Unable to gin, your unmatched meld is " + std::to_string(SumUnmatchedMeld(PlayerCards));
        }
    }
    else
    {
        StatusMessage = "ERROR: Unexpected Input of " + Input;
    }

    return StatusMessage;
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
int GinRummy::FindUnmatchedMeld(std::vector<Card> &Hand, bool ResetMeld) const
{
    if(ResetMeld)
        for(Card& card : Hand)
            card.meld = Card::NOTMELD;

    std::vector<Card> RunsThenSets = Hand;
    std::vector<Card> SetsThenRuns = Hand;
    int RunsThenSetsSum;
    int SetsThenRunsSum;

    // Attempt 1 - runs then sets
    SearchForRuns(RunsThenSets);
    SearchForSets(RunsThenSets);
    RunsThenSetsSum = SumUnmatchedMeld(RunsThenSets);

    // Attempt 2 - sets thenn runs
    SearchForSets(SetsThenRuns);
    SearchForRuns(SetsThenRuns);
    SetsThenRunsSum = SumUnmatchedMeld(SetsThenRuns);

    if(RunsThenSetsSum < SetsThenRunsSum)
    {
        Hand = RunsThenSets;
        CalculateProbabilityOfMeld(Hand);
        return RunsThenSetsSum;
    }
    else
    {
        Hand = SetsThenRuns;
        CalculateProbabilityOfMeld(Hand);
        return SetsThenRunsSum;
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
    std::vector<Card> PartnerMeld;
    for(Card& C : Hand)
        C.meld = Card::NOTMELD;

    for(const Card& C : PartnerHand)
    {
        if(C.isMeld())
        {
            PartnerMeld.push_back(C);
            Hand.push_back(C);
        }
    }

    FindUnmatchedMeld(Hand, false);

    for(const Card& C : PartnerMeld)
    {
        //TODO Remove Card C from Hand
    }

    return SumUnmatchedMeld(Hand);
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
        if(!card.isMeld())
            Count = Count + card.CardPoints();

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
            if(Hand.at(idx).meld != Card::SETMELD && Hand.at(idx - 1).meld != Card::SETMELD && Hand.at(idx - 2).meld != Card::SETMELD)
            {
                Hand.at(idx).meld = Card::RUNMELD;
                Hand.at(idx - 1).meld = Card::RUNMELD;
                Hand.at(idx - 2).meld = Card::RUNMELD;
            }
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
            if(Hand.at(idx).meld != Card::RUNMELD && Hand.at(idx - 1).meld != Card::RUNMELD && Hand.at(idx - 2).meld != Card::RUNMELD)
            {
                Hand.at(idx).meld = Card::SETMELD;
                Hand.at(idx - 1).meld = Card::SETMELD;
                Hand.at(idx - 2).meld = Card::SETMELD;
            }
        }
    }
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
    double ProbGinAsIs = ProbabilityOfGin(Hand);
    std::vector<Card> TempHand = Hand;
    TempHand.push_back(Discard.back());
    FindUnmatchedMeld(TempHand);
    int idx = IndexToDiscard(TempHand);
    TempHand.erase(TempHand.begin() + idx);
    FindUnmatchedMeld(TempHand);
    double ProbGinDiscard = ProbabilityOfGin(TempHand);
    if(ProbGinDiscard > ProbGinAsIs)
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////
/// Which card should the user discard? The goal of the function is to
/// discard the card that is least like to be melded, with ties broken
/// against the card's value and randomly, if needed.
///
/// Returns: int (index of the vector of the card to discard)
//////////////////////////////////////////////////////////////////////
int GinRummy::IndexToDiscard(const std::vector<Card> &Hand) const
{
    // TODO This apporach can lead to a discarding of cards that will break
    // meld in a situation where all 11 cards are melded (i.e. gin with 11)

    // Discard the card with the lowest probability of meld
    double MinProbability = Hand.at(0).probOfMeld;
    std::vector<int> MinProbIdxs = { 0 };
    for(int idx = 1; idx < Hand.size(); ++idx)
    {
        if(Hand.at(idx).probOfMeld < MinProbability)
        {
            MinProbability = Hand.at(idx).probOfMeld;
            MinProbIdxs.clear();
            MinProbIdxs.push_back(idx);
        }
        else if(Hand.at(idx).probOfMeld == MinProbability)
        {
            MinProbIdxs.push_back(idx);
        }
    }
    if(MinProbIdxs.size() == 1)
        return MinProbIdxs.at(0);

    // If there is a tie, discard the card with the highest value (i.e. Kings, Queens, etc)
    Card::Value MaxValue = Hand.at(MinProbIdxs.at(0)).value;
    int MaxValueIdx = MinProbIdxs.at(0);
    for(int idx = 1; idx < MinProbIdxs.size(); ++idx)
    {
        if(Hand.at(MinProbIdxs.at(idx)).value > MaxValue)
        {
            MaxValue = Hand.at(MinProbIdxs.at(idx)).value;
            MaxValueIdx = MinProbIdxs.at(idx);
        }
    }
    return MaxValueIdx;
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
    int UnmatchedMeldSum = SumUnmatchedMeld(Hand);
    if(UnmatchedMeldSum > 10)
        return false;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::geometric_distribution<int> d;

    if(d(gen) > UnmatchedMeldSum)
        return true;
    else
        return false;
}

//////////////////////////////////////////////////////////////////////
/// Computes the probability of gin for the Hand
///
/// Returns: double (probability of gin)
//////////////////////////////////////////////////////////////////////
double GinRummy::ProbabilityOfGin(const std::vector<Card>& Hand) const
{
    double product = 1.0;
    for(int idx = 0; idx < Hand.size(); ++idx)
    {
        product *= Hand.at(idx).probOfMeld;
    }
    return product;
}

//////////////////////////////////////////////////////////////////////
/// Computes the probability of meld for each card in the hand (and
/// sets the value accordingly in the vector)
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::CalculateProbabilityOfMeld(std::vector<Card>& Hand) const
{
    for(int idx = 0; idx < Hand.size(); ++idx)
    {
        if(Hand.at(idx).isMeld())
            Hand.at(idx).probOfMeld = 1.0;
        else
            Hand.at(idx).probOfMeld = 0.25;
    }
}
