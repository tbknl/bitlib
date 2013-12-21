#include <iostream>
#include <ctime>


#include <cstdlib>
#include <cstring>


typedef unsigned char byte;
byte countLUT[] = {
	0, 1, 1, 2, 1, 2, 2, 3,
	1, 2, 2, 3, 2, 3, 3, 4,
	1, 2, 2, 3, 2, 3, 3, 4,
	2, 3, 3, 4, 3, 4, 4, 5,
	1, 2, 2, 3, 2, 3, 3, 4,
	2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5,
	3, 4, 4, 5, 4, 5, 5, 6,
	1, 2, 2, 3, 2, 3, 3, 4,
	2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5,
	3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5,
	3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6,
	4, 5, 5, 6, 5, 6, 6, 7,
	1, 2, 2, 3, 2, 3, 3, 4,
	2, 3, 3, 4, 3, 4, 4, 5,
	2, 3, 3, 4, 3, 4, 4, 5,
	3, 4, 4, 5, 4, 5, 5, 6,
	2, 3, 3, 4, 3, 4, 4, 5,
	3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6,
	4, 5, 5, 6, 5, 6, 6, 7,
	2, 3, 3, 4, 3, 4, 4, 5,
	3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6,
	4, 5, 5, 6, 5, 6, 6, 7,
	3, 4, 4, 5, 4, 5, 5, 6,
	4, 5, 5, 6, 5, 6, 6, 7,
	4, 5, 5, 6, 5, 6, 6, 7,
	5, 6, 6, 7, 6, 7, 7, 8
};


template <typename T, typename I = unsigned int>
class BitVector
{
	public:
		typedef I IndexType;

		enum {
			TSizeBytes = sizeof(T),
			TSize = TSizeBytes * 8
		};

	private:
		I size;
		T* data;

		inline I getTSize() const {
			if (this->size == 0) return 1;
			return 1 + (this->size - 1) / TSize;
		}

		inline I getTSizeBytes() const {
			return this->getTSize() * 8;
		}

	public:
		BitVector(I size) :
			size(size)
		{
			this->data = (T*)::calloc(this->getTSize(), TSize);
			if (this->data == NULL) {
				throw std::bad_alloc();
			}
		}

		BitVector(const BitVector& other) :
			data(other.data),
			size(0)
		{
			*this = other;
		}

		BitVector& operator=(const BitVector& other) {
			if (other.size != this->size) {
				this->data = (T*)::realloc(this->data, other.getTSizeBytes());
				if (this->data == NULL) {
					throw std::bad_alloc();
				}
				this->size = other.size;
			}

			if (other.size != 0) {
				memcpy(this->data, other.data, other.getTSizeBytes());
			}
		}

		~BitVector() {
			free(this->data);
		}

		I getSize() const {
			return this->size;
		}

		void set(const I& index, T value) {
			value = value != 0;
			if (value) {
				data[index / TSize] |= 1 << (index % TSize);
			}
			else {
				data[index / TSize] &= ~(1 << (index % TSize));
			}
		}

		T get(const I& index) const {
			return (data[index / TSize] & (1 << (index % TSize))) != 0;
		}

		I count() const {
			I count = 0;
			T* p = this->data;
			T* p_stop = &p[this->getTSize()];
			for (; p < p_stop; ++p) {
				for (I bitIndex = 0; bitIndex < TSize; bitIndex += 8) {
					count += countLUT[(*p >> bitIndex) & 0xFF];
				}
			}
			return count;
		}

		BitVector& clear() {
			memset(this->data, 0, this->getTSizeBytes());
			return *this;
		}

		BitVector& bitAnd(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.

			T* p = this->data;
			T* p_other = other.data;
			T* p_stop = &p[this->getTSize()];
			for (; p < p_stop; ++p, ++p_other) {
				*p &= *p_other;
			}
			return *this;
		}

		BitVector& bitOr(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.

			T* p = this->data;
			T* p_other = other.data;
			T* p_stop = &p[this->getTSize()];
			for (; p < p_stop; ++p, ++p_other) {
				*p |= *p_other;
			}
			return *this;
		}

		BitVector& bitXor(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.

			T* p = this->data;
			T* p_other = other.data;
			T* p_stop = &p[this->getTSize()];
			for (; p < p_stop; ++p, ++p_other) {
				*p ^= *p_other;
			}
			return *this;
		}
};

//typedef BitVector<unsigned short> BV;
//typedef BitVector<unsigned int> BV;
typedef BitVector<unsigned long, unsigned long> BV;


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

