
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
			bv.bitInvert();
			CHECK_EQUAL(bv.count(), 99999);
			BitVectorUtil::fillRandom(bv, 50);
			bv2 = bv;
			bv.bitInvert();
			bv.bitXor(bv2);
			CHECK_EQUAL(bv.count(), 99999);
		}
	}


	TEST(Or)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitOr(bv2);
			CHECK_EQUAL(bv1.get(0), 0);
			bv2.set(0, 1);
			bv1.bitOr(bv2);
			CHECK_EQUAL(bv1.get(0), 1);
			bv1.bitOr(bv2);
			CHECK_EQUAL(bv1.get(0), 1);
			bv2.set(0, 0);
			bv1.bitOr(bv2);
			CHECK_EQUAL(bv1.get(0), 1);
		}

		{
			BV bv1(99999), bv2(99999), bv3(0), bv4(0);
			BitVectorUtil::fillRandom(bv1, 50);
			BitVectorUtil::fillRandom(bv2, 50);
			bv4 = bv3 = bv1;
			bv3.bitOr(bv2);
			bv4.bitAnd(bv2);
			CHECK_EQUAL(bv3.count() + bv4.count(), bv1.count() + bv2.count());
		}
	}


	TEST(And)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(bv1.get(0), 0);
			bv1.set(0, 1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(bv1.get(0), 0);
			bv2.set(0, 1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(bv1.get(0), 0);
			bv1.set(0, 1);
			bv2.set(0, 1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(bv1.get(0), 1);
		}

		{
			BV bv1(99999), bv2(99999), bv3(0);
			BitVectorUtil::fillRandom(bv1, 50);
			BitVectorUtil::fillRandom(bv2, 50);
			bv3 = bv1;
			bv3.bitAnd(bv2);
			bv2.bitInvert();
			bv1.bitInvert();
			bv2.bitOr(bv1);
			bv2.bitInvert();
			bv3.bitXor(bv2);
			CHECK_EQUAL(bv3.count(), 0);
		}
	}


	TEST(Xor)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitXor(bv2);
			CHECK_EQUAL(bv1.get(0), 0);
			bv2.set(0, 1);
			bv1.bitXor(bv2);
			CHECK_EQUAL(bv1.get(0), 1);
			bv1.bitXor(bv2);
			CHECK_EQUAL(bv1.get(0), 0);
			bv1.set(0, 1);
			bv2.set(0, 0);
			bv1.bitXor(bv2);
			CHECK_EQUAL(bv1.get(0), 1);
		}

		{
			BV bv1(99999), bv2(99999), bv3(0), bv4(0);
			BitVectorUtil::fillRandom(bv1, 50);
			BitVectorUtil::fillRandom(bv2, 50);
			bv4 = bv3 = bv1;
			bv3.bitOr(bv2);
			bv4.bitAnd(bv2);
			CHECK_EQUAL(bv3.count() + bv4.count(), bv1.count() + bv2.count());
		}
	}


}

