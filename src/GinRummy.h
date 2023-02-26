#ifndef GINRUMMY_H
#define GINRUMMY_H

#include "Card.h"
#include "Deck.h"

#include <vector>
#include <string>

#include <gtest/gtest.h>

class GinRummy
{
public:
    GinRummy();

    void Play();

    std::string SortByRuns();
    std::string SortBySets();
    std::string ShowComputerHand();
    std::string HideComputerHand();
    std::string Pass();//TODO
    std::string Discard();

    unsigned int getPlayerScore() const { return PlayerScore; }
    unsigned int getComputerScore() const { return ComputerScore; }

    Hand getPlayerHand() const { return PlayerHand; }
    Hand getComputerHand() const { return ComputerHand; }

    bool getShowingComputerHand() const { return ShowingComputerHand; }

private:
    Deck FaceDownDeck;
    Deck DiscardDeck;

    Hand PlayerHand;
    Hand ComputerHand;

    unsigned int PlayerScore;
    unsigned int ComputerScore;

    bool PlayerTurn;
    bool ShowingComputerHand;
    bool SortingByRuns;

    static int LineLength;

    void DrawGame(const std::string& StatusMessage);
    std::string UserInput();
    void DealNewRound();

    void PrintLine(const std::string& Left, const std::string& Right) const;
    void PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const;

    int SumUnmatchedMeld(const Hand& hand) const;
    int FindUnmatchedMeld(Hand& hand, bool ResetMeld = true);
    int FindUnmatchedMeldWithPartner(Hand& hand, const Hand &partner);

    bool Knock(const Hand& hand) const;
    bool PickupDiscard(const Hand& hand);
    int IndexToDiscard(const Hand& hand) const;

    void CalculateProbabilityOfMeld(Hand& hand); // TODO
    double ProbabilityOfGin(const Hand& hand) const;

    FRIEND_TEST(SumUnmatchedMeldTest, EmptySum);
    FRIEND_TEST(SumUnmatchedMeldTest, AllNotMeldSum);

    FRIEND_TEST(FindUnmatchedMeldTest, SomeMeldFind);
    FRIEND_TEST(FindUnmatchedMeldTest, AllMeldFind);
    FRIEND_TEST(FindUnmatchedMeldTest, AllNotMeldFind);
    FRIEND_TEST(FindUnmatchedMeldTest, EmptyFind);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddRunsAndSetsToPartner);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddNothingToPartner);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddRunsToPartner);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddSetsToPartner);

    FRIEND_TEST(SearchForRunsTest, SearchForRuns);
    FRIEND_TEST(SearchForRunsTest, SearchForRunsNoResetMeld);

    FRIEND_TEST(SearchForSetsTest, SearchForSets);
    FRIEND_TEST(SearchForSetsTest, SearchForSetsNoResetMeld);

    FRIEND_TEST(IndexToDiscardTest, IndexToDiscardTest);

    FRIEND_TEST(CalculateProbabilityOfMeldTest, AllMeldCardTest);
    FRIEND_TEST(CalculateProbabilityOfMeldTest, NoMeldCardTest);

    FRIEND_TEST(CalculateProbabilityOfGinTest, AllMeldGinTest);
    FRIEND_TEST(CalculateProbabilityOfGinTest, NoMeldGinTest);
};

#endif // GINRUMMY_H
