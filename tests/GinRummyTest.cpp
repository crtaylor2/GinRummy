#include "GinRummy.h"

#include <gtest/gtest.h>

TEST(SumUnmatchedMeldTest, Empty)
{
    GinRummy gr;
    std::vector<Card> Hand;
    EXPECT_EQ(gr.SumUnmatchedMeld(Hand), 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
