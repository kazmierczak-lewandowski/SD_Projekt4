#include <gtest/gtest.h>

#include <queue>
#include <string>

#include "../src/Pairing.hpp"

void add3ElementsForTests(Pairing &heap) {
  heap.insert(Element{1, 10});
  heap.insert(Element{2, 20});
  heap.insert(Element{3, 5});
}

TEST(PairingTests, Insert) {
  Pairing heap;
  add3ElementsForTests(heap);
  EXPECT_EQ(heap.peek().getPriority(), 20);
  EXPECT_EQ(heap.getSize(), 3);
}

TEST(PairingTests, ExtractMax) {
  Pairing heap;
  add3ElementsForTests(heap);

  Element max = heap.extractMax();
  EXPECT_EQ(max.getPriority(), 20);
  EXPECT_EQ(heap.getSize(), 2);

  max = heap.extractMax();
  EXPECT_EQ(max.getPriority(), 10);
  EXPECT_EQ(heap.getSize(), 1);
}

TEST(PairingTests, IncreaseKey) {
  Pairing heap;
  add3ElementsForTests(heap);

  heap.increaseKey(Element{1, 10}, 25);
  EXPECT_EQ(heap.peek().getPriority(), 25);
}

TEST(PairingTests, Meld) {
  Pairing heap1;
  heap1.insert(Element{1, 10});
  heap1.insert(Element{2, 20});

  Pairing heap2;
  heap2.insert(Element{3, 5});
  heap2.insert(Element{4, 15});
  heap2.insert(Element{5, 25});

  heap1.meld(heap2);

  EXPECT_EQ(heap1.getSize(), 5);
  EXPECT_EQ(heap1.peek().getPriority(), 25);
}
