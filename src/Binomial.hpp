#ifndef BINOMIAL_HPP
#define BINOMIAL_HPP
#include <memory>

#include "Heap.hpp"

class Binomial final : public Heap {
 private:
  struct Node {
    Element value;
    Node* parent = nullptr;
    std::unique_ptr<Node> child = nullptr;
    std::unique_ptr<Node> sibling = nullptr;
    int degree = 0;

    explicit Node(const Element val) : value(val) {}
  };
  std::unique_ptr<Node> head = nullptr;

  static std::unique_ptr<Node> mergeTrees(std::unique_ptr<Node>& t1,
                                          std::unique_ptr<Node>& t2);
  void consolidate();
  [[nodiscard]] Node* find(const Element& value) const;
  static Node* findInTree(Node* root, const Element& value);
  static void increaseKey(Node* node, int newPriority);

 public:

  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
};

#endif  // BINOMIAL_HPP
