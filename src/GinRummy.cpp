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
    FaceDownDeck.clear();
    PlayerHand.clear();
    ComputerHand.clear();
    DiscardDeck.clear();

    FaceDownDeck.createDeck();
    FaceDownDeck.shuffle();

    //Deal each player 10 cards
    for(int d = 0; d < 10; ++d)
    {
        PlayerHand.push_back(FaceDownDeck.back());
        FaceDownDeck.pop_back();

        ComputerHand.push_back(FaceDownDeck.back());
        FaceDownDeck.pop_back();
    }

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

    if(SortByRuns)
    {
        std::sort(PlayerHand.begin(), PlayerHand.end(), Card::CompareForRuns);
        std::sort(ComputerHand.begin(), ComputerHand.end(), Card::CompareForRuns);
    }
    else
    {
        std::sort(PlayerHand.begin(), PlayerHand.end(), Card::CompareForSets);
        std::sort(ComputerHand.begin(), ComputerHand.end(), Card::CompareForSets);
    }

    for(int idx = 0; idx < PlayerHand.size(); ++idx) //5-14, maybe 15
    {
        std::string Left = std::to_string(idx+1) + ". " + PlayerHand.at(idx).CardToString();
        std::string Right;
        std::string Middle;
        if(idx < ComputerHand.size())
        {
            if(ShowComputerHand)
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
        if(PlayerTurn && PlayerHand.size() < 11 && !DiscardDeck.empty())
        {
            PlayerHand.push_back(DiscardDeck.back());
            DiscardDeck.pop_back();
            StatusMessage = "Chose " + PlayerHand.back().CardToString() + " from discard pile";
        }
        else
        {
            StatusMessage = "Error - unable to choose from discard pile";
        }
    }
    else if(Input == "F")
    {
        if(PlayerTurn && PlayerHand.size() < 11 && !FaceDownDeck.empty())
        {
            PlayerHand.push_back(FaceDownDeck.back());
            FaceDownDeck.pop_back();
            StatusMessage = "Chose " + PlayerHand.back().CardToString() + " from face down pile";
        }
        else
        {
            StatusMessage = "Error - unable to choose from face down pile";
        }
    }
    else if(!Input.empty() && Input.at(0) == 'D')
    {
        if(PlayerTurn && PlayerHand.size() > 10)
        {
            int idx = std::stoi(Input.substr(1)) - 1;
            if(idx >= 0 && idx < PlayerHand.size())
            {
                DiscardDeck.push_back(PlayerHand.at(idx));
                PlayerHand.at(idx) = PlayerHand.back();
                PlayerHand.pop_back();
                StatusMessage = "You discarded " + DiscardDeck.back().CardToString();
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
            if(PickupDiscard(ComputerHand))
            {
                ComputerHand.push_back(DiscardDeck.back());
                DiscardDeck.pop_back();
                StatusMessage = "Computer chose " + ComputerHand.back().CardToString() + " from discards";
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
    }
    else if(Input == "K")
    {
        if(PlayerHand.size() < 11 && SumUnmatchedMeld(PlayerHand) < 11)
        {
            int handUnmatched = SumUnmatchedMeld(PlayerHand);
            int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerHand, PlayerHand);

            if(handUnmatched < ComputerHandUnmatched)
            {
                PlayerScore += ComputerHandUnmatched - handUnmatched;
                StatusMessage = "CONGRATULATIONS! You knocked with " + std::to_string(handUnmatched) +
                        " points. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
            }
            else
            {
                ComputerScore += handUnmatched - ComputerHandUnmatched + 10;
                StatusMessage = "GOOD TRY! You knocked with " + std::to_string(handUnmatched) +
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
        if(PlayerHand.size() < 11 && SumUnmatchedMeld(PlayerHand) == 0)
        {
            int ComputerHandUnmatched = FindUnmatchedMeldWithPartner(ComputerHand, PlayerHand);
            PlayerScore += ComputerHandUnmatched + 20;

            DealNewRound();

            StatusMessage = "CONGRATULATIONS! You ginned. The computer had " + std::to_string(ComputerHandUnmatched) + " points.";
        }
        else
        {
            StatusMessage = "Unable to gin, your unmatched meld is " + std::to_string(SumUnmatchedMeld(PlayerHand));
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
    // TODO This apporach can lead to a discarding of cards that will break
    // meld in a situation where all 11 cards are melded (i.e. gin with 11)

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
    std::sort(hand.begin(), hand.end(), Card::CompareForSets);

    //Check One From Meld
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

    //Check Two From Meld
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
/*SortByRuns(Hand)

For Each Card in Hand
    If Card.isMeld
        Skip Card

    //Check One Meld
    If ((Card.Suite == Card.Next.Suite) &&
         (Card.Value == Card.Next.Value - 1) || // 6,7, example

        // Check Left
        CheckCard.Suite = Card.Suite
        CheckCard.Value = Card.Value - 1
        If IsValidCard(CheckCard) And CheckCard Not InDiscardDeck
        ++Card.OneFromMeld

        // Check Right
        CheckCard.Suite = Card.Suite
        CheckCard.Value = Card.Value + 2
        If IsValidCard(CheckCard) And CheckCard Not InDiscardDeck
        ++Card.OneFromMeld

    Else If (Card.Suite == Card.Next.Suite &&
         Card.Value == Card.Next.Value - 2) // 6,8, example
        // Check Middle
        CheckCard.Suite = Card.Suite
        CheckCard.Value = Card.Value + 1
        If CheckCard Not In DiscardDeck
        ++Card.OneFromMeld

//Check Two Away From Meld
    Check1.Suite = Card.Suite
    Check2.Suite = Card.Suite

        //Check for two left
        Check1.Value = Card.Value - 2
        Check2.Value = Card.Value - 1
        If(IsValid(Check1) && Check1 is Not In DiscardDeck &&
               IsValid(Check2) && Check2 is Not In DiscardDeck)
            ++Card.TwoFromMeld

        //Check for two right
        Check1.Value = Card.Value + 2
        Check2.Value = Card.Value + 1
        If(IsValid(Check1) && Check1 is Not In DiscardDeck &&
               IsValid(Check2) && Check2 is Not In DiscardDeck)
            ++Card.TwoFromMeld

        //Check for one left, one right
        Check1.Value = Card.Value + 1
        Check2.Value = Card.Value - 1
        If(IsValid(Check1) && Check1 is Not In DiscardDeck &&
               IsValid(Check2) && Check2 is Not In DiscardDeck)
            ++Card.TwoFromMeld

*/
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
