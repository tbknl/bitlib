
#ifndef _BITVECTOR_UTIL_H_
#define _BITVECTOR_UTIL_H_

#include <iostream>
#include <cstdlib>
#include <ctime>


#define PPCAT_NX(a,b) a ## b
#define SUITE_X_INTERNAL(n) SUITE(n)
#define SUITE_X(a,b) SUITE_X_INTERNAL(PPCAT_NX(a,b))


class BitVectorUtil
{
	public:

		static void randomSeed()
		{
			srand(time(NULL));
		}


		static void randomSeed(unsigned int s)
		{
			srand(s);
		}


		template <typename BV>
			static void fillRandom(BV& bv, int prob) {
				typename BV::IndexType size = bv.getSize();
				for (typename BV::IndexType i = 0; i < size; ++i) {
					bv.set(i, rand() % 100 < prob);
				}
			}


		template <typename BV>
			void print(const BV& bv, typename BV::IndexType maxBits) {
				std::cout << '[' << bv.count() << "] ";
				typename BV::IndexType size = bv.getSize();
				for (typename BV::IndexType i = 0; i < size && i < maxBits; ++i) {
					std::cout << (bv.get(i) ? '1' : '0');
					if (i % 8 == 7) {
						std::cout << ' ';
					}
				}
				std::cout << std::endl;
			}

};


#endif // _BITVECTOR_UTIL_H_

