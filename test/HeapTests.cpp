#include <gtest/gtest.h>

#include <vector>

#include "../src/Binary.hpp"

bool isHeapValid(const Element *elements, const int size) {
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

TEST(HeapTests, Insert) {
  Binary heap;
  add3ElementsForTests(heap);

  EXPECT_EQ(heap.peek().getPriority(), 20);
  EXPECT_TRUE(isHeapValid(heap.getElements(), heap.getSize()));
}

TEST(HeapTests, ExtractMax) {
  Binary heap;
  add3ElementsForTests(heap);

  const Element max = heap.extractMax();
  EXPECT_EQ(max.getPriority(), 20);
  EXPECT_TRUE(isHeapValid(heap.getElements(), heap.getSize()));
}
TEST(HeapTests, IncreaseKey) {
  Binary heap;
  add3ElementsForTests(heap);

  heap.increaseKey(Element{1, 10}, 25);
  EXPECT_EQ(heap.peek().getPriority(), 25);
  EXPECT_TRUE(isHeapValid(heap.getElements(), heap.getSize()));
}
TEST(HeapTests, FindElement) {
  Binary heap;
  add3ElementsForTests(heap);

  const int index = heap.findElement(Element{3, 5});
  EXPECT_NE(index, -1);
  EXPECT_EQ(index, 2);
}

TEST(HeapTests, GrowTest) {
  const std::vector<Element> elements = {{1, 10}, {2, 20}, {3, 5}};
  Binary heap;
  for (const auto &element : elements) {
    heap.insert(element);
  }

  heap.insert(Element{4, 15});
  EXPECT_EQ(heap.getCapacity(), 6);
}

TEST(HeapTests, GetLevels) {
  const std::vector<Element> elements = {{1, 10}, {2, 20}, {3, 5}, {4, 15}, {5, 25}};

  Binary heap;
  for (const auto &element : elements) {
    heap.insert(element);
  }

  const std::vector<std::vector<Element>> levels = heap.getLevels();

  ASSERT_EQ(levels.size(), 3);

  EXPECT_EQ(levels[0].size(), 1);
  EXPECT_EQ(levels[0][0], elements[4]);

  EXPECT_EQ(levels[1].size(), 2);
  EXPECT_EQ(levels[1][0], elements[1]);
  EXPECT_EQ(levels[1][1], elements[2]);

  EXPECT_EQ(levels[2].size(), 4);
  EXPECT_EQ(levels[2][0], elements[3]);
  EXPECT_EQ(levels[2][1], elements[0]);
}