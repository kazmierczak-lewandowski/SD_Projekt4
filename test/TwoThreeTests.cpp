#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "TwoThreeHeap.hpp"



class TwoThreeHeapTest : public ::testing::Test {
protected:
  TwoThreeHeap heap;
};

TEST_F(TwoThreeHeapTest, InsertAndPeek) {
  heap.insert({1, 10});
  heap.insert({2, 20});
  heap.insert({3, 15});

  EXPECT_EQ(heap.peek().getValue(), 2);
  EXPECT_EQ(heap.peek().getPriority(), 20);
}

TEST_F(TwoThreeHeapTest, ExtractMax) {
  heap.insert({1, 10});
  heap.insert({2, 20});
  heap.insert({3, 15});

  Element max = heap.extractMax();
  EXPECT_EQ(max.getValue(), 2);
  EXPECT_EQ(max.getPriority(), 20);

  EXPECT_EQ(heap.peek().getValue(), 3);
  EXPECT_EQ(heap.peek().getPriority(), 15);
}

TEST_F(TwoThreeHeapTest, IncreaseKey) {
  heap.insert({1, 10});
  heap.insert({2, 20});
  heap.insert({3, 15});

  heap.increaseKey({1, 10}, 25);

  EXPECT_EQ(heap.peek().getValue(), 1);
  EXPECT_EQ(heap.peek().getPriority(), 25);
}

TEST_F(TwoThreeHeapTest, Meld) {
  TwoThreeHeap otherHeap;
  heap.insert({1, 10});
  heap.insert({2, 20});

  otherHeap.insert({3, 15});
  otherHeap.insert({4, 25});

  heap.meld(otherHeap);

  EXPECT_EQ(heap.peek().getValue(), 4);
  EXPECT_EQ(heap.peek().getPriority(), 25);
}

TEST_F(TwoThreeHeapTest, GetRandomElement) {
  heap.insert({1, 10});
  heap.insert({2, 20});

  Element randomElement = heap.getRandomElement();
  EXPECT_TRUE(randomElement.getValue() == 1 || randomElement.getValue() == 2);
}
TEST_F(TwoThreeHeapTest, EnforceTwoThreeRule) {
  heap.insert({1, 10});
  heap.insert({2, 20});
  heap.insert({3, 15});
  heap.insert({4, 25});
  heap.insert({5, 30});

  auto root = heap.getRoot();
  ASSERT_NE(root, nullptr);
  EXPECT_EQ(root->children.size(), 3);

  auto newNode = root->children.back().get();
  ASSERT_NE(newNode, nullptr);
  EXPECT_EQ(newNode->children.size(), 2);
}