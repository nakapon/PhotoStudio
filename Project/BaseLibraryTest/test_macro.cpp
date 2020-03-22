#include <Platform.h>

#include <gtest/gtest.h>

namespace
{

TEST(Macro, ArrayLength)
{
	int test1[1];
	int test2[32];

	EXPECT_EQ(1, PF_ARRAY_LENGTH(test1));

	EXPECT_EQ(32, PF_ARRAY_LENGTH(test2));
}

}
