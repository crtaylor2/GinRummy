#ifndef GINRUMMY_H
#define GINRUMMY_H

#include "Card.h"

#include <vector>
#include <string>

#include <gtest/gtest.h>

class GinRummy
{
public:
    GinRummy();

    void Play();

private:
    std::vector<Card> Deck;

    std::vector<Card> PlayerCards;
    std::vector<Card> ComputerCards;

    std::vector<Card> Discard;

    unsigned int PlayerScore;
    unsigned int ComputerScore;

    bool PlayerTurn;
    bool ShowComputerHand;
    bool SortByRuns;

    static int LineLength;

    void DrawGame(const std::string& StatusMessage);
    std::string UserInput();
    void DealNewRound();

    void PrintLine(const std::string& Left, const std::string& Right) const;
    void PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const;

    int SumUnmatchedMeld(const std::vector<Card>& Hand) const;
    int FindUnmatchedMeld(std::vector<Card>& Hand, bool ResetMeld = true) const;
    int FindUnmatchedMeldWithPartner(std::vector<Card>& Hand, const std::vector<Card> &PartnerHand) const;

    void SearchForRuns(std::vector<Card>& Hand) const;
    void SearchForSets(std::vector<Card>& Hand) const;

    bool Knock(const std::vector<Card>& Hand) const;
    bool PickupDiscard(const std::vector<Card>& Hand) const;
    int IndexToDiscard(const std::vector<Card>& Hand) const;

    void CalculateProbabilityOfMeld(std::vector<Card>& Hand) const; // TODO
    double ProbabilityOfGin(const std::vector<Card>& Hand) const;

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
