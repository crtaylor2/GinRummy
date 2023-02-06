#ifndef GINRUMMY_H
#define GINRUMMY_H

#include "Card.h"

#include <vector>
#include <string>

class GinRummy
{
public:
    GinRummy();

    void Play();

    int SumUnmatchedMeld(const std::vector<Card>& Hand) const;

private:
    std::vector<Card> Deck;

    std::vector<Card> PlayerCards;
    std::vector<Card> ComputerCards;

    unsigned int PlayerScore;
    unsigned int ComputerScore;

    bool PlayerTurn;
    bool ShowComputerHand;
    bool SortByRuns;

    std::vector<Card> Discard;

    static int LineLength;

    void DrawGame(const std::string& StatusMessage);
    std::string UserInput();
    void DealNewRound();

    void PrintLine(const std::string& Left, const std::string& Right) const;
    void PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const;

    int FindUnmatchedMeld(std::vector<Card>& Hand, bool ResetMeld = true) const;
    int FindUnmatchedMeldWithPartner(const std::vector<Card>& Hand, const std::vector<Card> &PartnerHand) const;

    void SearchForRuns(std::vector<Card>& Hand) const;
    void SearchForSets(std::vector<Card>& Hand) const;

    bool PickupDiscard(std::vector<Card>& Hand) const;
    int IndexToDiscard(std::vector<Card>& Hand) const; // TODO
    bool Knock(std::vector<Card>& Hand) const;

    double ProbabilityOfGin(std::vector<Card>& Hand) const;
    void CalculateProbabilityOfMeld(std::vector<Card>& Hand) const; // TODO
};

#endif // GINRUMMY_H
