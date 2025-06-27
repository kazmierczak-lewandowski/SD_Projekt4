#pragma once
#include <memory>
#include "Heap.hpp"

class Fibonacci final : public Heap {
 private:
  struct Node {
    Element key;
    Node* parent = nullptr;
    Node* child = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;
    int degree = 0;
    char mark = 'W';
    char c = 'N';
    explicit Node(const Element& k)
        : key(k) {}
  };
  Node* maxNode = nullptr;
  void consolidate();
  void cut(Node* found, Node* temp) const;
  void cascadeCut(Node* temp);
  void increaseKey(Node* found, int val);
  void fibonnaciLink(Node* ptr2, Node* ptr1);

 public:
  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
};
