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

    std::vector<Card> Discard;

    static int LineLength;

    void DrawGame();
    void UserInput(std::string Input);

    void PrintLine(const std::string& Left, const std::string& Right) const;
    void PrintLine(const std::string& Left, const std::string& Middle, const std::string& Right) const;

    int CalculateUnmatchedMeld(std::vector<Card>& Hand) const;
    int CountUnmatchedMeld(const std::vector<Card>& Hand) const;
    void SearchForRuns(std::vector<Card>& Hand) const;
    void SearchForPairs(std::vector<Card>& Hand) const;
    void RemoveMeld(std::vector<Card>& Hand, std::vector<Card>& Meld) const;
    void AddMeld(std::vector<Card>& Hand, std::vector<Card>& Meld) const;
};

#endif // GINRUMMY_H
