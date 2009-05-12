#include "BitfieldMan.h"

#include <cstring>

#include <cppunit/extensions/HelperMacros.h>

#include "FixedNumberRandomizer.h"
#include "bitfield.h"

namespace aria2 {

class BitfieldManTest:public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(BitfieldManTest);
  CPPUNIT_TEST(testGetBlockSize);
  CPPUNIT_TEST(testGetFirstMissingUnusedIndex);
  CPPUNIT_TEST(testGetFirstMissingIndex);
  CPPUNIT_TEST(testIsAllBitSet);
  CPPUNIT_TEST(testFilter);
  CPPUNIT_TEST(testAddFilter_zeroLength);
  CPPUNIT_TEST(testGetMissingIndex);
  CPPUNIT_TEST(testGetSparceMissingUnusedIndex);
  CPPUNIT_TEST(testGetSparceMissingUnusedIndex_setBit);
  CPPUNIT_TEST(testIsBitSetOffsetRange);
  CPPUNIT_TEST(testGetMissingUnusedLength);
  CPPUNIT_TEST(testSetBitRange);
  CPPUNIT_TEST(testGetAllMissingIndexes);
  CPPUNIT_TEST(testGetAllMissingIndexes_noarg);
  CPPUNIT_TEST(testGetAllMissingIndexes_checkLastByte);
  CPPUNIT_TEST(testGetAllMissingUnusedIndexes);
  CPPUNIT_TEST(testGetMissingUnusedIndex);
  CPPUNIT_TEST(testGetMissingIndex_noarg);
  CPPUNIT_TEST(testGetMissingUnusedIndex_noarg);
  CPPUNIT_TEST(testCountFilteredBlock);
  CPPUNIT_TEST(testCountMissingBlock);
  CPPUNIT_TEST(testZeroLengthFilter);
  CPPUNIT_TEST_SUITE_END();
private:
  SharedHandle<Randomizer> fixedNumberRandomizer;

public:
  BitfieldManTest() {
    FixedNumberRandomizer* randomizer = new FixedNumberRandomizer();
    randomizer->setFixedNumber(0);
    this->fixedNumberRandomizer.reset(randomizer);
  }

  void setUp() {
  }

  void testGetBlockSize();
  void testGetFirstMissingUnusedIndex();
  void testGetFirstMissingIndex();
  void testGetMissingIndex();
  void testGetMissingIndex_noarg();
  void testGetMissingUnusedIndex();
  void testGetMissingUnusedIndex_noarg();
  void testGetAllMissingIndexes();
  void testGetAllMissingIndexes_noarg();
  void testGetAllMissingIndexes_checkLastByte();
  void testGetAllMissingUnusedIndexes();
  
  void testIsAllBitSet();
  void testFilter();
  void testAddFilter_zeroLength();
  void testGetSparceMissingUnusedIndex();
  void testGetSparceMissingUnusedIndex_setBit();
  void testIsBitSetOffsetRange();
  void testGetMissingUnusedLength();
  void testSetBitRange();
  void testCountFilteredBlock();
  void testCountMissingBlock();
  void testZeroLengthFilter();
};


CPPUNIT_TEST_SUITE_REGISTRATION( BitfieldManTest );

void BitfieldManTest::testGetBlockSize() {
  BitfieldMan bt1(1024, 1024*10);
  CPPUNIT_ASSERT_EQUAL((size_t)1024, bt1.getBlockLength(9));

  BitfieldMan bt2(1024, 1024*10+1);
  CPPUNIT_ASSERT_EQUAL((size_t)1024, bt2.getBlockLength(9));
  CPPUNIT_ASSERT_EQUAL((size_t)1, bt2.getBlockLength(10));
  CPPUNIT_ASSERT_EQUAL((size_t)0, bt2.getBlockLength(11));
}

void BitfieldManTest::testGetFirstMissingUnusedIndex()
{
  {
    BitfieldMan bt1(1024, 1024*10);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)0, index);
    }
    bt1.setUseBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.unsetUseBit(0);
    bt1.setBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setAllBit();
    {
      size_t index;
      CPPUNIT_ASSERT(!bt1.getFirstMissingUnusedIndex(index));
    }
  }
  {
    BitfieldMan bt1(1024, 1024*10);

    bt1.addFilter(1024, 1024*10);
    bt1.enableFilter();
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setUseBit(1);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)2, index);
    }
    bt1.setBit(2);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)3, index);
    }
  }
}

void BitfieldManTest::testGetFirstMissingIndex()
{
  {
    BitfieldMan bt1(1024, 1024*10);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)0, index);
    }
    bt1.setUseBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)0, index);
    }
    bt1.unsetUseBit(0);
    bt1.setBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setAllBit();
    {
      size_t index;
      CPPUNIT_ASSERT(!bt1.getFirstMissingIndex(index));
    }
  }
  {
    BitfieldMan bt1(1024, 1024*10);

    bt1.addFilter(1024, 1024*10);
    bt1.enableFilter();
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setUseBit(1);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setBit(1);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getFirstMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)2, index);
    }
  }
}

void BitfieldManTest::testGetMissingUnusedIndex_noarg()
{
  {
    BitfieldMan bt1(1024, 1024*10);
    bt1.setRandomizer(fixedNumberRandomizer);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)0, index);
    }
    bt1.setUseBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.unsetUseBit(0);
    bt1.setBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setAllBit();
    {
      size_t index;
      CPPUNIT_ASSERT(!bt1.getMissingUnusedIndex(index));
    }
  }
  {
    BitfieldMan bt1(1024, 1024*10);
    bt1.setRandomizer(fixedNumberRandomizer);

    bt1.addFilter(1024, 1024*10);
    bt1.enableFilter();
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setUseBit(1);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)2, index);
    }
    bt1.setBit(2);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)3, index);
    }
  }
}

void BitfieldManTest::testGetMissingIndex_noarg()
{
  {
    BitfieldMan bt1(1024, 1024*10);
    bt1.setRandomizer(fixedNumberRandomizer);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)0, index);
    }
    bt1.setUseBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)0, index);
    }
    bt1.unsetUseBit(0);
    bt1.setBit(0);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setAllBit();
    {
      size_t index;
      CPPUNIT_ASSERT(!bt1.getMissingIndex(index));
    }
  }
  {
    BitfieldMan bt1(1024, 1024*10);
    bt1.setRandomizer(fixedNumberRandomizer);

    bt1.addFilter(1024, 1024*10);
    bt1.enableFilter();
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setUseBit(1);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    }
    bt1.setBit(1);
    {
      size_t index;
      CPPUNIT_ASSERT(bt1.getMissingIndex(index));
      CPPUNIT_ASSERT_EQUAL((size_t)2, index);
    }
  }
}

void BitfieldManTest::testIsAllBitSet() {
  BitfieldMan bt1(1024, 1024*10);
  CPPUNIT_ASSERT(!bt1.isAllBitSet());
  bt1.setBit(1);
  CPPUNIT_ASSERT(!bt1.isAllBitSet());
  
  for(size_t i = 0; i < 8; i++) {
    CPPUNIT_ASSERT(bt1.setBit(i));
  }
  CPPUNIT_ASSERT(!bt1.isAllBitSet());

  for(size_t i = 0; i < bt1.countBlock(); i++) {
    CPPUNIT_ASSERT(bt1.setBit(i));
  }
  CPPUNIT_ASSERT(bt1.isAllBitSet());

  BitfieldMan btzero(1024, 0);
  CPPUNIT_ASSERT(btzero.isAllBitSet());
}

void BitfieldManTest::testFilter() {
  BitfieldMan btman(2, 32);
  btman.setRandomizer(fixedNumberRandomizer);

  // test offset=4, length=12
  btman.addFilter(4, 12);
  btman.enableFilter();
  unsigned char peerBt[2];
  memset(peerBt, 0xff, sizeof(peerBt));

  size_t index;
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)2, index);
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)3, index);
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)4, index);
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)5, index);
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)6, index);
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)7, index);
  CPPUNIT_ASSERT(!btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  CPPUNIT_ASSERT_EQUAL((uint64_t)12ULL, btman.getFilteredTotalLength());

  // test offset=5, length=2
  btman.clearAllBit();
  btman.clearAllUseBit();
  btman.clearFilter();
  btman.addFilter(5, 2);
  btman.enableFilter();
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  btman.setBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)2, index);
  CPPUNIT_ASSERT(btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  btman.setUseBit(index);
  btman.setBit(index);
  CPPUNIT_ASSERT_EQUAL((size_t)3, index);
  CPPUNIT_ASSERT(!btman.getMissingUnusedIndex(index, peerBt, sizeof(peerBt)));
  CPPUNIT_ASSERT_EQUAL((uint64_t)4ULL, btman.getFilteredTotalLength());
  CPPUNIT_ASSERT(btman.isFilteredAllBitSet());

  BitfieldMan btman2(2, 31);
  btman2.addFilter(0, 31);
  btman2.enableFilter();
  CPPUNIT_ASSERT_EQUAL((uint64_t)31ULL, btman2.getFilteredTotalLength());

}

void BitfieldManTest::testAddFilter_zeroLength()
{
  BitfieldMan bits(1024, 1024*1024);
  bits.addFilter(2048, 0);
  bits.enableFilter();
  CPPUNIT_ASSERT_EQUAL((size_t)0, bits.countMissingBlock());
  CPPUNIT_ASSERT(bits.isFilteredAllBitSet());
}

void BitfieldManTest::testGetMissingIndex() {
  BitfieldMan bt1(1024, 1024*256);
  bt1.setRandomizer(fixedNumberRandomizer);

  unsigned char bitArray[] = {
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
  };
  size_t index;
  CPPUNIT_ASSERT(bt1.getMissingIndex(index, bitArray, 32));
  CPPUNIT_ASSERT_EQUAL((size_t)0, index);

  bt1.addFilter(1024, 1024*256);
  bt1.enableFilter();
  CPPUNIT_ASSERT(bt1.getMissingIndex(index, bitArray, 32));
  CPPUNIT_ASSERT_EQUAL((size_t)1, index);
  bt1.disableFilter();

  unsigned char bitArray2[] = {
    0x0f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
  };

  CPPUNIT_ASSERT(bt1.getMissingIndex(index, bitArray2, 32));
  CPPUNIT_ASSERT_EQUAL((size_t)4, index);

  unsigned char bitArray3[] = {
    0x00, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
  };

  CPPUNIT_ASSERT(bt1.getMissingIndex(index, bitArray3, 32));
  CPPUNIT_ASSERT_EQUAL((size_t)8, index);

  unsigned char bitArray4[] = {
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
  };

  CPPUNIT_ASSERT(!bt1.getMissingIndex(index, bitArray4, 32));

}

void BitfieldManTest::testGetSparceMissingUnusedIndex() {
  BitfieldMan bitfield(1024*1024, 10*1024*1024);

  size_t index;
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)0, index);
  bitfield.setUseBit(0);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)5, index);
  bitfield.setUseBit(5);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)3, index);
  bitfield.setUseBit(3);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)8, index);
  bitfield.setUseBit(8);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)2, index);
  bitfield.setUseBit(2);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)7, index);
  bitfield.setUseBit(7);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)1, index);
  bitfield.setUseBit(1);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)4, index);
  bitfield.setUseBit(4);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)6, index);
  bitfield.setUseBit(6);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)9, index);
  bitfield.setUseBit(9);
  CPPUNIT_ASSERT(!bitfield.getSparseMissingUnusedIndex(index));
}

void BitfieldManTest::testGetSparceMissingUnusedIndex_setBit() {
  BitfieldMan bitfield(1024*1024, 10*1024*1024);
  size_t index;
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)0, index);
  bitfield.setBit(0);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)1, index);
  bitfield.setBit(1);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)2, index);
  bitfield.setBit(2);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)3, index);
  bitfield.setBit(3);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)4, index);
  bitfield.setBit(4);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)5, index);
  bitfield.setBit(5);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)6, index);
  bitfield.setBit(6);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)7, index);
  bitfield.setBit(7);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)8, index);
  bitfield.setBit(8);
  CPPUNIT_ASSERT(bitfield.getSparseMissingUnusedIndex(index));
  CPPUNIT_ASSERT_EQUAL((size_t)9, index);
  bitfield.setBit(9);
  CPPUNIT_ASSERT(!bitfield.getSparseMissingUnusedIndex(index));
}

void BitfieldManTest::testIsBitSetOffsetRange()
{
  int64_t totalLength = 4ULL*1024*1024*1024;
  int32_t pieceLength = 4*1024*1024;
  BitfieldMan bitfield(pieceLength, totalLength);
  bitfield.setAllBit();

  CPPUNIT_ASSERT(!bitfield.isBitSetOffsetRange(0, 0));
  CPPUNIT_ASSERT(!bitfield.isBitSetOffsetRange(totalLength, 100));
  CPPUNIT_ASSERT(!bitfield.isBitSetOffsetRange(totalLength+1, 100));

  CPPUNIT_ASSERT(bitfield.isBitSetOffsetRange(0, totalLength));
  CPPUNIT_ASSERT(bitfield.isBitSetOffsetRange(0, totalLength+1));

  bitfield.clearAllBit();

  bitfield.setBit(100);
  bitfield.setBit(101);
  
  CPPUNIT_ASSERT(bitfield.isBitSetOffsetRange(pieceLength*100, pieceLength*2));
  CPPUNIT_ASSERT(!bitfield.isBitSetOffsetRange(pieceLength*100-10, pieceLength*2));
  CPPUNIT_ASSERT(!bitfield.isBitSetOffsetRange(pieceLength*100, pieceLength*2+1));
    
  bitfield.clearAllBit();

  bitfield.setBit(100);
  bitfield.setBit(102);

  CPPUNIT_ASSERT(!bitfield.isBitSetOffsetRange(pieceLength*100, pieceLength*3));
}

void BitfieldManTest::testGetMissingUnusedLength()
{
  uint64_t totalLength = 1024*10+10;
  size_t blockLength = 1024;

  BitfieldMan bf(blockLength, totalLength);

  // from index 0 and all blocks are unused and not acquired.
  CPPUNIT_ASSERT_EQUAL(totalLength, bf.getMissingUnusedLength(0));

  // from index 10 and all blocks are unused and not acquired.
  CPPUNIT_ASSERT_EQUAL((uint64_t)10ULL, bf.getMissingUnusedLength(10));

  // from index 11
  CPPUNIT_ASSERT_EQUAL((uint64_t)0ULL, bf.getMissingUnusedLength(11));

  // from index 12
  CPPUNIT_ASSERT_EQUAL((uint64_t)0ULL, bf.getMissingUnusedLength(12));

  // from index 0 and 5th block is used.
  bf.setUseBit(5);
  CPPUNIT_ASSERT_EQUAL((uint64_t)5ULL*blockLength, bf.getMissingUnusedLength(0));

  // from index 0 and 4th block is acquired.
  bf.setBit(4);
  CPPUNIT_ASSERT_EQUAL((uint64_t)4ULL*blockLength, bf.getMissingUnusedLength(0));

  // from index 1
  CPPUNIT_ASSERT_EQUAL((uint64_t)3ULL*blockLength, bf.getMissingUnusedLength(1));
}

void BitfieldManTest::testSetBitRange()
{
  size_t blockLength = 1024*1024;
  uint64_t totalLength = 10*blockLength;

  BitfieldMan bf(blockLength, totalLength);

  bf.setBitRange(0, 4);

  for(size_t i = 0; i < 5; ++i) {
    CPPUNIT_ASSERT(bf.isBitSet(i));
  }
  for(size_t i = 5; i < 10; ++i) {
    CPPUNIT_ASSERT(!bf.isBitSet(i));
  }
  CPPUNIT_ASSERT_EQUAL((uint64_t)5ULL*blockLength, bf.getCompletedLength());
}

void BitfieldManTest::testGetAllMissingIndexes_noarg()
{
  size_t blockLength = 16*1024;
  uint64_t totalLength = 1024*1024;
  size_t nbits = (totalLength+blockLength-1)/blockLength;
  BitfieldMan bf(blockLength, totalLength);
  unsigned char misbitfield[8];
  CPPUNIT_ASSERT(bf.getAllMissingIndexes(misbitfield, sizeof(misbitfield)));
  CPPUNIT_ASSERT_EQUAL((size_t)64, bitfield::countSetBit(misbitfield, nbits));

  for(size_t i = 0; i < 63; ++i) {
    bf.setBit(i);
  }
  CPPUNIT_ASSERT(bf.getAllMissingIndexes(misbitfield, sizeof(misbitfield)));
  CPPUNIT_ASSERT_EQUAL((size_t)1, bitfield::countSetBit(misbitfield, nbits));
  CPPUNIT_ASSERT(bitfield::test(misbitfield, nbits, 63));
}

// See garbage bits of last byte are 0
void BitfieldManTest::testGetAllMissingIndexes_checkLastByte()
{
  size_t blockLength = 16*1024;
  uint64_t totalLength = blockLength*2;
  size_t nbits = (totalLength+blockLength-1)/blockLength;
  BitfieldMan bf(blockLength, totalLength);
  unsigned char misbitfield[1];
  CPPUNIT_ASSERT(bf.getAllMissingIndexes(misbitfield, sizeof(misbitfield)));
  CPPUNIT_ASSERT_EQUAL((size_t)2, bitfield::countSetBit(misbitfield, nbits));
  CPPUNIT_ASSERT(bitfield::test(misbitfield, nbits, 0));
  CPPUNIT_ASSERT(bitfield::test(misbitfield, nbits, 1));
}

void BitfieldManTest::testGetAllMissingIndexes()
{
  size_t blockLength = 16*1024;
  uint64_t totalLength = 1024*1024;
  size_t nbits = (totalLength+blockLength-1)/blockLength;
  BitfieldMan bf(blockLength, totalLength);
  BitfieldMan peerBf(blockLength, totalLength);
  peerBf.setAllBit();
  unsigned char misbitfield[8];

  CPPUNIT_ASSERT(bf.getAllMissingIndexes(misbitfield, sizeof(misbitfield),
					 peerBf.getBitfield(),
					 peerBf.getBitfieldLength()));
  CPPUNIT_ASSERT_EQUAL((size_t)64, bitfield::countSetBit(misbitfield, nbits));
  for(size_t i = 0; i < 62; ++i) {
    bf.setBit(i);
  }
  peerBf.unsetBit(62);

  CPPUNIT_ASSERT(bf.getAllMissingIndexes(misbitfield, sizeof(misbitfield),
					 peerBf.getBitfield(),
					 peerBf.getBitfieldLength()));
  CPPUNIT_ASSERT_EQUAL((size_t)1, bitfield::countSetBit(misbitfield, nbits));
  CPPUNIT_ASSERT(bitfield::test(misbitfield, nbits, 63));
}

void BitfieldManTest::testGetAllMissingUnusedIndexes()
{
  size_t blockLength = 16*1024;
  uint64_t totalLength = 1024*1024;
  size_t nbits = (totalLength+blockLength-1)/blockLength;
  BitfieldMan bf(blockLength, totalLength);
  BitfieldMan peerBf(blockLength, totalLength);
  peerBf.setAllBit();
  unsigned char misbitfield[8];

  CPPUNIT_ASSERT(bf.getAllMissingUnusedIndexes(misbitfield,
					       sizeof(misbitfield),
					       peerBf.getBitfield(),
					       peerBf.getBitfieldLength()));
  CPPUNIT_ASSERT_EQUAL((size_t)64, bitfield::countSetBit(misbitfield, nbits));

  for(size_t i = 0; i < 61; ++i) {
    bf.setBit(i);
  }
  bf.setUseBit(61);
  peerBf.unsetBit(62);
  CPPUNIT_ASSERT(bf.getAllMissingUnusedIndexes(misbitfield,
					       sizeof(misbitfield),
					       peerBf.getBitfield(),
					       peerBf.getBitfieldLength()));
  CPPUNIT_ASSERT_EQUAL((size_t)1, bitfield::countSetBit(misbitfield, nbits));
  CPPUNIT_ASSERT(bitfield::test(misbitfield, nbits, 63));
}

void BitfieldManTest::testGetMissingUnusedIndex()
{
  BitfieldMan bt1(1024, 1024*256);
  bt1.setRandomizer(fixedNumberRandomizer);
  size_t index;
  {
    unsigned char bitArray[] = {
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff,
    };
    CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index, bitArray, 32));
    CPPUNIT_ASSERT_EQUAL((size_t)0, index);
  
    bt1.addFilter(1024, 1024*256);
    bt1.enableFilter();
    CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index, bitArray, 32));
    CPPUNIT_ASSERT_EQUAL((size_t)1, index);
    bt1.setUseBit(1);
    CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index, bitArray, 32));
    CPPUNIT_ASSERT_EQUAL((size_t)2, index);
    bt1.disableFilter();

    bt1.setBit(0);
    CPPUNIT_ASSERT(bt1.getMissingUnusedIndex(index, bitArray, 32));
    CPPUNIT_ASSERT_EQUAL((size_t)2, index);

    bt1.setAllBit();
    CPPUNIT_ASSERT(!bt1.getMissingUnusedIndex(index, bitArray, 32));
  
    bt1.clearAllBit();
    bt1.setAllUseBit();
    CPPUNIT_ASSERT(!bt1.getMissingUnusedIndex(index, bitArray, 32));
  }
  {
    unsigned char bitArray4[] = {
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00,
    };
    CPPUNIT_ASSERT(!bt1.getMissingUnusedIndex(index, bitArray4, 32));
  }
}

void BitfieldManTest::testCountFilteredBlock()
{
  BitfieldMan bt(1024, 1024*256);
  CPPUNIT_ASSERT_EQUAL((size_t)256, bt.countBlock());
  CPPUNIT_ASSERT_EQUAL((size_t)0, bt.countFilteredBlock());
  bt.addFilter(1024, 1024*256);
  bt.enableFilter();
  CPPUNIT_ASSERT_EQUAL((size_t)256, bt.countBlock());
  CPPUNIT_ASSERT_EQUAL((size_t)255, bt.countFilteredBlock());
  bt.disableFilter();
  CPPUNIT_ASSERT_EQUAL((size_t)256, bt.countBlock());
  CPPUNIT_ASSERT_EQUAL((size_t)0, bt.countFilteredBlock());
}

void BitfieldManTest::testCountMissingBlock()
{
  BitfieldMan bt(1024, 1024*10);
  CPPUNIT_ASSERT_EQUAL((size_t)10, bt.countMissingBlock());
  bt.setBit(1);
  CPPUNIT_ASSERT_EQUAL((size_t)9, bt.countMissingBlock());
  bt.setAllBit();
  CPPUNIT_ASSERT_EQUAL((size_t)0, bt.countMissingBlock());
}

void BitfieldManTest::testZeroLengthFilter()
{
  BitfieldMan bt(1024, 1024*10);
  bt.enableFilter();
  CPPUNIT_ASSERT_EQUAL((size_t)0, bt.countMissingBlock());
}

} // namespace aria2
