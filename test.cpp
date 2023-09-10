
#include "gtest/gtest.h"
#include "main.cc"

TEST(main, func)
{
    GTEST_ASSERT_EQ(func("test/orig.txt","test/orig_0.8_add.txt"), 0.65);
}

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
