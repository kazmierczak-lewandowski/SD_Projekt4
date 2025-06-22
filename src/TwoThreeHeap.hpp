#ifndef TWO_THREE_HEAP_HPP
#define TWO_THREE_HEAP_HPP

#include "Heap.hpp"
#include <memory>
#include <vector>

class TwoThreeHeap final : public Heap {
private:
  struct Node {
    Element value;
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;

    explicit Node(const Element val) : value(val) {}
  };

  std::unique_ptr<Node> root;

  static void deleteTree(std::unique_ptr<Node>& node);
  static std::unique_ptr<Node> meldTrees(std::unique_ptr<Node>& t1,
                                  std::unique_ptr<Node>& t2);
  static void enforceTwoThreeRule(const std::unique_ptr<Node>& node);
  static Node* find(Node* node, const Element& value);
public:
  TwoThreeHeap() = default;
  ~TwoThreeHeap() override { deleteTree(root); }

  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
  [[nodiscard]] Node* getRoot() const { return root.get(); }
};

#endif