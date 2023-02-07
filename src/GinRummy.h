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
    FRIEND_TEST(SumUnmatchedMeldTest, EmptySum);
    FRIEND_TEST(SumUnmatchedMeldTest, AllNotMeldSum);

    int FindUnmatchedMeld(std::vector<Card>& Hand, bool ResetMeld = true) const;
    FRIEND_TEST(FindUnmatchedMeldTest, SomeMeldFind);
    FRIEND_TEST(FindUnmatchedMeldTest, AllMeldFind);
    FRIEND_TEST(FindUnmatchedMeldTest, AllNotMeldFind);
    FRIEND_TEST(FindUnmatchedMeldTest, EmptyFind);

    int FindUnmatchedMeldWithPartner(const std::vector<Card>& Hand, const std::vector<Card> &PartnerHand) const;
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddRunsAndSetsToPartner);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddNothingToPartner);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddRunsToPartner);
    FRIEND_TEST(FindUnmatchedMeldTestWithParnter, AddSetsToPartner);

    void SearchForRuns(std::vector<Card>& Hand) const;
    FRIEND_TEST(SearchForRunsTest, SearchForRuns);
    FRIEND_TEST(SearchForRunsTest, SearchForRunsNoResetMeld);

    void SearchForSets(std::vector<Card>& Hand) const;
    FRIEND_TEST(SearchForSetsTest, SearchForSets);
    FRIEND_TEST(SearchForSetsTest, SearchForSetsNoResetMeld);

    bool PickupDiscard(std::vector<Card>& Hand) const;
    int IndexToDiscard(std::vector<Card>& Hand) const; // TODO
    bool Knock(std::vector<Card>& Hand) const;

    void CalculateProbabilityOfMeld(std::vector<Card>& Hand) const; // TODO
    FRIEND_TEST(CalculateProbabilityOfMeldTest, AllMeldCardTest);
    FRIEND_TEST(CalculateProbabilityOfMeldTest, NoMeldCardTest);

    double ProbabilityOfGin(std::vector<Card>& Hand) const;
    FRIEND_TEST(CalculateProbabilityOfGinTest, AllMeldGinTest);
    FRIEND_TEST(CalculateProbabilityOfGinTest, NoMeldGinTest);
};

#endif // GINRUMMY_H
