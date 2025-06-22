#ifndef BINOMIAL_HPP
#define BINOMIAL_HPP
#include <memory>

#include "Heap.hpp"

class Binomial final : public Heap {
 private:
  struct Node {
    Element value;
    Node* parent;
    std::unique_ptr<Node> child;
    std::unique_ptr<Node> sibling;
    int degree;

    explicit Node(const Element val) {
      value = val;
      parent = nullptr;
      child = nullptr;
      sibling = nullptr;
      degree = 0;
    }
  };
  std::unique_ptr<Node> head;

  static std::unique_ptr<Node> mergeTrees(std::unique_ptr<Node>& t1,
                                          std::unique_ptr<Node>& t2);
  void consolidate();
  [[nodiscard]] Node* find(const Element& value) const;
  static Node* findInTree(Node* root, const Element& value);
  static void increaseKey(Node* node, int newPriority);

 public:
  explicit Binomial() { head = nullptr; }

  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
};

#endif  // BINOMIAL_HPP
