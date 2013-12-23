
#ifndef _BITVECTOR_H_
#define _BITVECTOR_H_

#include <new>
#include <cstdlib>
#include <cstring>


namespace bitlib {


static const char countLUT[] = { // TODO: Move somewhere.
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


template <int BlockSize>
struct BitBlockType {
	typedef typename BitBlockType::INVALID_BIT_SIZE type;
};

// TODO! Check at compile time that these types indeed have this size.
template <> struct BitBlockType<64> { typedef unsigned long long type; };
template <> struct BitBlockType<32> { typedef unsigned int type; };
template <> struct BitBlockType<16> { typedef unsigned short type; };
template <> struct BitBlockType<8> { typedef unsigned char type; };


struct BitComputeNormal
{
	template <typename BV>
		static typename BV::IndexType count(const typename BV::BitBlock* const start, const typename BV::BitBlock* const end)
		{
			typename BV::IndexType count = 0;
			const typename BV::BitBlock* p = start;
			for (; p < end; ++p) {
				for (typename BV::IndexType bitIndex = 0; bitIndex < BV::BlockSize; bitIndex += 8) {
					count += countLUT[(*p >> bitIndex) & 0xFF];
				}
			}

			return count;
		}
};


template <int _BlockSize, typename I = unsigned int, typename C = BitComputeNormal>
class BitVector
{
	public:
		typedef I IndexType;
		typedef typename BitBlockType<_BlockSize>::type BitBlock;

		enum {
			BlockSize = sizeof(BitBlock) * 8
		};


	private:
		I size;
		BitBlock* data;


		/**
		 *
		 */
		inline I getBlockCount() const {
			if (this->size == 0) return 1;
			return 1 + (this->size - 1) / BlockSize;
		}


		/**
		 *
		 */
		inline I getDataSizeInBytes() const {
			return this->getBlockCount() * sizeof(BitBlock);
		}


	public:

		/**
		 *
		 */
		BitVector(I size) :
			size(size)
		{
			this->data = (BitBlock*)::calloc(this->getBlockCount(), sizeof(BitBlock));
			if (this->data == NULL) {
				throw std::bad_alloc();
			}
		}


		/**
		 *
		 */
		BitVector(const BitVector& other) :
			data(other.data),
			size(0)
		{
			*this = other;
		}


		/**
		 *
		 */
		BitVector& operator=(const BitVector& other) {
			if (other.size != this->size) {
				this->data = (BitBlock*)::realloc(this->data, other.getDataSizeInBytes());
				if (this->data == NULL) {
					throw std::bad_alloc();
				}
				this->size = other.size;
			}

			if (other.size != 0) {
				memcpy(this->data, other.data, other.getDataSizeInBytes());
			}

			return *this;
		}


		/**
		 *
		 */
		~BitVector() {
			free(this->data);
		}


		/**
		 *
		 */
		I getSize() const {
			return this->size;
		}


		/**
		 *
		 */
		void set(const I& index, BitBlock value) {
			value = value != 0;
			if (value) {
				data[index / BlockSize] |= ((BitBlock)1) << (index % BlockSize);
			}
			else {
				data[index / BlockSize] &= ~(((BitBlock)1) << (index % BlockSize));
			}
		}


		/**
		 *
		 */
		BitBlock get(const I& index) const {
			return (data[index / BlockSize] & (((BitBlock)1) << (index % BlockSize))) != 0;
		}


		/**
		 *
		 */
		I count() const {
			return C::template count<BitVector>(this->data, this->data + this->getBlockCount());
		}


		/**
		 *
		 */
		BitVector& clear() {
			memset(this->data, 0, this->getDataSizeInBytes());
			return *this;
		}


		/**
		 *
		 */
		BitVector& bitInvert() {
			BitBlock* p = this->data;
			BitBlock* p_stop = &p[this->getBlockCount()];
			for (; p < p_stop; ++p) {
				*p = ~(*p);
			}
			if (this->size % BlockSize != 0) {
				*(p-1) &= (((BitBlock)1) << (this->size % BlockSize)) - 1; // Set bits to '0' that fall outside the vector.
			}
			return *this;
		}


		/**
		 *
		 */
		BitVector& bitAnd(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.

			BitBlock* p = this->data;
			BitBlock* p_other = other.data;
			BitBlock* p_stop = &p[this->getBlockCount()];
			for (; p < p_stop; ++p, ++p_other) {
				*p &= *p_other;
			}
			return *this;
		}


		/**
		 *
		 */
		BitVector& bitOr(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.

			BitBlock* p = this->data;
			BitBlock* p_other = other.data;
			BitBlock* p_stop = &p[this->getBlockCount()];
			for (; p < p_stop; ++p, ++p_other) {
				*p |= *p_other;
			}
			return *this;
		}


		/**
		 *
		 */
		BitVector& bitXor(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.

			BitBlock* p = this->data;
			BitBlock* p_other = other.data;
			BitBlock* p_stop = &p[this->getBlockCount()];
			for (; p < p_stop; ++p, ++p_other) {
				*p ^= *p_other;
			}
			return *this;
		}
};


} // namespace bitlib


#endif // _BITVECTOR_H_

