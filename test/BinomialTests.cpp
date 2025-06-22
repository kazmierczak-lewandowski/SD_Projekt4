#include <gtest/gtest.h>

#include "Binomial.hpp"

class BinomialTest : public ::testing::Test {
 protected:
  void SetUp() override {
    elem1 = {1, 10};
    elem2 = {2, 20};
    elem3 = {3, 30};
    elem4 = {4, 5};
  }

  Binomial heap;
  Element elem1;
  Element elem2;
  Element elem3;
  Element elem4;
};

TEST_F(BinomialTest, EmptyHeap) {
  EXPECT_TRUE(heap.peek().getPriority() == -1);
  EXPECT_TRUE(heap.extractMax().getPriority() == -1);
}

TEST_F(BinomialTest, SingleInsert) {
  heap.insert(elem1);
  EXPECT_EQ(heap.peek().getPriority(), 10);
  EXPECT_EQ(heap.peek().getValue(), 1);
}

TEST_F(BinomialTest, InsertAndExtractMax) {
  heap.insert(elem1);
  heap.insert(elem2);
  heap.insert(elem3);

  EXPECT_EQ(heap.extractMax().getPriority(), 30);
  EXPECT_EQ(heap.extractMax().getPriority(), 20);
  EXPECT_EQ(heap.extractMax().getPriority(), 10);
  EXPECT_TRUE(heap.extractMax().getPriority() == -1);
}

TEST_F(BinomialTest, Consolidate) {
  heap.insert(elem1);
  heap.insert(elem2);
  heap.insert(elem3);

  Binomial heap2;
  heap2.insert(elem4);
  heap.meld(heap2);

  EXPECT_EQ(heap.peek().getPriority(), 30);
  EXPECT_EQ(heap.extractMax().getPriority(), 30);
  EXPECT_EQ(heap.extractMax().getPriority(), 20);
}

TEST_F(BinomialTest, IncreaseKey) {
  heap.insert(elem1);
  heap.insert(elem2);

  heap.increaseKey(elem1, 25);
  EXPECT_EQ(heap.extractMax().getPriority(), 25);
  EXPECT_EQ(heap.extractMax().getPriority(), 20);
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
    heap.insert({i, i});
  }

  for (int i = 100; i >= 1; i--) {
    EXPECT_EQ(heap.extractMax().getPriority(), i);
  }
}

TEST_F(BinomialTest, IncreaseKeyChangesMax) {
  heap.insert({1, 10});
  heap.insert({2, 20});
  heap.increaseKey({1, 10}, 30);

  EXPECT_EQ(heap.extractMax().getPriority(), 30);
  EXPECT_EQ(heap.extractMax().getPriority(), 20);
}