#pragma once
#include <memory>
#include "Heap.hpp"

class Fibonacci final : public Heap {
private:
  struct Node {
    Element key;
    Node* parent;
    Node* child;
    Node* left;
    Node* right;
    int degree;
    bool marked;
    explicit Node(const Element& k)
        : key(k), parent(nullptr), child(nullptr), left(nullptr), right(nullptr), degree(0), marked(false) {}
  };
  std::unique_ptr<Node> max_node;

  Node* find(const Element key) const;
  static Node* find(const Element key, Node* start);
  void consolidate();
  static void link(Node* y, Node* x);
  void increaseKey(Node* node, const Element new_key);
  void cut(Node* node, Node* parent);
  void cascadingCut(Node* node);
  void addToRootList(Node* node);

public:
  Fibonacci() : max_node(nullptr) {};
  [[nodiscard]] Element peek() const override;
  Element extractMax() override;
  void increaseKey(const Element& element, int newPriority) override;
  void insert(Element element) override;
  void meld(Heap& otherHeap) override;
  void print() const override;
  [[nodiscard]] Element getRandomElement() const override;
};