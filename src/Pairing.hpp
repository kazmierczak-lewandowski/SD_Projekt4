#ifndef PAIRING_HPP
#define PAIRING_HPP

#include <memory>
#include "Heap.hpp"

class Pairing final : public Heap {
public:
  struct Node {
    Element value;
    Node* parent = nullptr;
    std::unique_ptr<Node> child = nullptr;
    std::unique_ptr<Node> sibling = nullptr;

    explicit Node(const Element& val) : value(val) {}
  };
private:


  std::unique_ptr<Node> root = nullptr;

 static void deleteTree(std::unique_ptr<Node>& node);
  static std::unique_ptr<Node> meldTrees(std::unique_ptr<Node>& t1,
                                         std::unique_ptr<Node>& t2);
  static Node* find(Node* node, const Element& value);

public:
  ~Pairing() override;

  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
  Node* getRoot() const { return root.get(); }
};

#endif  // PAIRING_HPP