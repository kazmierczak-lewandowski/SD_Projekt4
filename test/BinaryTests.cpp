#include <gtest/gtest.h>

#include <vector>

#include "../src/Binary.hpp"

bool isBinaryValid(const Element *elements, const int size) {
  for (int i = 0; i < size; ++i) {
    const int left = 2 * i + 1;
    const int right = 2 * i + 2;
    if (left < size && elements[i] < elements[left]) {
      return false;
    }
    if (right < size && elements[i] < elements[right]) {
      return false;
    }
  }
  return true;
}
void add3ElementsForTests(Binary &heap) {
  heap.insert(Element{1, 10});
  heap.insert(Element{2, 20});
  heap.insert(Element{3, 5});
}

TEST(BinaryTests, Insert) {
  Binary heap;
  add3ElementsForTests(heap);

  EXPECT_EQ(heap.peek().getPriority(), 20);
  EXPECT_TRUE(isBinaryValid(heap.getElements(), heap.getSize()));
}

TEST(BinaryTests, ExtractMax) {
  Binary heap;
  add3ElementsForTests(heap);

  const Element max = heap.extractMax();
  EXPECT_EQ(max.getPriority(), 20);
  EXPECT_TRUE(isBinaryValid(heap.getElements(), heap.getSize()));
}
TEST(BinaryTests, IncreaseKey) {
  Binary heap;
  add3ElementsForTests(heap);

  heap.increaseKey(Element{1, 10}, 25);
  EXPECT_EQ(heap.peek().getPriority(), 25);
  EXPECT_TRUE(isBinaryValid(heap.getElements(), heap.getSize()));
}
TEST(BinaryTests, FindElement) {
  Binary heap;
  add3ElementsForTests(heap);

  const int index = heap.findElement(Element{3, 5});
  EXPECT_NE(index, -1);
  EXPECT_EQ(index, 2);
}

TEST(BinaryTests, GrowTest) {
  const std::vector<Element> elements = {{1, 10}, {2, 20}, {3, 5}};
  Binary heap(3);
  for (const auto &element : elements) {
    heap.insert(element);
  }

  heap.insert(Element{4, 15});
  EXPECT_EQ(heap.getCapacity(), 6);
}
TEST(BinaryTests, Meld) {
  Binary heap1;
  heap1.insert(Element{1, 10});
  heap1.insert(Element{2, 20});

  Binary heap2;
  heap2.insert(Element{3, 5});
  heap2.insert(Element{4, 15});
  heap2.insert(Element{5, 25});

  heap1.meld(heap2);

  EXPECT_EQ(heap1.getSize(), 5);
  isBinaryValid(heap1.getElements(), heap1.getSize());
  EXPECT_EQ(heap1.peek().getPriority(), 25);
}