#include <iostream>
#include <ctime>
#include <cstdlib>


#include <bitvector.h>


//typedef BitVector<unsigned short> BV;
//typedef BitVector<unsigned int> BV;
typedef bitlib::BitVector<unsigned long, unsigned long> BV;


void fillRandom(BV& bv, int prob) {
	BV::IndexType size = bv.getSize();
	for (BV::IndexType i = 0; i < size; ++i) {
		bv.set(i, rand() % 100 < prob);
	}
}


void print(const BV& bv, BV::IndexType maxBits) {
	std::cout << '[' << bv.count() << "] ";
	BV::IndexType size = bv.getSize();
	for (BV::IndexType i = 0; i < size && i < maxBits; ++i) {
		std::cout << (bv.get(i) ? '1' : '0');
		if (i % 8 == 7) {
			std::cout << ' ';
		}
	}
	std::cout << std::endl;
}


int main(int argc, char* argv[]) {
	srand(time(NULL));

	const BV::IndexType numBits = 10 * 1000 * 1000;
	BV bv1(numBits), bv2(numBits), bv3(0);
	fillRandom(bv1, 30);
	fillRandom(bv2, 50);
	bv3 = bv1;
	print(bv1, 50);
	print(bv2, 50);

	std::cout << "TSize: " << BV::TSize << std::endl;

	for (int r = 0; r < 1000; ++r) {
		bv1.bitAnd(bv2);
		bv1.bitXor(bv2);
		bv1 = bv3;
		bv1.bitOr(bv2);
	}

	print(bv1, 50);

	return 0;
}

