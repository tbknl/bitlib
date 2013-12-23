
#include <bitvector.h>


typedef bitlib::BitVector<64> BV;

int main(int argc, char* argv[])
{
	const int repeats = 10000;
	const BV::IndexType size = 10 * 1000 * 1000;
	BV bv1(size), bv2(size), bv3(0);
	bv1.set(size / 2 + 3, 1);
	bv2.set(size / 3 + 2, 1);

	if (argc > 1 && 0 == strcmp(argv[1], "chain")) {
		for (int r = 0; r < repeats; ++r) {
			bv3 = bv1;
			bv1.chain().bitOr(bv2).bitAnd(bv2).bitXor(bv2).compute();
			bv1.count();
		}
	}
	else {
		for (int r = 0; r < repeats; ++r) {
			bv3 = bv1;
			bv1.bitOr(bv2).bitAnd(bv2).bitXor(bv2);
			bv1.count();
		}
	}

	return 0;
}

