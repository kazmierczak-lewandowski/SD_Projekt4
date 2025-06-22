#include <gtest/gtest.h>

#include "Binomial.hpp"

class BinomialTest : public ::testing::Test {
private:
  Binomial heap;
 protected:
  void SetUp() override {
    elem1 = {1, 10};
    elem2 = {2, 20};
    elem3 = {3, 30};
    elem4 = {4, 5};
  }

  Binomial* getHeap() {;
    return &heap;
  }
  Element elem1;
  Element elem2;
  Element elem3;
  Element elem4;
};

TEST_F(BinomialTest, EmptyHeap) {
  EXPECT_TRUE(getHeap()->peek().getPriority() == -1);
  EXPECT_TRUE(getHeap()->extractMax().getPriority() == -1);
}

TEST_F(BinomialTest, SingleInsert) {
  getHeap()->insert(elem1);
  EXPECT_EQ(getHeap()->peek().getPriority(), 10);
  EXPECT_EQ(getHeap()->peek().getValue(), 1);
}

TEST_F(BinomialTest, InsertAndExtractMax) {
  getHeap()->insert(elem1);
  getHeap()->insert(elem2);
  getHeap()->insert(elem3);

  EXPECT_EQ(getHeap()->extractMax().getPriority(), 30);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 20);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 10);
  EXPECT_TRUE(getHeap()->extractMax().getPriority() == -1);
}

TEST_F(BinomialTest, Consolidate) {
  getHeap()->insert(elem1);
  getHeap()->insert(elem2);
  getHeap()->insert(elem3);

  Binomial heap2;
  heap2.insert(elem4);
  getHeap()->meld(heap2);

  EXPECT_EQ(getHeap()->peek().getPriority(), 30);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 30);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 20);
}

TEST_F(BinomialTest, IncreaseKey) {
  getHeap()->insert(elem1);
  getHeap()->insert(elem2);

  getHeap()->increaseKey(elem1, 25);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 25);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 20);
}

TEST_F(BinomialTest, MeldingHeaps) {
  Binomial heap1;
  heap1.insert(elem1);
  heap1.insert(elem3);

  Binomial heap2;
  heap2.insert(elem2);
  heap2.insert(elem4);

  heap1.meld(heap2);

  EXPECT_EQ(heap1.extractMax().getPriority(), 30);
  EXPECT_EQ(heap1.extractMax().getPriority(), 20);
  EXPECT_EQ(heap1.extractMax().getPriority(), 10);
  EXPECT_EQ(heap1.extractMax().getPriority(), 5);
  EXPECT_TRUE(heap2.peek().getPriority() == -1);
}

TEST_F(BinomialTest, LargeRandomOperations) {
  for (int i = 1; i <= 100; i++) {
    getHeap()->insert({i, i});
  }

  for (int i = 100; i >= 1; i--) {
    EXPECT_EQ(getHeap()->extractMax().getPriority(), i);
  }
}

TEST_F(BinomialTest, IncreaseKeyChangesMax) {
  getHeap()->insert({1, 10});
  getHeap()->insert({2, 20});
  getHeap()->increaseKey({1, 10}, 30);

  EXPECT_EQ(getHeap()->extractMax().getPriority(), 30);
  EXPECT_EQ(getHeap()->extractMax().getPriority(), 20);
}