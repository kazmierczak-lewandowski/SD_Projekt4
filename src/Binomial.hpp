#ifndef BINOMIAL_HPP
#define BINOMIAL_HPP
#include "Heap.hpp"

class Binomial : public Heap {
 public:
  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
};



#endif //BINOMIAL_HPP
