
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
		CHECK_EQUAL(BV8::BlockSize, 8);
		CHECK_EQUAL(BV16::BlockSize, 16);
		CHECK_EQUAL(BV32::BlockSize, 32);
		CHECK_EQUAL(BV64::BlockSize, 64);
	}


	TEST(IndexType)
	{
		CHECK(typeid(BV32_ushort::IndexType) == typeid(unsigned short));
		CHECK(typeid(BV32_uint::IndexType) == typeid(unsigned int));
	}

}

