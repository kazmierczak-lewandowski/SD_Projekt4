#include "../src/Element.hpp"
#include <gtest/gtest.h>

TEST(ElementTests, ParameterizedConstructor) {
  const Element element(42, 10);
  EXPECT_EQ(element.getValue(), 42);
  EXPECT_EQ(element.getPriority(), 10);
}

TEST(ElementTests, ComparisonOperators) {
  const Element element1(10, 5);
  const Element element2(20, 10);
  const Element element3(30, 10);
  EXPECT_TRUE(element1 < element2);
  EXPECT_FALSE(element2 < element1);
  EXPECT_TRUE(element2 <= element3);
  EXPECT_TRUE(element2 >= element3);
}

TEST(ElementTests, EqualityOperator) {
  const Element element1(10, 5);
  const Element element2(10, 5);
  const Element element3(20, 5);
  EXPECT_TRUE(element1 == element2);
  EXPECT_FALSE(element1 == element3);
  EXPECT_TRUE(element1 != element3);
}