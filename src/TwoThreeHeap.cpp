#include "TwoThreeHeap.hpp"

#include <algorithm>

std::unique_ptr<TwoThreeHeap::Node> TwoThreeHeap::meldTrees(
    std::unique_ptr<Node>& t1, std::unique_ptr<Node>& t2) {
  if (!t1) return std::move(t2);
  if (!t2) return std::move(t1);

  if (t1->value < t2->value) {
    std::swap(t1, t2);
  }

  t2->parent = t1.get();
  t1->children.push_back(std::move(t2));
  enforceTwoThreeRule(t1);

  return std::move(t1);
}

void TwoThreeHeap::enforceTwoThreeRule(const std::unique_ptr<Node>& node) {
  if (!node || node->children.size() <= 3) return;

  auto newNode = std::make_unique<Node>(node->children[2]->value);
  newNode->children.push_back(std::move(node->children[2]));
  newNode->children.push_back(std::move(node->children[3]));
  node->children.resize(2);

  for (const auto& child : newNode->children) {
    child->parent = newNode.get();
  }

  node->children.push_back(std::move(newNode));
}

Element TwoThreeHeap::peek() const {
  if (!root) return {-1, -1};
  return root->value;
}

Element TwoThreeHeap::extractMax() {
  if (!root) return {-1, -1};

  const Element maxValue = root->value;
  std::vector<std::unique_ptr<Node>> children = std::move(root->children);

  std::unique_ptr<Node> newRoot = nullptr;
  for (auto& child : children) {
    if (child) {
      child->parent = nullptr;
      newRoot = meldTrees(newRoot, child);
    }
  }

  root = std::move(newRoot);
  setSize(getSize() - 1);
  return maxValue;
}

void TwoThreeHeap::increaseKey(const Element& element, int newPriority) {
  Node* node = find(root.get(), element);
  if (!node) return;

  node->value.setPriority(newPriority);

  if (!node->parent || node->value <= node->parent->value) return;

  auto& siblings = node->parent->children;
  const auto it = std::ranges::find_if(siblings,
                                 [node](const std::unique_ptr<Node>& child) {
                                   return child.get() == node;
                                 });

  if (it != siblings.end()) {
    auto detachedNode = std::move(*it);
    siblings.erase(it);

    for (const auto& child : detachedNode->children) {
      child->parent = nullptr;
    }

    detachedNode->parent = nullptr;
    root = meldTrees(root, detachedNode);
  }
}

void TwoThreeHeap::insert(Element element) {
  auto newNode = std::make_unique<Node>(element);
  root = meldTrees(root, newNode);
  enforceTwoThreeRule(root);
  setSize(getSize() + 1);
}

void TwoThreeHeap::meld(Heap& otherHeap) {
  auto& other = dynamic_cast<TwoThreeHeap&>(otherHeap);
  root = meldTrees(root, other.root);
  setSize(getSize() + other.getSize());
  other.root.reset();
  other.setSize(0);
}

void TwoThreeHeap::print() const {}

Element TwoThreeHeap::getRandomElement() const {
  return root ? root->value : Element{-1, -1};
}

TwoThreeHeap::Node* TwoThreeHeap::find(Node* node, const Element& value) {
  if (!node) return nullptr;
  if (node->value == value) return node;
  for (const auto& child : node->children) {
    if (Node* found = find(child.get(), value)) return found;
  }
  return nullptr;
}