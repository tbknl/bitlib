
SUITE_X(getset, BLOCKSIZE)
{
	typedef bitlib::BitVector<BLOCKSIZE> BV;


	TEST(GetAndSet)
	{
		BV bv(2 * BLOCKSIZE);

		CHECK_EQUAL(0, bv.get(0));
		bv.set(0, 1);
		CHECK_EQUAL(1, bv.get(0));
		bv.set(0, 0);
		CHECK_EQUAL(0, bv.get(0));

		CHECK_EQUAL(0, bv.get(2 * BLOCKSIZE - 1));
		bv.set(2 * BLOCKSIZE - 1, 1);
		CHECK_EQUAL(1, bv.get(2 * BLOCKSIZE - 1));
		bv.set(2 * BLOCKSIZE - 1, 0);
		CHECK_EQUAL(0, bv.get(2 * BLOCKSIZE - 1));
	}


	TEST(Toggle)
	{
		BV bv(BLOCKSIZE);

		CHECK_EQUAL(0, bv.get(3));
		bv.toggle(3);
		CHECK_EQUAL(1, bv.get(3));
		bv.toggle(3);
		CHECK_EQUAL(0, bv.get(3));
	}

}

