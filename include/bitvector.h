
#ifndef _BITVECTOR_H_
#define _BITVECTOR_H_

#include <new>
#include <vector>
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
	/**
	 *
	 */
	template <typename BV>
		static typename BV::IndexType count(
			const typename BV::BitBlock* data,
			const typename BV::BitBlock* const end
		)
		{
			typename BV::IndexType count = 0;
			for (; data < end; ++data) {
				for (typename BV::IndexType bitIndex = 0; bitIndex < BV::BlockSize; bitIndex += 8) {
					count += countLUT[(*data >> bitIndex) & 0xFF];
				}
			}

			return count;
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitInvert(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data) {
				*data = ~(*data);
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitAnd(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* data2,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data, ++data2) {
				*data &= *data2;
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitAndInverted(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* data2,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data, ++data2) {
				*data &= ~(*data2);
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitOr(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* data2,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data, ++data2) {
				*data |= *data2;
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitOrInverted(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* data2,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data, ++data2) {
				*data |= ~(*data2);
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitXor(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* data2,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data, ++data2) {
				*data ^= *data2;
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void bitXorInverted(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* data2,
			const typename BV::BitBlock* const end
		)
		{
			for (; data < end; ++data, ++data2) {
				*data ^= ~(*data2);
			}
		}


	/**
	 *
	 */
	template <typename BV>
		static void chainCompute(
			typename BV::BitBlock* data,
			const typename BV::BitBlock* const end,
			const typename BV::OpChain& chain
		)
		{
			for (; data < end; ++data) {
				typename BV::OpChain::OpList::const_iterator it = chain.operations.begin();
				for (; it != chain.operations.end(); ++it) {
					const typename BV::OpChain::Operation& op = *it;

					switch (op.type) {
						case BV::OpChain::Operation::INVERT:
							*data = ~(*data);
							break;
						case BV::OpChain::Operation::AND:
							*data &= *(op.data++);
							break;
						case BV::OpChain::Operation::OR:
							*data |= *(op.data++);
							break;
						case BV::OpChain::Operation::XOR:
							*data ^= *(op.data++);
							break;
						case BV::OpChain::Operation::AND_INVERTED:
							*data &= ~(*(op.data++));
							break;
						case BV::OpChain::Operation::OR_INVERTED:
							*data |= ~(*(op.data++));
							break;
						case BV::OpChain::Operation::XOR_INVERTED:
							*data ^= ~(*(op.data++));
							break;
					}
				}
			}
		}


};


template <int _BlockSize, typename I = unsigned int, typename Compute = BitComputeNormal>
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


		/**
		 * Make sure that bit values are '0' for bits that fall outside the vector, but
		 * within the final allocated block.
		 */
		inline void clearOutsideBits()
		{
			if (this->size % BlockSize != 0) {
				*(this->data + this->getBlockCount() - 1) &= (((BitBlock)1) << (this->size % BlockSize)) - 1;
			}
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
		 * Toggle a single bit by index.
		 */
		void toggle(const I& index) const {
			data[index / BlockSize] ^= ((BitBlock)1) << (index % BlockSize);
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
		BitVector& clear() {
			memset(this->data, 0, this->getDataSizeInBytes());
			return *this;
		}


		/**
		 *
		 */
		I count() const {
			return Compute::template count<BitVector>(this->data, this->data + this->getBlockCount());
		}


		/**
		 * Invert/toggle the values of all bits in the vector.
		 */
		BitVector& bitInvert() {
			Compute::template bitInvert<BitVector>(this->data, this->data + this->getBlockCount());
			this->clearOutsideBits();
			return *this;
		}


		/**
		 * Compute bitwise AND.
		 */
		BitVector& bitAnd(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.
			Compute::template bitAnd<BitVector>(this->data, other.data, this->data + this->getBlockCount());
			return *this;
		}


		/**
		 * Compute bitwise AND with inverted value of the argument.
		 */
		BitVector& bitAndInverted(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.
			Compute::template bitAndInverted<BitVector>(this->data, other.data, this->data + this->getBlockCount());
			this->clearOutsideBits();
			return *this;
		}


		/**
		 * Compute bitwise OR.
		 */
		BitVector& bitOr(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.
			Compute::template bitOr<BitVector>(this->data, other.data, this->data + this->getBlockCount());
			return *this;
		}


		/**
		 * Compute bitwise OR with inverted value of the argument.
		 */
		BitVector& bitOrInverted(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.
			Compute::template bitOrInverted<BitVector>(this->data, other.data, this->data + this->getBlockCount());
			this->clearOutsideBits();
			return *this;
		}


		/**
		 * Compute bitwise XOR.
		 */
		BitVector& bitXor(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.
			Compute::template bitXor<BitVector>(this->data, other.data, this->data + this->getBlockCount());
			return *this;
		}


		/**
		 * Compute bitwise XOR with inverted value of the argument.
		 */
		BitVector& bitXorInverted(const BitVector& other) {
			if (other.size != this->size) { return *this; } // For now we don't do anything.
			Compute::template bitXorInverted<BitVector>(this->data, other.data, this->data + this->getBlockCount());
			this->clearOutsideBits();
			return *this;
		}


		/**
		 * Definition of an Operation Chain.
		 */
		struct OpChain
		{
			struct Operation
			{
				enum {
					INVERT,
					AND,
					OR,
					XOR,
					AND_INVERTED,
					OR_INVERTED,
					XOR_INVERTED
				};

				int type;
				const BitVector* arg;
				mutable const BitBlock* data;
			};

			typedef std::vector<Operation> OpList;

			BitVector& operand;
			OpList operations;

			OpChain(BitVector& bv) : operand(bv) {}

			OpChain& bitInvert()
			{
				const Operation op = {Operation::INVERT, NULL, NULL};
				this->operations.push_back(op);
				return *this;
			}

			OpChain& bitAnd(const BitVector& arg)
			{
				const Operation op = {Operation::AND, &arg, NULL};
				if (arg.getSize() == operand.getSize()) { // For now, ignore bitvector with unequal lengths.
					this->operations.push_back(op);
				}
				return *this;
			}

			OpChain& bitOr(const BitVector& arg)
			{
				const Operation op = {Operation::OR, &arg, NULL};
				if (arg.getSize() == operand.getSize()) { // For now, ignore bitvector with unequal lengths.
					this->operations.push_back(op);
				}
				return *this;
			}

			OpChain& bitXor(const BitVector& arg)
			{
				const Operation op = {Operation::XOR, &arg, NULL};
				if (arg.getSize() == operand.getSize()) { // For now, ignore bitvector with unequal lengths.
					this->operations.push_back(op);
				}
				return *this;
			}

			OpChain& bitAndInverted(const BitVector& arg)
			{
				const Operation op = {Operation::AND_INVERTED, &arg, NULL};
				if (arg.getSize() == operand.getSize()) { // For now, ignore bitvector with unequal lengths.
					this->operations.push_back(op);
				}
				return *this;
			}

			OpChain& bitOrInverted(const BitVector& arg)
			{
				const Operation op = {Operation::OR_INVERTED, &arg, NULL};
				if (arg.getSize() == operand.getSize()) { // For now, ignore bitvector with unequal lengths.
					this->operations.push_back(op);
				}
				return *this;
			}

			OpChain& bitXorInverted(const BitVector& arg)
			{
				const Operation op = {Operation::XOR_INVERTED, &arg, NULL};
				if (arg.getSize() == operand.getSize()) { // For now, ignore bitvector with unequal lengths.
					this->operations.push_back(op);
				}
				return *this;
			}

			BitVector& compute()
			{
				return this->operand.chainCompute(*this);
			}
		};


		/**
		 * Start an operation chain.
		 */
		OpChain chain()
		{
			return OpChain(*this);
		}


		/*
		 * Compute an operation chain.
		 * Note: Normally this will not be called by the client code.
		 */
		BitVector& chainCompute(const OpChain& chain)
		{
			// Set the data pointer of all operation arguments:
			typename OpChain::OpList::const_iterator it = chain.operations.begin();
			for (; it != chain.operations.end(); ++it) {
				const typename OpChain::Operation& op = *it;
				if (op.arg != NULL) {
					op.data = op.arg->data;
				}
			}

			Compute::template chainCompute<BitVector>(this->data, this->data + this->getBlockCount(), chain);
			this->clearOutsideBits();
			return *this;
		}

};


} // namespace bitlib


#endif // _BITVECTOR_H_

