
SUITE_X(opchain, BLOCKSIZE)
{
	typedef bitlib::BitVector<BLOCKSIZE> BV;


	TEST(Chain)
	{
		BV bv1(99999), bv2(99999), bv3(99999), bv1_mirror(0);
		BitVectorUtil::fillRandom(bv2, 50);
		BitVectorUtil::fillRandom(bv3, 50);

		BitVectorUtil::fillRandom(bv1, 50);
		bv1_mirror = bv1;
		bv1.chain().bitInvert().bitOr(bv2).bitAndInverted(bv3).compute();
		bv1_mirror.bitInvert().bitOr(bv2).bitAndInverted(bv3);
		CHECK_EQUAL(0, bv1.bitXor(bv1_mirror).count());

		BitVectorUtil::fillRandom(bv1, 50);
		bv1_mirror = bv1;
		bv1.chain().bitXor(bv2).bitAnd(bv3).compute();
		bv1_mirror.bitXor(bv2).bitAnd(bv3);
		CHECK_EQUAL(0, bv1.bitXor(bv1_mirror).count());

		BitVectorUtil::fillRandom(bv1, 50);
		bv1_mirror = bv1;
		bv1.chain().bitOrInverted(bv2).bitXorInverted(bv3).compute();
		bv1_mirror.bitOrInverted(bv2).bitXorInverted(bv3);
		CHECK_EQUAL(0, bv1.bitXor(bv1_mirror).count());
	}

}

