#ifndef GINRUMMY_H
#define GINRUMMY_H

#include "Card.h"

#include <vector>
#include <string>

class GinRummy
{
public:
    GinRummy();

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

    void DrawGame();
    void Play();
    void UserInput();

    void PrintLine(const std::string& Left, const std::string& Right) const;
    void PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const;

    int FindUnmatchedMeld(std::vector<Card>& Hand) const;
    int SumUnmatchedMeld(const std::vector<Card>& Hand) const;

    void SearchForRuns(std::vector<Card>& Hand) const;
    void SearchForSets(std::vector<Card>& Hand) const;

    void AddMeld(std::vector<Card>& Hand, std::vector<Card>& Meld) const;
    void RemoveMeld(std::vector<Card>& Hand, std::vector<Card>& Meld) const;

    bool PickupDiscard(const std::vector<Card>& Hand) const; // TODO
    int IndexToDiscard(const std::vector<Card>& Hand) const; // TODO
    bool Knock(const std::vector<Card>& Hand) const; // TODO

    double ProbabilityOfGin(const std::vector<Card>& Hand) const; // TODO
    void CalculateProbabilityOfMeld(std::vector<Card>& Hand) const; // TODO
};

#endif // GINRUMMY_H
