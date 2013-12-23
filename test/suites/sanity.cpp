
#include <UnitTest++/UnitTest++.h>
#include <typeinfo>

#include <bitvector.h>


typedef bitlib::BitVector<8> BV8;
typedef bitlib::BitVector<16> BV16;
typedef bitlib::BitVector<32> BV32;
typedef bitlib::BitVector<64> BV64;

typedef bitlib::BitVector<32, unsigned short> BV32_ushort;
typedef bitlib::BitVector<32, unsigned int> BV32_uint;


SUITE(sanity)
{

	TEST(BitBlockSizes)
	{
		CHECK_EQUAL(8, BV8::BlockSize);
		CHECK_EQUAL(16, BV16::BlockSize);
		CHECK_EQUAL(32, BV32::BlockSize);
		CHECK_EQUAL(64, BV64::BlockSize);
	}


	TEST(IndexType)
	{
		CHECK(typeid(BV32_ushort::IndexType) == typeid(unsigned short));
		CHECK(typeid(BV32_uint::IndexType) == typeid(unsigned int));
	}

}

