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
    ShowingComputerHand = false;
    SortingByRuns = true;

    DealNewRound();
}

//////////////////////////////////////////////////////////////////////
/// Creates the decks and deals the cards to the players
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::DealNewRound()
{
    FaceDownDeck.clear();
    PlayerHand.clear();
    ComputerHand.clear();
    DiscardDeck.clear();

    CanPass = true;
    JustPassed = false;

    FaceDownDeck.createDeck();
    FaceDownDeck.shuffle();

    FaceDownDeck.deal(PlayerHand, 10);
    FaceDownDeck.deal(ComputerHand, 10);

    //Turn over top card card place in the Discard pile
    DiscardDeck.push_back(FaceDownDeck.back());
    FaceDownDeck.pop_back();
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

    FindUnmatchedMeld(PlayerHand);
    FindUnmatchedMeld(ComputerHand);

    for(Card& card : DiscardDeck)
        card.meld = Card::NOTMELD;

    if(SortingByRuns)
    {
        PlayerHand.sortByRuns();
        ComputerHand.sortByRuns();
    }
    else
    {
        PlayerHand.sortBySets();
        ComputerHand.sortBySets();
    }

    for(int idx = 0; idx < PlayerHand.size(); ++idx) //5-14, maybe 15
    {
        std::string Left = std::to_string(idx+1) + ". " + PlayerHand.at(idx).CardToString();
        std::string Right;
        std::string Middle;
        if(idx < ComputerHand.size())
        {
            if(ShowingComputerHand)
                Right = std::to_string(idx+1) + ". " + ComputerHand.at(idx).CardToString();
            else
                Right = "Card " + std::to_string(idx+1);
        }
        if(idx == 1)
            Middle = "DISCARD PILE";
        else if(idx == 2)
            Middle = !DiscardDeck.empty() ? DiscardDeck.back().CardToString() : "";
        else if(idx == 5)
            Middle = "(F) - Take Face Down (" + std::to_string(FaceDownDeck.size()) + ")";
        else if(idx == 6)
            Middle = "(D) - Take Discard (" + std::to_string(DiscardDeck.size()) + ")";
        PrintLine(Left, Middle, Right);
    }

    if(PlayerHand.size() == 10) //15
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
        StatusMessage = SortByRuns();
    }
    else if(Input == "P")
    {
        StatusMessage = SortBySets();
    }
    else if(Input == "S")
    {
        StatusMessage = ShowComputerHand();
    }
    else if(Input == "H")
    {
        StatusMessage = HideComputerHand();
    }
    else if(Input == "A")
    {
        StatusMessage = Pass();
    }
    else if(Input == "Q")
    {
        exit(0);
    }
    else if(Input == "D")
    {
        StatusMessage = ChooseDiscard();
    }
    else if(Input == "F")
    {
        StatusMessage = ChooseFaceDown();
    }
    else if(!Input.empty() && Input.at(0) == 'D')
    {
        int idx = std::stoi(Input.substr(1)) - 1;
        StatusMessage = Discard(idx);
    }
    else if(Input == "C")
    {
        StatusMessage = ComputersTurn();
    }
    else if(Input == "K")
    {
        StatusMessage = Knock();
    }
    else if(Input == "G")
    {
        StatusMessage = Gin();
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
int GinRummy::FindUnmatchedMeld(Hand& hand, bool ResetMeld)
{
    if(ResetMeld)
        for(Card& card : hand)
            card.meld = Card::NOTMELD;

    Hand RunsThenSets = hand;
    Hand SetsThenRuns = hand;
    int RunsThenSetsSum;
    int SetsThenRunsSum;

    // Attempt 1 - runs then sets
    RunsThenSets.searchForRuns();
    RunsThenSets.searchForSets();
    RunsThenSetsSum = SumUnmatchedMeld(RunsThenSets);

    // Attempt 2 - sets thenn runs
    SetsThenRuns.searchForSets();
    SetsThenRuns.searchForRuns();
    SetsThenRunsSum = SumUnmatchedMeld(SetsThenRuns);

    if(RunsThenSetsSum < SetsThenRunsSum)
    {
        hand = RunsThenSets;
        CalculateProbabilityOfMeld(hand);
        return RunsThenSetsSum;
    }
    else
    {
        hand = SetsThenRuns;
        CalculateProbabilityOfMeld(hand);
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
int GinRummy::FindUnmatchedMeldWithPartner(Hand& hand, const Hand &PartnerHand)
{
    Hand PartnerMeld;
    for(Card& C : hand)
        C.meld = Card::NOTMELD;

    for(const Card& C : PartnerHand)
    {
        if(C.isMeld())
        {
            PartnerMeld.push_back(C);
            hand.push_back(C);
        }
    }

    FindUnmatchedMeld(hand, false);

    for(const Card& C : PartnerMeld)
        hand.remove(C);

    return SumUnmatchedMeld(hand);
}

//////////////////////////////////////////////////////////////////////
/// Loops through a Hand of cards and keeps a running total of the
/// unmelded cards. This function relies upon the isMeld member of the
/// Cards being set correctly and will NOT try to find any meld combos
///
/// Returns: int (the sum of the unmelded card's values in the Hand)
//////////////////////////////////////////////////////////////////////
int GinRummy::SumUnmatchedMeld(const Hand& hand) const
{
    int Count = 0;
    for(const Card& card : hand)
        if(!card.isMeld())
            Count = Count + card.CardPoints();

    return Count;
}

//////////////////////////////////////////////////////////////////////
/// Should the user pickup the discarded card? The goal of the function
/// is to make the decision based upon whether or not the discarded
/// card will improve the user's probability of gin.
///
/// Returns: bool (true is pickup discard, false is pickup face down)
//////////////////////////////////////////////////////////////////////
bool GinRummy::PickupDiscard(const Hand& hand)
{
    double ProbGinAsIs = ProbabilityOfGin(hand);
    Hand TempHand = hand;
    TempHand.push_back(DiscardDeck.back());
    FindUnmatchedMeld(TempHand);
    int idx = IndexToDiscard(TempHand);
    if(TempHand.at(idx) == DiscardDeck.back())
    {
        return false;
    }
    TempHand.erase(TempHand.begin() + idx);
    FindUnmatchedMeld(TempHand);
    double ProbGinDiscardDeck = ProbabilityOfGin(TempHand);
    if(ProbGinDiscardDeck > ProbGinAsIs)
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
int GinRummy::IndexToDiscard(const Hand& hand) const
{
    // Discard the card with the lowest probability of meld
    double MinProbability = hand.at(0).probOfMeld;
    std::vector<int> MinProbIdxs = { 0 };
    for(int idx = 1; idx < hand.size(); ++idx)
    {
        if(hand.at(idx).probOfMeld < MinProbability)
        {
            MinProbability = hand.at(idx).probOfMeld;
            MinProbIdxs.clear();
            MinProbIdxs.push_back(idx);
        }
        else if(hand.at(idx).probOfMeld == MinProbability)
        {
            MinProbIdxs.push_back(idx);
        }
    }
    if(MinProbIdxs.size() == 1)
        return MinProbIdxs.at(0);

    // If there is a tie, discard the card with the highest value (i.e. Kings, Queens, etc)
    Card::Value MaxValue = hand.at(MinProbIdxs.at(0)).value;
    int MaxValueIdx = MinProbIdxs.at(0);
    for(int idx = 1; idx < MinProbIdxs.size(); ++idx)
    {
        if(hand.at(MinProbIdxs.at(idx)).value > MaxValue)
        {
            MaxValue = hand.at(MinProbIdxs.at(idx)).value;
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
bool GinRummy::Knock(const Hand& hand) const
{
    int UnmatchedMeldSum = SumUnmatchedMeld(hand);
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
double GinRummy::ProbabilityOfGin(const Hand& hand) const
{
    double product = 1.0;
    for(int idx = 0; idx < hand.size(); ++idx)
    {
        product *= hand.at(idx).probOfMeld;
    }
    return product;
}

//////////////////////////////////////////////////////////////////////
/// Computes the probability of meld for each card in the hand (and
/// sets the value accordingly in the vector)
///
/// Returns: void
//////////////////////////////////////////////////////////////////////
void GinRummy::CalculateProbabilityOfMeld(Hand& hand)
{
    if(hand.empty())
        return;

    for(Card& C : hand)
    {
        C.OneFromMeld = 0;
        C.TwoFromMeld = 0;
    }

    // Calculate for Sets First
    hand.sortBySets();

    //Check One From Meld - Sets
    for(int idx = 0; idx < hand.size() - 1; ++idx)
    {
        if(hand.at(idx).isMeld())
            continue;

        if(hand.at(idx).value == hand.at(idx + 1).value)
        {
            for(int s = Card::DIAMOND; s <= Card::SPADE; ++s)
            {
                Card CheckCard((Card::Suit)s, hand.at(idx).value);
                if(CheckCard != hand.at(idx) && CheckCard !=  hand.at(idx + 1))
                {
                    if(!DiscardDeck.contains(CheckCard))
                    {
                        ++(hand.at(idx).OneFromMeld);
                        ++(hand.at(idx + 1).OneFromMeld);
                    }
                }
            }
        }
    }

    //Check Two From Meld - Sets
    for(int idx = 0; idx < hand.size(); ++idx)
    {
        if(hand.at(idx).isMeld())
            continue;

        for(int s1 = Card::DIAMOND; s1 <= Card::SPADE; ++s1)
        {
            if(s1 == hand.at(idx).suit)
                continue;

            Card CheckCard1((Card::Suit)s1, hand.at(idx).value);
            if(DiscardDeck.contains(CheckCard1))
                continue;

            for(int s2 = Card::DIAMOND; s2 <= Card::SPADE; ++s2)
            {
                if(s2 == hand.at(idx).suit || s1 == s2)
                    continue;

                Card CheckCard2((Card::Suit)s2, hand.at(idx).value);
                if(!DiscardDeck.contains(CheckCard2))
                {
                    ++(hand.at(idx).TwoFromMeld);
                }
            }
        }
    }

    // Calculate for Runs Second
    hand.sortByRuns();

    //Check One From Meld - Runs
    for(int idx = 0; idx < hand.size() - 1; ++idx)
    {
        if(hand.at(idx).isMeld())
            continue;

        if(hand.at(idx).suit == hand.at(idx + 1).suit && hand.at(idx).value == hand.at(idx + 1).value - 1)//6,7 example
        {
            // Check Left
            Card CheckCard1(hand.at(idx).suit, Card::Value(hand.at(idx).value - 1));
            if(CheckCard1.isValid() && !DiscardDeck.contains(CheckCard1))
            {
                ++(hand.at(idx).OneFromMeld);
                ++(hand.at(idx + 1).OneFromMeld);
            }

            // Check Right
            Card CheckCard2(hand.at(idx).suit, Card::Value(hand.at(idx).value + 2));
            if(CheckCard2.isValid() && !DiscardDeck.contains(CheckCard2))
            {
                ++(hand.at(idx).OneFromMeld);
                ++(hand.at(idx + 1).OneFromMeld);
            }
        }
        else if(hand.at(idx).suit == hand.at(idx + 1).suit && hand.at(idx).value == hand.at(idx + 1).value - 2)//3,5 example
        {
            Card CheckCard(hand.at(idx).suit, Card::Value(hand.at(idx).value + 1));
            if(!DiscardDeck.contains(CheckCard))
            {
                ++(hand.at(idx).OneFromMeld);
                ++(hand.at(idx + 1).OneFromMeld);
            }
        }
    }

    //Check Two From Meld - Runs
    for(int idx = 0; idx < hand.size(); ++idx)
    {
        if(hand.at(idx).isMeld())
            continue;

        Card CheckCardL1(hand.at(idx).suit, Card::Value(hand.at(idx).value - 1));
        Card CheckCardL2(hand.at(idx).suit, Card::Value(hand.at(idx).value - 2));

        Card CheckCardR1(hand.at(idx).suit, Card::Value(hand.at(idx).value + 1));
        Card CheckCardR2(hand.at(idx).suit, Card::Value(hand.at(idx).value + 2));

        //Check for two left
        if(CheckCardL1.isValid() && !DiscardDeck.contains(CheckCardL1) &&
                CheckCardL2.isValid() && !DiscardDeck.contains(CheckCardL2))
            ++(hand.at(idx).TwoFromMeld);

        //Check for two right
        if(CheckCardR1.isValid() && !DiscardDeck.contains(CheckCardR1) &&
                CheckCardR2.isValid() && !DiscardDeck.contains(CheckCardR2))
            ++(hand.at(idx).TwoFromMeld);

        //Check for one on both sides
        if(CheckCardL1.isValid() && !DiscardDeck.contains(CheckCardL1) &&
                CheckCardR1.isValid() && !DiscardDeck.contains(CheckCardR1))
            ++(hand.at(idx).TwoFromMeld);
    }

    // Calculate Probability
    for(Card& C : hand)
    {
        if(C.isMeld())
        {
            C.probOfMeld = 1.0;
        }
        else
        {
            C.probOfMeld = ((double)C.OneFromMeld / (double)FaceDownDeck.size()) + std::pow((double)C.TwoFromMeld / (double)FaceDownDeck.size(),2);
            if(C.probOfMeld > 1.0)
                C.probOfMeld = 0.99;
        }
    }
}

//////////////////////////////////////////////////////////////////////
/// Sets the flag to sort the hand by runs
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::SortByRuns()
{
    SortingByRuns = true;
    return "Sorted hands by runs";
}

//////////////////////////////////////////////////////////////////////
/// Sets the flag to sort the hand by sets
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::SortBySets()
{
    SortingByRuns = false;
    return "Sorted hands by sets";
}

//////////////////////////////////////////////////////////////////////
/// Sets the flag to display the compuer hand
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::ShowComputerHand()
{
    ShowingComputerHand = true;
    return "Showed computer hand";
}

//////////////////////////////////////////////////////////////////////
/// Sets the flag to hide the computer hand
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::HideComputerHand()
{
    ShowingComputerHand = false;
    return "Hid computer hand";
}

//////////////////////////////////////////////////////////////////////
/// Passes from picking up the discard
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::Pass()
{
    PlayerTurn = !PlayerTurn;
    JustPassed = true;
    return "You passed";
}

//////////////////////////////////////////////////////////////////////
/// Picks up a card from the discard pile for the player
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::ChooseDiscard()
{
    if(PlayerTurn && PlayerHand.size() < 11 && !DiscardDeck.empty())
    {
        PlayerHand.push_back(DiscardDeck.back());
        DiscardDeck.pop_back();
        CanPass = false;
        JustPassed = false;
        return "Chose " + PlayerHand.back().CardToString() + " from discard pile";
    }
    else
    {
        return "Error - unable to choose from discard pile";
    }
}

//////////////////////////////////////////////////////////////////////
/// Picks up a card from the face down pile for the player
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::ChooseFaceDown()
{
    if(JustPassed)
    {
        return "Error - computer passed, you can't pickup the face down card";
    }
    else if(PlayerTurn && PlayerHand.size() < 11 && !FaceDownDeck.empty())
    {
        PlayerHand.push_back(FaceDownDeck.back());
        FaceDownDeck.pop_back();
        CanPass = false;
        JustPassed = false;
        return  "Chose " + PlayerHand.back().CardToString() + " from face down pile";
    }
    else
    {
        return "Error - unable to choose from face down pile";
    }
}

//////////////////////////////////////////////////////////////////////
/// Discards the card at indedx idx from the player's hand
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::Discard(int idx)
{
    if(PlayerTurn && PlayerHand.size() > 10)
    {
        if(idx >= 0 && idx < PlayerHand.size())
        {
            DiscardDeck.push_back(PlayerHand.at(idx));
            PlayerHand.at(idx) = PlayerHand.back();
            PlayerHand.pop_back();
            PlayerTurn = false;
            return "You discarded " + DiscardDeck.back().CardToString();
        }
    }

    return "Error - unable to discard " + std::to_string(idx);
}

//////////////////////////////////////////////////////////////////////
/// Computer's Turn
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::ComputersTurn()
{
    std::string StatusMessage;
    
    if(!PlayerTurn)
    {
        if(PickupDiscard(ComputerHand))
        {
            ComputerHand.push_back(DiscardDeck.back());
            DiscardDeck.pop_back();
            StatusMessage = "Computer chose " + ComputerHand.back().CardToString() + " from discards";
        }
        else if(JustPassed)
        {
            JustPassed = false;
            return "Computer Passed";
        }
        else if(CanPass)
        {
            CanPass = false;
            JustPassed = true;
            return "Computer Passed";
        }
        else
        {
            ComputerHand.push_back(FaceDownDeck.back());
            FaceDownDeck.pop_back();
            StatusMessage = "Computer chose from the deck";
        }

        FindUnmatchedMeld(ComputerHand);

        int idx = IndexToDiscard(ComputerHand);
        DiscardDeck.push_back(ComputerHand.at(idx));
        StatusMessage += " and discarded " + DiscardDeck.back().CardToString();
        ComputerHand.erase(ComputerHand.begin() + idx);

        FindUnmatchedMeld(ComputerHand);

        if(ComputerHand.size() < 11 && SumUnmatchedMeld(ComputerHand) == 0)
        {
            int handUnmatched = FindUnmatchedMeldWithPartner(PlayerHand, ComputerHand);
            ComputerScore += handUnmatched + 20;
            DealNewRound();
            StatusMessage += " Computer ginned and gained " + std::to_string(handUnmatched + 20) + " points. Try again.";
        }
        else if(Knock(ComputerHand))
        {
            int ComputerHandUnmatched = SumUnmatchedMeld(ComputerHand);
            int handUnmatched = FindUnmatchedMeldWithPartner(PlayerHand, ComputerHand);

            if(ComputerHandUnmatched < handUnmatched)
            {
                ComputerScore += handUnmatched - ComputerHandUnmatched;
                StatusMessage += " Computer knocked and gained " + std::to_string(handUnmatched - ComputerHandUnmatched) + " points.";
            }
            else
            {
                PlayerScore += ComputerHandUnmatched - handUnmatched + 10;
                StatusMessage += " Computer knocked and lost. You gained " + std::to_string(ComputerHandUnmatched - handUnmatched + 10) + " points.";
            }
            DealNewRound();
        }
        PlayerTurn = true;
    }
    else
    {
        StatusMessage = "Error - Player Turn";
    }

    return StatusMessage;
}

//////////////////////////////////////////////////////////////////////
/// Knocks for the player
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::Knock()
{
    if(PlayerHand.size() < 11 && SumUnmatchedMeld(PlayerHand) < 11)
    {
        int handUnmatched = SumUnmatchedMeld(PlayerHand);
        int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerHand, PlayerHand);
        
        DealNewRound();

        if(handUnmatched < ComputerHandUnmatched)
        {
            PlayerScore += ComputerHandUnmatched - handUnmatched;
            return "CONGRATULATIONS! You knocked with " + std::to_string(handUnmatched) +
                    " points. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
        }
        else
        {
            ComputerScore += handUnmatched - ComputerHandUnmatched + 10;
            return "GOOD TRY! You knocked with " + std::to_string(handUnmatched) +
                    " points. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
        }
    }
    else
    {
        return "Error - you are unable to knock";
    }
}

//////////////////////////////////////////////////////////////////////
/// Gin for the player
///
/// Returns: std::string (status message)
//////////////////////////////////////////////////////////////////////
std::string GinRummy::Gin()
{
    if(PlayerHand.size() < 11 && SumUnmatchedMeld(PlayerHand) == 0)
    {
        int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerHand, PlayerHand);
        PlayerScore += ComputerHandUnmatched + 20;

        DealNewRound();

        return "CONGRATULATIONS! You ginned. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
    }
    else
    {
        return "Unable to gin, your unmatched meld is " + std::to_string(SumUnmatchedMeld(PlayerHand));
    }
}
