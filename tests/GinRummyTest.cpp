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

TEST(SearchForRunsTest, SearchForRuns)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::SPADE, Card::ACE)); // Run 1
    Hand.push_back(Card(Card::SPADE, Card::TWO)); // Run 1
    Hand.push_back(Card(Card::SPADE, Card::THREE)); // Run 1
    Hand.push_back(Card(Card::CLUB, Card::FOUR));
    Hand.push_back(Card(Card::DIAMOND, Card::FIVE)); // Run 2
    Hand.push_back(Card(Card::DIAMOND, Card::SIX)); // Run 2
    Hand.push_back(Card(Card::DIAMOND, Card::SEVEN)); // Run 2
    Hand.push_back(Card(Card::CLUB, Card::EIGHT));
    Hand.push_back(Card(Card::CLUB, Card::NINE));
    Hand.push_back(Card(Card::SPADE, Card::TEN)); // Run 3
    Hand.push_back(Card(Card::SPADE, Card::JACK)); // Run 3
    Hand.push_back(Card(Card::SPADE, Card::QUEEN)); // Run 3
    Hand.push_back(Card(Card::SPADE, Card::KING)); // Run 3
    gr.SearchForRuns(Hand);
    EXPECT_EQ(Hand.at(0).meld, Card::RUNMELD); // 5 of Diamonds
    EXPECT_EQ(Hand.at(1).meld, Card::RUNMELD); // 6 of Diamonds
    EXPECT_EQ(Hand.at(2).meld, Card::RUNMELD); // 7 of Diamonds
    EXPECT_EQ(Hand.at(3).meld, Card::NOTMELD); // 4 of Clubs
    EXPECT_EQ(Hand.at(4).meld, Card::NOTMELD); // 8 of Clubs
    EXPECT_EQ(Hand.at(5).meld, Card::NOTMELD); // 9 of Clubs
    EXPECT_EQ(Hand.at(6).meld, Card::RUNMELD); // Ace of Spades
    EXPECT_EQ(Hand.at(7).meld, Card::RUNMELD); // 2 of Spades
    EXPECT_EQ(Hand.at(8).meld, Card::RUNMELD); // 3 of Spades
    EXPECT_EQ(Hand.at(9).meld, Card::RUNMELD); // 10 of Spades
    EXPECT_EQ(Hand.at(10).meld, Card::RUNMELD); // Jack of Spades
    EXPECT_EQ(Hand.at(11).meld, Card::RUNMELD); // Queen of Spades
    EXPECT_EQ(Hand.at(12).meld, Card::RUNMELD); // King of Spades
}

TEST(SearchForRunsTest, SearchForRunsNoResetMeld)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::SPADE, Card::ACE));
    Hand.push_back(Card(Card::SPADE, Card::TWO));
    Hand.back().meld = Card::SETMELD;
    Hand.push_back(Card(Card::SPADE, Card::THREE));
    Hand.push_back(Card(Card::CLUB, Card::FOUR));
    Hand.push_back(Card(Card::DIAMOND, Card::FIVE));
    Hand.push_back(Card(Card::DIAMOND, Card::SIX));
    Hand.back().meld = Card::SETMELD;
    Hand.push_back(Card(Card::DIAMOND, Card::SEVEN));
    Hand.push_back(Card(Card::CLUB, Card::EIGHT));
    Hand.push_back(Card(Card::CLUB, Card::NINE));
    Hand.push_back(Card(Card::SPADE, Card::TEN)); // Run
    Hand.push_back(Card(Card::SPADE, Card::JACK)); // Run
    Hand.push_back(Card(Card::SPADE, Card::QUEEN)); // Run
    Hand.push_back(Card(Card::SPADE, Card::KING)); // Run
    gr.SearchForRuns(Hand);
    EXPECT_EQ(Hand.at(0).meld, Card::NOTMELD); // 5 of Diamonds
    EXPECT_EQ(Hand.at(1).meld, Card::SETMELD); // 6 of Diamonds
    EXPECT_EQ(Hand.at(2).meld, Card::NOTMELD); // 7 of Diamonds
    EXPECT_EQ(Hand.at(3).meld, Card::NOTMELD); // 4 of Clubs
    EXPECT_EQ(Hand.at(4).meld, Card::NOTMELD); // 8 of Clubs
    EXPECT_EQ(Hand.at(5).meld, Card::NOTMELD); // 9 of Clubs
    EXPECT_EQ(Hand.at(6).meld, Card::NOTMELD); // Ace of Spades
    EXPECT_EQ(Hand.at(7).meld, Card::SETMELD); // 2 of Spades
    EXPECT_EQ(Hand.at(8).meld, Card::NOTMELD); // 3 of Spades
    EXPECT_EQ(Hand.at(9).meld, Card::RUNMELD); // 10 of Spades
    EXPECT_EQ(Hand.at(10).meld, Card::RUNMELD); // Jack of Spades
    EXPECT_EQ(Hand.at(11).meld, Card::RUNMELD); // Queen of Spades
    EXPECT_EQ(Hand.at(12).meld, Card::RUNMELD); // King of Spades
}

TEST(SearchForSetsTest, SearchForSets)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::SPADE, Card::ACE));
    Hand.push_back(Card(Card::HEART, Card::ACE));
    Hand.push_back(Card(Card::DIAMOND, Card::SEVEN));
    Hand.push_back(Card(Card::CLUB, Card::KING));
    Hand.push_back(Card(Card::SPADE, Card::KING));
    Hand.push_back(Card(Card::DIAMOND, Card::KING));
    gr.SearchForSets(Hand);
    EXPECT_EQ(Hand.at(0).meld, Card::SETMELD); // Ace of Clubs
    EXPECT_EQ(Hand.at(1).meld, Card::SETMELD); // Ace of Spades
    EXPECT_EQ(Hand.at(2).meld, Card::SETMELD); // Ace of Hearts
    EXPECT_EQ(Hand.at(3).meld, Card::NOTMELD); // 7 of Diamonds
    EXPECT_EQ(Hand.at(4).meld, Card::SETMELD); // King of Clubs
    EXPECT_EQ(Hand.at(5).meld, Card::SETMELD); // King of Spades
    EXPECT_EQ(Hand.at(6).meld, Card::SETMELD); // King of Diamonds
}

TEST(SearchForSetsTest, SearchForSetsNoResetMeld)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::HEART, Card::ACE));
    Hand.push_back(Card(Card::SPADE, Card::ACE));
    Hand.back().meld = Card::RUNMELD;
    Hand.push_back(Card(Card::DIAMOND, Card::SEVEN));
    Hand.push_back(Card(Card::CLUB, Card::KING));
    Hand.push_back(Card(Card::SPADE, Card::KING));
    Hand.push_back(Card(Card::DIAMOND, Card::KING));
    gr.SearchForSets(Hand);
    EXPECT_EQ(Hand.at(0).meld, Card::NOTMELD); // Ace of Clubs
    EXPECT_EQ(Hand.at(1).meld, Card::NOTMELD); // Ace of Hearts
    EXPECT_EQ(Hand.at(2).meld, Card::RUNMELD); // Ace of Spades
    EXPECT_EQ(Hand.at(3).meld, Card::NOTMELD); // 7 of Diamonds
    EXPECT_EQ(Hand.at(4).meld, Card::SETMELD); // King of Clubs
    EXPECT_EQ(Hand.at(5).meld, Card::SETMELD); // King of Spades
    EXPECT_EQ(Hand.at(6).meld, Card::SETMELD); // King of Diamonds
}

TEST(CalculateProbabilityOfGinTest, AllMeldGinTest)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::HEART, Card::ACE));
    Hand.push_back(Card(Card::SPADE, Card::ACE));
    gr.FindUnmatchedMeld(Hand);
    EXPECT_EQ(gr.ProbabilityOfGin(Hand), 1.0);
}

TEST(CalculateProbabilityOfGinTest, NoMeldGinTest)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::CLUB, Card::FOUR));
    gr.Discard.push_back(Card(Card::HEART, Card::ACE));
    gr.Discard.push_back(Card(Card::SPADE, Card::ACE));
    gr.Discard.push_back(Card(Card::CLUB, Card::TWO));
    gr.FindUnmatchedMeld(Hand);
    EXPECT_EQ(gr.ProbabilityOfGin(Hand), 0.0);
}

TEST(CalculateProbabilityOfMeldTest, AllMeldCardTest)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::HEART, Card::ACE));
    Hand.push_back(Card(Card::SPADE, Card::ACE));
    gr.FindUnmatchedMeld(Hand);
    gr.CalculateProbabilityOfMeld(Hand);
    EXPECT_EQ(Hand.at(0).probOfMeld, 1.0);
    EXPECT_EQ(Hand.at(1).probOfMeld, 1.0);
    EXPECT_EQ(Hand.at(2).probOfMeld, 1.0);
}

TEST(CalculateProbabilityOfMeldTest, NoMeldCardTest)
{
    GinRummy gr;
    std::vector<Card> Hand;
    Hand.push_back(Card(Card::CLUB, Card::ACE));
    Hand.push_back(Card(Card::CLUB, Card::FOUR));
    gr.Discard.push_back(Card(Card::HEART, Card::ACE));
    gr.Discard.push_back(Card(Card::SPADE, Card::ACE));
    gr.Discard.push_back(Card(Card::CLUB, Card::TWO));
    gr.FindUnmatchedMeld(Hand);
    gr.CalculateProbabilityOfMeld(Hand);
    EXPECT_EQ(Hand.at(0).probOfMeld, 0.0);
    EXPECT_GT(Hand.at(1).probOfMeld, 0.0);
    EXPECT_LT(Hand.at(1).probOfMeld, 1.0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
