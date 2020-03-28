#include <Platform.h>

#include <gtest/gtest.h>

namespace
{

TEST(Math, Abs)
{
	EXPECT_EQ(5, PFMath::Abs( 5));
	EXPECT_EQ(5, PFMath::Abs(-5));
	EXPECT_NEAR(5.4f, PFMath::Abs( 5.4f), 0.000001f);
	EXPECT_NEAR(5.4f, PFMath::Abs(-5.4f), 0.000001f);
	EXPECT_NEAR(5.4 , PFMath::Abs( 5.4 ), 0.000001 );
	EXPECT_NEAR(5.4 , PFMath::Abs(-5.4 ), 0.000001 );

	EXPECT_NEAR(5.4f, PFMath::Abs<float> ( 5.4f), 0.000001f);
	EXPECT_NEAR(5.4 , PFMath::Abs<double>( 5.4 ), 0.000001 );
}

TEST(Math, Max)
{
	EXPECT_EQ( 6, PFMath::Max( 5,  6));
	EXPECT_EQ(-4, PFMath::Max(-5, -4));
	EXPECT_NEAR( 7.6f, PFMath::Max( 5.4f,  7.6f), 0.000001f);
	EXPECT_NEAR(-4.5f, PFMath::Max(-5.4f, -4.5f), 0.000001f);
	EXPECT_NEAR( 7.6 , PFMath::Max( 5.4 ,  7.6 ), 0.000001 );
	EXPECT_NEAR(-4.5 , PFMath::Max(-5.4 , -4.5 ), 0.000001 );
}

TEST(Math, Min)
{
	EXPECT_EQ( 5, PFMath::Min( 5,  6));
	EXPECT_EQ(-5, PFMath::Min(-5, -4));
	EXPECT_NEAR( 5.4f, PFMath::Min( 5.4f,  7.6f), 0.000001f);
	EXPECT_NEAR(-5.4f, PFMath::Min(-5.4f, -4.5f), 0.000001f);
	EXPECT_NEAR( 5.4 , PFMath::Min( 5.4 ,  7.6 ), 0.000001 );
	EXPECT_NEAR(-5.4 , PFMath::Min(-5.4 , -4.5 ), 0.000001 );
}

TEST(Math, Clamp)
{
	EXPECT_EQ(8, PFMath::Clamp(10, 2, 8));
	EXPECT_EQ(5, PFMath::Clamp( 5, 2, 8));
	EXPECT_EQ(2, PFMath::Clamp( 0, 2, 8));
}

TEST(Math, FloorLog2)
{
	EXPECT_EQ(6, PFMath::FloorLog2U32(126));
	EXPECT_EQ(6, PFMath::FloorLog2U32(127));
	EXPECT_EQ(7, PFMath::FloorLog2U32(128));
	EXPECT_EQ(7, PFMath::FloorLog2U32(129));

	EXPECT_EQ(7, PFMath::FloorLog2U32(254));
	EXPECT_EQ(7, PFMath::FloorLog2U32(255));
	EXPECT_EQ(8, PFMath::FloorLog2U32(256));
	EXPECT_EQ(8, PFMath::FloorLog2U32(257));

	EXPECT_EQ(13, PFMath::FloorLog2U32(16382));
	EXPECT_EQ(13, PFMath::FloorLog2U32(16383));
	EXPECT_EQ(14, PFMath::FloorLog2U32(16384));
	EXPECT_EQ(14, PFMath::FloorLog2U32(16385));

	EXPECT_EQ(15, PFMath::FloorLog2U32(65534));
	EXPECT_EQ(15, PFMath::FloorLog2U32(65535));
	EXPECT_EQ(16, PFMath::FloorLog2U32(65536));
	EXPECT_EQ(16, PFMath::FloorLog2U32(65537));

	EXPECT_EQ(31, PFMath::FloorLog2U32(UINT_MAX - 1));
	EXPECT_EQ(31, PFMath::FloorLog2U32(UINT_MAX));
	EXPECT_EQ(32, PFMath::FloorLog2U64((UInt64)UINT_MAX + 1));
	EXPECT_EQ(32, PFMath::FloorLog2U64((UInt64)UINT_MAX + 2));
}

TEST(Math, CeilLog2)
{
	EXPECT_EQ(7, PFMath::CeilLog2U32(126));
	EXPECT_EQ(7, PFMath::CeilLog2U32(127));
	EXPECT_EQ(8, PFMath::CeilLog2U32(128));
	EXPECT_EQ(8, PFMath::CeilLog2U32(129));

	EXPECT_EQ(8, PFMath::CeilLog2U32(254));
	EXPECT_EQ(8, PFMath::CeilLog2U32(255));
	EXPECT_EQ(9, PFMath::CeilLog2U32(256));
	EXPECT_EQ(9, PFMath::CeilLog2U32(257));

	EXPECT_EQ(14, PFMath::CeilLog2U32(16382));
	EXPECT_EQ(14, PFMath::CeilLog2U32(16383));
	EXPECT_EQ(15, PFMath::CeilLog2U32(16384));
	EXPECT_EQ(15, PFMath::CeilLog2U32(16385));

	EXPECT_EQ(16, PFMath::CeilLog2U32(65534));
	EXPECT_EQ(16, PFMath::CeilLog2U32(65535));
	EXPECT_EQ(17, PFMath::CeilLog2U32(65536));
	EXPECT_EQ(17, PFMath::CeilLog2U32(65537));

	EXPECT_EQ(32, PFMath::CeilLog2U32(UINT_MAX - 1));
	EXPECT_EQ(32, PFMath::CeilLog2U32(UINT_MAX));
	EXPECT_EQ(33, PFMath::CeilLog2U64((UInt64)UINT_MAX + 1));
	EXPECT_EQ(33, PFMath::CeilLog2U64((UInt64)UINT_MAX + 2));
}

TEST(Math, RoundUpPowerOfTow)
{
	// U32
	EXPECT_EQ(PFMath::CeilLog2U32(130), 8);
	EXPECT_EQ(PFMath::RoundUpPowerOfTowU32(130), 256);

	EXPECT_EQ(PFMath::CeilLog2U32(1000), 10);
	EXPECT_EQ(PFMath::RoundUpPowerOfTowU32(1000), 1024);

	// U64
	EXPECT_EQ(PFMath::CeilLog2U64(130), 8);
	EXPECT_EQ(PFMath::RoundUpPowerOfTowU64(130), 256);

	EXPECT_EQ(PFMath::CeilLog2U64(1000), 10);
	EXPECT_EQ(PFMath::RoundUpPowerOfTowU64(1000), 1024);
}

}
