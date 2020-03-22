#include <Platform.h>

#include <gtest/gtest.h>

namespace
{

TEST(Math, Abs)
{
	EXPECT_EQ(PFMath::Abs( 5), 5);
	EXPECT_EQ(PFMath::Abs(-5), 5);
	EXPECT_NEAR(PFMath::Abs( 5.4f),  5.4f, 0.000001f);
	EXPECT_NEAR(PFMath::Abs(-5.4f),  5.4f, 0.000001f);
	EXPECT_NEAR(PFMath::Abs( 5.4 ),  5.4 , 0.000001 );
	EXPECT_NEAR(PFMath::Abs(-5.4 ),  5.4 , 0.000001 );

	EXPECT_NEAR(PFMath::Abs<float> ( 5.4f),  5.4f, 0.000001f);
	EXPECT_NEAR(PFMath::Abs<double>( 5.4 ),  5.4 , 0.000001 );
}

TEST(Math, Max)
{
	EXPECT_EQ(PFMath::Max( 5,  6),  6);
	EXPECT_EQ(PFMath::Max(-5, -4), -4);
	EXPECT_NEAR(PFMath::Max( 5.4f,  7.6f),  7.6f, 0.000001f);
	EXPECT_NEAR(PFMath::Max(-5.4f, -4.5f), -4.5f, 0.000001f);
	EXPECT_NEAR(PFMath::Max( 5.4 ,  7.6 ),  7.6 , 0.000001 );
	EXPECT_NEAR(PFMath::Max(-5.4 , -4.5 ), -4.5 , 0.000001 );
}

TEST(Math, Min)
{
	EXPECT_EQ(PFMath::Min( 5,  6),  5);
	EXPECT_EQ(PFMath::Min(-5, -4), -5);
	EXPECT_NEAR(PFMath::Min( 5.4f,  7.6f),  5.4f, 0.000001f);
	EXPECT_NEAR(PFMath::Min(-5.4f, -4.5f), -5.4f, 0.000001f);
	EXPECT_NEAR(PFMath::Min( 5.4 ,  7.6 ),  5.4 , 0.000001 );
	EXPECT_NEAR(PFMath::Min(-5.4 , -4.5 ), -5.4 , 0.000001 );
}

TEST(Math, Clamp)
{
	EXPECT_EQ(PFMath::Clamp(10, 2, 8), 8);
	EXPECT_EQ(PFMath::Clamp( 5, 2, 8), 5);
	EXPECT_EQ(PFMath::Clamp( 0, 2, 8), 2);
}

}
