#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

#include "Fibonacci.hpp"
#include "Utils.hpp"

class FibonacciHeapTest : public ::testing::Test {
protected:
    Fibonacci heap;
};

TEST_F(FibonacciHeapTest, SingleInsert) {
    heap.insert({1, 10});
    EXPECT_FALSE(heap.isEmpty());
    EXPECT_EQ(heap.getSize(), 1);
    EXPECT_EQ(heap.peek(), (Element{1, 10}));
}

TEST_F(FibonacciHeapTest, MultipleInsertAndExtract) {
    std::vector<Element> elements = {{1, 5}, {2, 10}, {3, 3}, {4, 8}};

    for (const auto& e : elements) {
        heap.insert(e);
    }

    EXPECT_EQ(heap.getSize(), 4);

    std::ranges::sort(elements, std::greater<Element>());

    for (const auto& e : elements) {
        EXPECT_EQ(heap.peek(), e);
        EXPECT_EQ(heap.extractMax(), e);
    }

    EXPECT_TRUE(heap.isEmpty());
}

TEST_F(FibonacciHeapTest, IncreaseKey) {
    heap.insert({1, 5});
    heap.insert({2, 10});
    heap.insert({3, 3});

    heap.increaseKey({1, 5}, 15);
    EXPECT_EQ(heap.peek(), (Element{1, 15}));

    heap.increaseKey({3, 3}, 20);
    EXPECT_EQ(heap.peek(), (Element{3, 20}));
}

TEST_F(FibonacciHeapTest, Consolidation) {
    for (int i = 1; i <= 10; ++i) {
        heap.insert({i, i});
    }

    EXPECT_EQ(heap.getSize(), 10);
    EXPECT_EQ(heap.peek(), (Element{10, 10}));

    for (int i = 10; i >= 1; --i) {
        EXPECT_EQ(heap.extractMax(), (Element{i, i}));
    }

    EXPECT_TRUE(heap.isEmpty());
}

TEST_F(FibonacciHeapTest, RandomOperations) {
    std::vector<Element> elements;
    for (int i = 0; i < 100; ++i) {
        elements.emplace_back(i, Utils::rng(0, 1000));
    }

    for (const auto& e : elements) {
        heap.insert(e);
    }

    std::ranges::sort(elements, std::greater<Element>());

    for (const auto& e : elements) {
        EXPECT_EQ(heap.extractMax(), e);
    }
}

TEST_F(FibonacciHeapTest, MeldOperation) {
    Fibonacci heap1, heap2;

    heap1.insert({1, 10});
    heap1.insert({2, 20});

    heap2.insert({3, 5});
    heap2.insert({4, 30});

    heap1.meld(heap2);

    EXPECT_EQ(heap1.getSize(), 4);
    EXPECT_EQ(heap1.peek(), (Element{4, 30}));

    EXPECT_EQ(heap1.extractMax(), (Element{4, 30}));
    EXPECT_EQ(heap1.extractMax(), (Element{2, 20}));
    EXPECT_EQ(heap1.extractMax(), (Element{1, 10}));
    EXPECT_EQ(heap1.extractMax(), (Element{3, 5}));

    EXPECT_TRUE(heap1.isEmpty());
    EXPECT_TRUE(heap2.isEmpty());
}

TEST_F(FibonacciHeapTest, CascadingCut) {
    heap.insert({1, 10});
    heap.insert({2, 20});
    heap.insert({3, 30});

    heap.increaseKey({1, 10}, 40);
    heap.increaseKey({2, 20}, 50);

    EXPECT_EQ(heap.peek(), (Element{2, 50}));
}