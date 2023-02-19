#include "Card.h"

#include <gtest/gtest.h>

TEST(CardToStringTest, CardToStringTest)
{
    Card c1(Card::DIAMOND, Card::EIGHT);
    EXPECT_EQ(c1.CardToString(), "Eight of Diamonds");
    Card c2(Card::SPADE, Card::ACE);
    EXPECT_EQ(c2.CardToString(), "Ace of Spades");
    Card c3(Card::CLUB, Card::KING);
    EXPECT_EQ(c3.CardToString(), "King of Clubs");
    Card c4(Card::HEART, Card::SIX);
    EXPECT_EQ(c4.CardToString(), "Six of Hearts");
}

TEST(CardPointsTest, CardPointsTest)
{
    Card c1(Card::DIAMOND, Card::ACE);
    EXPECT_EQ(c1.CardPoints(), 1);
    Card c2(Card::SPADE, Card::TWO);
    EXPECT_EQ(c2.CardPoints(), 2);
    Card c3(Card::CLUB, Card::THREE);
    EXPECT_EQ(c3.CardPoints(), 3);
    Card c4(Card::HEART, Card::FOUR);
    EXPECT_EQ(c4.CardPoints(), 4);
    Card c5(Card::DIAMOND, Card::FIVE);
    EXPECT_EQ(c5.CardPoints(), 5);
    Card c6(Card::SPADE, Card::SIX);
    EXPECT_EQ(c6.CardPoints(), 6);
    Card c7(Card::CLUB, Card::SEVEN);
    EXPECT_EQ(c7.CardPoints(), 7);
    Card c8(Card::HEART, Card::EIGHT);
    EXPECT_EQ(c8.CardPoints(), 8);
    Card c9(Card::DIAMOND, Card::NINE);
    EXPECT_EQ(c9.CardPoints(), 9);
    Card c10(Card::SPADE, Card::TEN);
    EXPECT_EQ(c10.CardPoints(), 10);
    Card c11(Card::CLUB, Card::JACK);
    EXPECT_EQ(c11.CardPoints(), 10);
    Card c12(Card::HEART, Card::QUEEN);
    EXPECT_EQ(c12.CardPoints(), 10);
    Card c13(Card::DIAMOND, Card::KING);
    EXPECT_EQ(c13.CardPoints(), 10);
}

TEST(CompareForSetsTest, CompareForSetsTest)
{
    std::vector<Card> Hand;
    Card c1(Card::HEART, Card::ACE);
    Card c2(Card::HEART, Card::FIVE);
    Card c3(Card::SPADE, Card::ACE);
    Hand.push_back(c1);
    Hand.push_back(c2);
    Hand.push_back(c3);
    std::sort(Hand.begin(), Hand.end(), Card::CompareForSets);
    EXPECT_EQ(Hand.at(0), c1);
    EXPECT_EQ(Hand.at(1), c3);
    EXPECT_EQ(Hand.at(2), c2);
}

TEST(CompareForRunsTest, CompareForRunsTest)
{
    std::vector<Card> Hand;
    Card c1(Card::HEART, Card::ACE);
    Card c2(Card::HEART, Card::FIVE);
    Card c3(Card::SPADE, Card::TWO);
    Hand.push_back(c1);
    Hand.push_back(c2);
    Hand.push_back(c3);
    std::sort(Hand.begin(), Hand.end(), Card::CompareForRuns);
    EXPECT_EQ(Hand.at(0), c1);
    EXPECT_EQ(Hand.at(1), c2);
    EXPECT_EQ(Hand.at(2), c3);
}
