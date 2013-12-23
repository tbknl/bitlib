
SUITE_X(operations, BLOCKSIZE)
{
	typedef bitlib::BitVector<BLOCKSIZE> BV;


	TEST(Invert)
	{
		{
			BV bv(1);
			CHECK_EQUAL(bv.get(0), 0);
			bv.bitInvert();
			CHECK_EQUAL(bv.get(0), 1);
			bv.bitInvert();
			CHECK_EQUAL(bv.get(0), 0);
		}
		
		{
			BV bv(3 * BLOCKSIZE + 1); 
			bv.set(3 * BLOCKSIZE, 1);
			bv.bitInvert();
			CHECK_EQUAL(bv.get(3 * BLOCKSIZE), 0);
			bv.bitInvert();
			CHECK_EQUAL(bv.get(3 * BLOCKSIZE), 1);
		}

		{
			BV bv(99999), bv2(0);
			BitVectorUtil::fillRandom(bv, 50);
			bv2 = bv;
			bv.bitInvert();
			bv.bitXor(bv2);
			CHECK_EQUAL(bv.count(), 99999);
		}
	}


	TEST(Or)
	{
		// TODO
	}


	TEST(And)
	{
		// TODO
	}


	TEST(Xor)
	{
		// TODO
	}


}

