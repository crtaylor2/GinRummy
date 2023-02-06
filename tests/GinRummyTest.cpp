#include "GinRummy.h"

#include <gtest/gtest.h>

TEST(SumUnmatchedMeldTest, EmptySum)
{
    GinRummy gr;
    std::vector<Card> Hand;
    EXPECT_EQ(gr.SumUnmatchedMeld(Hand), 0);
}

TEST(SumUnmatchedMeldTest, AllNotMeldSum)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::TEN));
    Hand.push_back(Card(Card::SPADE, Card::JACK));
    Hand.push_back(Card(Card::HEART, Card::KING));
    Hand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.SumUnmatchedMeld(Hand), 40);
}

TEST(FindUnmatchedMeldTest, EmptyFind)
{
    GinRummy gr;
    std::vector<Card> Hand;
    EXPECT_EQ(gr.FindUnmatchedMeld(Hand), 0);
}

TEST(FindUnmatchedMeldTest, AllNotMeldFind)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::TEN));
    Hand.push_back(Card(Card::SPADE, Card::JACK));
    Hand.push_back(Card(Card::HEART, Card::KING));
    Hand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(Hand), 40);
}

TEST(FindUnmatchedMeldTest, AllMeldFind)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::SPADE, Card::KING));
    Hand.push_back(Card(Card::HEART, Card::KING));
    Hand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(Hand), 0);
}

TEST(FindUnmatchedMeldTest, SomeMeldFind)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::DIAMOND, Card::ACE));
    Hand.push_back(Card(Card::CLUB, Card::TWO));
    Hand.push_back(Card(Card::CLUB, Card::THREE));
    Hand.push_back(Card(Card::CLUB, Card::FOUR));
    Hand.push_back(Card(Card::SPADE, Card::FIVE));
    EXPECT_EQ(gr.FindUnmatchedMeld(Hand), 6);
}

TEST(FindUnmatchedMeldTestWithParnter, AddSetsToPartner)
{
    GinRummy gr;
    std::vector<Card> Hand;
    std::vector<Card> PartnerHand;
    Hand.push_back(Card(Card::DIAMOND, Card::KING));
    Hand.push_back(Card(Card::DIAMOND, Card::FOUR));
    Hand.push_back(Card(Card::DIAMOND, Card::TWO));
    PartnerHand.push_back(Card(Card::SPADE, Card::KING));
    PartnerHand.push_back(Card(Card::HEART, Card::KING));
    PartnerHand.push_back(Card(Card::CLUB, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(PartnerHand), 0);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(Hand, PartnerHand), 6);
}

TEST(FindUnmatchedMeldTestWithParnter, AddRunsToPartner)
{
    GinRummy gr;
    std::vector<Card> Hand;
    std::vector<Card> PartnerHand;
    Hand.push_back(Card(Card::SPADE, Card::KING));
    Hand.push_back(Card(Card::DIAMOND, Card::TEN));
    Hand.push_back(Card(Card::SPADE, Card::TWO));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::JACK));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::QUEEN));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(PartnerHand), 0);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(Hand, PartnerHand), 12);
}

TEST(FindUnmatchedMeldTestWithParnter, AddNothingToPartner)
{
    GinRummy gr;
    std::vector<Card> Hand;
    std::vector<Card> PartnerHand;
    Hand.push_back(Card(Card::SPADE, Card::KING));
    Hand.push_back(Card(Card::CLUB, Card::TEN));
    Hand.push_back(Card(Card::SPADE, Card::TWO));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::JACK));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::QUEEN));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::KING));
    EXPECT_EQ(gr.FindUnmatchedMeld(PartnerHand), 0);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(Hand, PartnerHand), 22);
}

TEST(FindUnmatchedMeldTestWithParnter, AddRunsAndSetsToPartner)
{
    GinRummy gr;
    std::vector<Card> Hand;
    std::vector<Card> PartnerHand;
    Hand.push_back(Card(Card::DIAMOND, Card::TEN));
    Hand.push_back(Card(Card::SPADE, Card::NINE));
    Hand.push_back(Card(Card::SPADE, Card::ACE));
    Hand.push_back(Card(Card::DIAMOND, Card::ACE));
    Hand.push_back(Card(Card::HEART, Card::ACE));
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::CLUB, Card::FOUR));

    PartnerHand.push_back(Card(Card::HEART, Card::TWO));
    PartnerHand.push_back(Card(Card::HEART, Card::NINE));
    PartnerHand.push_back(Card(Card::SPADE, Card::NINE));
    PartnerHand.push_back(Card(Card::CLUB, Card::NINE));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::JACK));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::QUEEN));
    PartnerHand.push_back(Card(Card::DIAMOND, Card::KING));

    EXPECT_EQ(gr.FindUnmatchedMeld(PartnerHand), 2);
    EXPECT_EQ(gr.FindUnmatchedMeldWithPartner(Hand, PartnerHand), 4);
}


int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
