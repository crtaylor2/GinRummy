#include "GinRummy.h"

#include <gtest/gtest.h>

TEST(SumUnmatchedMeldTest, EmptySum)
{
    GinRummy gr;
    Hand hand;
    EXPECT_EQ(gr.SumUnmatchedMeld(hand), 0);
}

TEST(SumUnmatchedMeldTest, AllNotMeldSum)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::CLUB, Card::TEN));
    hand.push_back(Card(Card::SPADE, Card::JACK));
    hand.push_back(Card(Card::HEART, Card::KING));
    hand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.SumUnmatchedMeld(hand), 40);
}

TEST(FindUnmatchedMeldTest, EmptyFind)
{
    GinRummy gr;
    Hand hand;
    EXPECT_EQ(gr.FindUnmatchedMeld(hand), 0);
}

TEST(FindUnmatchedMeldTest, AllNotMeldFind)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::CLUB, Card::TEN));
    hand.push_back(Card(Card::SPADE, Card::JACK));
    hand.push_back(Card(Card::HEART, Card::KING));
    hand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(hand), 40);
}

TEST(FindUnmatchedMeldTest, AllMeldFind)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::SPADE, Card::KING));
    hand.push_back(Card(Card::HEART, Card::KING));
    hand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(hand), 0);
}

TEST(FindUnmatchedMeldTest, SomeMeldFind)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::DIAMOND, Card::ACE));
    hand.push_back(Card(Card::CLUB, Card::TWO));
    hand.push_back(Card(Card::CLUB, Card::THREE));
    hand.push_back(Card(Card::CLUB, Card::FOUR));
    hand.push_back(Card(Card::SPADE, Card::FIVE));
    EXPECT_EQ(gr.FindUnmatchedMeld(hand), 6);
}

TEST(FindUnmatchedMeldTestWithParnter, AddSetsToPartner)
{
    GinRummy gr;
    Hand hand;
    Hand partner;
    hand.push_back(Card(Card::DIAMOND, Card::KING));
    hand.push_back(Card(Card::DIAMOND, Card::FOUR));
    hand.push_back(Card(Card::DIAMOND, Card::TWO));
    partner.push_back(Card(Card::SPADE, Card::KING));
    partner.push_back(Card(Card::HEART, Card::KING));
    partner.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(partner), 0);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(hand, partner), 6);
    EXPECT_EQ(hand.size(), 3);
}

TEST(FindUnmatchedMeldTestWithParnter, AddRunsToPartner)
{
    GinRummy gr;
    Hand hand;
    Hand partner;
    hand.push_back(Card(Card::SPADE, Card::KING));
    hand.push_back(Card(Card::DIAMOND, Card::TEN));
    hand.push_back(Card(Card::SPADE, Card::TWO));
    partner.push_back(Card(Card::DIAMOND, Card::JACK));
    partner.push_back(Card(Card::DIAMOND, Card::QUEEN));
    partner.push_back(Card(Card::DIAMOND, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(partner), 0);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(hand, partner), 12);
    EXPECT_EQ(hand.size(), 3);
}

TEST(FindUnmatchedMeldTestWithParnter, AddNothingToPartner)
{
    GinRummy gr;
    Hand hand;
    Hand partner;
    hand.push_back(Card(Card::SPADE, Card::KING));
    hand.push_back(Card(Card::CLUB, Card::TEN));
    hand.push_back(Card(Card::SPADE, Card::TWO));
    partner.push_back(Card(Card::DIAMOND, Card::JACK));
    partner.push_back(Card(Card::DIAMOND, Card::QUEEN));
    partner.push_back(Card(Card::DIAMOND, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(partner), 0);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(hand, partner), 22);
    EXPECT_EQ(hand.size(), 3);
}

TEST(FindUnmatchedMeldTestWithParnter, AddRunsAndSetsToPartner)
{
    GinRummy gr;
    Hand hand;
    Hand partner;
    hand.push_back(Card(Card::DIAMOND, Card::TEN));
    hand.push_back(Card(Card::SPADE, Card::NINE));
    hand.push_back(Card(Card::SPADE, Card::ACE));
    hand.push_back(Card(Card::DIAMOND, Card::ACE));
    hand.push_back(Card(Card::HEART, Card::ACE));
    hand.push_back(Card(Card::CLUB, Card::ACE));
    hand.push_back(Card(Card::CLUB, Card::FOUR));

    partner.push_back(Card(Card::HEART, Card::TWO));
    partner.push_back(Card(Card::HEART, Card::NINE));
    partner.push_back(Card(Card::SPADE, Card::NINE));
    partner.push_back(Card(Card::CLUB, Card::NINE));
    partner.push_back(Card(Card::DIAMOND, Card::JACK));
    partner.push_back(Card(Card::DIAMOND, Card::QUEEN));
    partner.push_back(Card(Card::DIAMOND, Card::KING));

    EXPECT_EQ(gr.FindUnmatchedMeld(partner), 2);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(hand, partner), 4);
    EXPECT_EQ(hand.size(), 7);
}

TEST(CalculateProbabilityOfGinTest, AllMeldGinTest)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::CLUB, Card::ACE));
    hand.push_back(Card(Card::HEART, Card::ACE));
    hand.push_back(Card(Card::SPADE, Card::ACE));
    gr.FindUnmatchedMeld(hand);
    EXPECT_EQ(gr.ProbabilityOfGin(hand), 1.0);
}

TEST(CalculateProbabilityOfGinTest, NoMeldGinTest)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::CLUB, Card::ACE));
    hand.push_back(Card(Card::CLUB, Card::FOUR));
    gr.DiscardDeck.push_back(Card(Card::HEART, Card::ACE));
    gr.DiscardDeck.push_back(Card(Card::SPADE, Card::ACE));
    gr.DiscardDeck.push_back(Card(Card::CLUB, Card::TWO));
    gr.FindUnmatchedMeld(hand);
    EXPECT_EQ(gr.ProbabilityOfGin(hand), 0.0);
}

TEST(CalculateProbabilityOfMeldTest, AllMeldCardTest)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::CLUB, Card::ACE));
    hand.push_back(Card(Card::HEART, Card::ACE));
    hand.push_back(Card(Card::SPADE, Card::ACE));
    gr.FindUnmatchedMeld(hand);
    gr.CalculateProbabilityOfMeld(hand);
    EXPECT_EQ(hand.at(0).probOfMeld, 1.0);
    EXPECT_EQ(hand.at(1).probOfMeld, 1.0);
    EXPECT_EQ(hand.at(2).probOfMeld, 1.0);
}

TEST(CalculateProbabilityOfMeldTest, NoMeldCardTest)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::CLUB, Card::ACE));
    hand.push_back(Card(Card::CLUB, Card::FOUR));
    gr.DiscardDeck.push_back(Card(Card::HEART, Card::ACE));
    gr.DiscardDeck.push_back(Card(Card::SPADE, Card::ACE));
    gr.DiscardDeck.push_back(Card(Card::CLUB, Card::TWO));
    gr.FindUnmatchedMeld(hand);
    gr.CalculateProbabilityOfMeld(hand);
    EXPECT_EQ(hand.at(0).probOfMeld, 0.0);
    EXPECT_GT(hand.at(1).probOfMeld, 0.0);
    EXPECT_LT(hand.at(1).probOfMeld, 1.0);
}

TEST(IndexToDiscardTest, IndexToDiscardTest)
{
    GinRummy gr;
    Hand hand;
    hand.push_back(Card(Card::HEART, Card::KING));
    hand.push_back(Card(Card::CLUB, Card::KING));
    hand.push_back(Card(Card::SPADE, Card::KING));
    hand.push_back(Card(Card::DIAMOND, Card::KING));
    hand.push_back(Card(Card::DIAMOND, Card::QUEEN));
    hand.push_back(Card(Card::SPADE, Card::EIGHT));
    hand.push_back(Card(Card::DIAMOND, Card::TEN));
    hand.push_back(Card(Card::CLUB, Card::TWO));
    hand.push_back(Card(Card::DIAMOND, Card::FIVE));
    hand.push_back(Card(Card::SPADE, Card::TWO));
    gr.FindUnmatchedMeld(hand);
    int idx = gr.IndexToDiscard(hand);
    EXPECT_EQ(hand.at(idx).value, Card::QUEEN);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
