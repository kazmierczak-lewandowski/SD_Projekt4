#include "Pairing.hpp"

#include <stack>
#include <vector>
void Pairing::deleteTree(std::unique_ptr<Node>& node) {
  if (!node) return;

  std::stack<std::unique_ptr<Node>> stack;
  stack.push(std::move(node));

  while (!stack.empty()) {
    auto current = std::move(stack.top());
    stack.pop();

    if (current->child) {
      stack.push(std::move(current->child));
    }
    if (current->sibling) {
      stack.push(std::move(current->sibling));
    }
  }

  node.reset();
}
std::unique_ptr<Pairing::Node> Pairing::meldTrees(std::unique_ptr<Node>& t1,
                                                  std::unique_ptr<Node>& t2) {
  if (!t1) return std::move(t2);
  if (!t2) return std::move(t1);

  if (t1->value < t2->value) {
    t2->sibling = std::move(t1->child);
    t1->child = std::move(t2);
    t1->child->parent = t1.get();
    return std::move(t1);
  } else {
    t1->sibling = std::move(t2->child);
    t2->child = std::move(t1);
    t2->child->parent = t2.get();
    return std::move(t2);
  }
}

[[nodiscard]] Element Pairing::peek() const {
  if (!root) return {-1, -1};
  return root->value;
}

Element Pairing::extractMax() {
  if (!root) return {-1, -1};

  Element maxValue = root->value;

  std::vector<std::unique_ptr<Node>> children;
  while (root->child) {
    children.push_back(std::move(root->child));
    root->child = std::move(children.back()->sibling);
  }

  while (children.size() > 1) {
    auto t1 = std::move(children.back());
    children.pop_back();
    auto t2 = std::move(children.back());
    children.pop_back();
    children.push_back(meldTrees(t1, t2));
  }
  root = children.empty() ? nullptr : std::move(children.back());
  setSize(getSize() - 1);

  return maxValue;
}

void Pairing::increaseKey(const Element& element, const int newPriority) {
  Node* node = find(root.get(), element);
  if (!node) return;

  node->value.setPriority(newPriority);

  if (!node->parent || node->value <= node->parent->value) return;

  if (node->parent->child.get() == node) {
    node->parent->child = std::move(node->sibling);
  } else {
    Node* sibling = node->parent->child.get();
    while (sibling && sibling->sibling.get() != node) {
      sibling = sibling->sibling.get();
    }
    if (sibling && node->sibling) {
      sibling->sibling = std::move(node->sibling);
    }
  }

  node->parent = nullptr;
  node->sibling = nullptr;

  auto detachedNode = std::make_unique<Node>(node->value);
  root = meldTrees(root, detachedNode);
}

void Pairing::insert(Element element) {
  auto newNode = std::make_unique<Node>(element);
  root = meldTrees(root, newNode);
  setSize(getSize() + 1);
}

void Pairing::meld(Heap& otherHeap) {
  auto& other = dynamic_cast<Pairing&>(otherHeap);
  root = meldTrees(root, other.root);
  setSize(getSize() + other.getSize());
}

void Pairing::print() const {}

[[nodiscard]] Element Pairing::getRandomElement() const {
  return root ? root->value : Element{-1, -1};
}

Pairing::Node* Pairing::find(Node* node, const Element& value) {
  if (!node) return nullptr;
  if (node->value == value) return node;
  if (Node* found = find(node->child.get(), value)) return found;
  return find(node->sibling.get(), value);
}