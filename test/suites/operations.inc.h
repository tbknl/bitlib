
SUITE_X(operations, BLOCKSIZE)
{
	typedef bitlib::BitVector<BLOCKSIZE> BV;


	TEST(Invert)
	{
		{
			BV bv(1);
			CHECK_EQUAL(0, bv.get(0));
			bv.bitInvert();
			CHECK_EQUAL(1, bv.get(0));
			bv.bitInvert();
			CHECK_EQUAL(0, bv.get(0));
		}
		
		{
			BV bv(3 * BLOCKSIZE + 1); 
			bv.set(3 * BLOCKSIZE, 1);
			bv.bitInvert();
			CHECK_EQUAL(0, bv.get(3 * BLOCKSIZE));
			bv.bitInvert();
			CHECK_EQUAL(1, bv.get(3 * BLOCKSIZE));
		}

		{
			BV bv(99999), bv2(0);
			bv.bitInvert();
			CHECK_EQUAL(99999, bv.count());
			BitVectorUtil::fillRandom(bv, 50);
			bv2 = bv;
			bv.bitInvert();
			bv.bitXor(bv2);
			CHECK_EQUAL(99999, bv.count());
		}
	}


	TEST(Or)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitOr(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv2.set(0, 1);
			bv1.bitOr(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv1.bitOr(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv2.set(0, 0);
			bv1.bitOr(bv2);
			CHECK_EQUAL(1, bv1.get(0));
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
			CHECK_EQUAL(0, bv1.get(0));
			bv1.set(0, 1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv2.set(0, 1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv1.set(0, 1);
			bv2.set(0, 1);
			bv1.bitAnd(bv2);
			CHECK_EQUAL(1, bv1.get(0));
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
			CHECK_EQUAL(0, bv3.bitXor(bv2).count());
		}
	}


	TEST(Xor)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitXor(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv2.set(0, 1);
			bv1.bitXor(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv1.bitXor(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv1.set(0, 1);
			bv2.set(0, 0);
			bv1.bitXor(bv2);
			CHECK_EQUAL(1, bv1.get(0));
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


	TEST(AndInverted)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitAndInverted(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv1.set(0, 1);
			bv1.bitAndInverted(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv2.set(0, 1);
			bv1.bitAndInverted(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv1.bitAndInverted(bv2);
			CHECK_EQUAL(0, bv1.get(0));
		}

		{
			BV bv1(99999), bv2(99999), bv3(0), bv4(0);
			BitVectorUtil::fillRandom(bv1, 50);
			BitVectorUtil::fillRandom(bv2, 50);
			bv3 = bv1;
			bv4 = bv2;
			bv1.bitAndInverted(bv2);
			bv3.bitAnd(bv4.bitInvert());
			CHECK_EQUAL(0, bv1.bitXor(bv3).count());
		}
	}


	TEST(OrInverted)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitOrInverted(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv1.bitOrInverted(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv2.set(0, 1);
			bv1.bitOrInverted(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv1.set(0, 0);
			bv1.bitOrInverted(bv2);
			CHECK_EQUAL(0, bv1.get(0));
		}

		{
			BV bv1(99999), bv2(99999), bv3(0), bv4(0);
			BitVectorUtil::fillRandom(bv1, 50);
			BitVectorUtil::fillRandom(bv2, 50);
			bv3 = bv1;
			bv4 = bv2;
			bv1.bitOrInverted(bv2);
			bv3.bitOr(bv4.bitInvert());
			CHECK_EQUAL(0, bv1.bitXor(bv3).count());
		}
	}


	TEST(XorInverted)
	{
		{
			BV bv1(1), bv2(1);
			bv1.bitXorInverted(bv2);
			CHECK_EQUAL(1, bv1.get(0));
			bv1.bitXorInverted(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv2.set(0, 1);
			bv1.bitXorInverted(bv2);
			CHECK_EQUAL(0, bv1.get(0));
			bv1.bitXorInverted(bv2);
			bv1.set(0, 1);
			CHECK_EQUAL(1, bv1.get(0));
		}

		{
			BV bv1(99999), bv2(99999), bv3(0), bv4(0);
			BitVectorUtil::fillRandom(bv1, 50);
			BitVectorUtil::fillRandom(bv2, 50);
			bv3 = bv1;
			bv4 = bv2;
			bv1.bitXorInverted(bv2);
			bv3.bitXor(bv4.bitInvert());
			CHECK_EQUAL(0, bv1.bitXor(bv3).count());
		}
	}

}

