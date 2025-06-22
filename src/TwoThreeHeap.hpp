#ifndef TWOTHREEHEAP_HPP
#define TWOTHREEHEAP_HPP

#include <memory>
#include <vector>

#include "Heap.hpp"

class TwoThreeHeap final : public Heap {
public:
  struct Node {
    Element value;
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

    explicit Node(const Element& val) : value(val) {}
  };

private:
  std::unique_ptr<Node> root = nullptr;

  static void deleteTree(std::unique_ptr<Node>& node);
  static std::unique_ptr<Node> meldTrees(std::unique_ptr<Node>& t1, std::unique_ptr<Node>& t2);
  static Node* find(Node* node, const Element& value);

public:
  ~TwoThreeHeap() override;

  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
  [[nodiscard]] Node* getRoot() const { return root.get(); }
};

#endif  // TWOTHREEHEAP_HPP