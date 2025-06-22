#include "Binomial.hpp"

#include <vector>
std::unique_ptr<Binomial::Node> Binomial::mergeTrees(
    std::unique_ptr<Node>& t1, std::unique_ptr<Node>& t2) {
  if (t1->value < t2->value) {
    return mergeTrees(t2, t1);
  }
  t2->parent = t1.get();
  t2->sibling = std::move(t1->child);
  t1->child = std::move(t2);
  t1->degree++;
  return std::move(t1);
}
void Binomial::consolidate() {
  constexpr int MAX_DEGREE = 64;
  Node* trees[MAX_DEGREE] = {nullptr};
  std::unique_ptr<Node> current = std::move(head);
  head = nullptr;

  while (current) {
    std::unique_ptr<Node> next = std::move(current->sibling);
    int degree = current->degree;

    Node* current_raw = current.release();

    while (trees[degree] != nullptr) {
      std::unique_ptr<Node> existing_tree(trees[degree]);
      std::unique_ptr<Node> new_tree(current_raw);
      auto merged = mergeTrees(new_tree, existing_tree);

      trees[degree] = nullptr;
      current_raw = merged.release();
      degree++;
    }

    trees[degree] = current_raw;
    current = std::move(next);
  }

  for (int i = MAX_DEGREE - 1; i >= 0; --i) {
    if (trees[i]) {
      std::unique_ptr<Node> tree(trees[i]);
      tree->sibling = std::move(head);
      head = std::move(tree);
    }
  }
}
Element Binomial::peek() const {
  const Node* current = head.get();
  Element max;
  while (current != nullptr) {
    if (max < current->value) {
      max = current->value;
    }
    current = current->sibling.get();
  }
  return max;
}
Element Binomial::extractMax() {
  if (!head) return {-1, -1};

  std::unique_ptr<Node>* maxPtr = &head;
  const std::unique_ptr<Node>* current = &head;

  while ((*current)->sibling) {
    if ((*current)->sibling->value > (*maxPtr)->value) {
      maxPtr = &(*current)->sibling;
    }
    current = &(*current)->sibling;
  }

  const Element maxValue = (*maxPtr)->value;
  const std::unique_ptr<Node> maxTree = std::move(*maxPtr);
  *maxPtr = std::move(maxTree->sibling);

  Binomial childrenHeap;
  childrenHeap.head = std::move(maxTree->child);

  std::unique_ptr<Node> reversedChildren = nullptr;
  while (childrenHeap.head) {
    auto next = std::move(childrenHeap.head->sibling);
    childrenHeap.head->sibling = std::move(reversedChildren);
    reversedChildren = std::move(childrenHeap.head);
    childrenHeap.head = std::move(next);
  }
  childrenHeap.head = std::move(reversedChildren);
  setSize(getSize() - 1);
  meld(childrenHeap);

  return maxValue;
}
void Binomial::increaseKey(const Element& element, const int newPriority) {
  auto* node = find(element);
  increaseKey(node, newPriority);
}
void Binomial::insert(const Element element) {
  Binomial newHeap;
  newHeap.head = std::make_unique<Node>(element);
  meld(newHeap);
  setSize(getSize() + 1);
}
void Binomial::meld(Heap& otherHeap) {
  const int otherSize = otherHeap.getSize();
  auto& other = dynamic_cast<Binomial&>(otherHeap);
  std::unique_ptr<Node> newHead = nullptr;
  std::unique_ptr<Node>* current = &newHead;
  std::unique_ptr<Node> h1 = std::move(head);
  std::unique_ptr<Node> h2 = std::move(other.head);

  while (h1 != nullptr && h2 != nullptr) {
    if (h1->degree <= h2->degree) {
      std::unique_ptr<Node> h1_next = std::move(h1->sibling);
      *current = std::move(h1);
      h1 = std::move(h1_next);
    } else {
      std::unique_ptr<Node> h2_next = std::move(h2->sibling);
      *current = std::move(h2);
      h2 = std::move(h2_next);
    }
    current = &((*current)->sibling);
  }
  if (h1 != nullptr) *current = std::move(h1);
  if (h2 != nullptr) *current = std::move(h2);

  head = std::move(newHead);
  consolidate();
  setSize(getSize() + otherSize);
}
void Binomial::print() const {}
Element Binomial::getRandomElement() const { return {-1, -1}; }

Binomial::Node* Binomial::find(const Element& value) const {
  return findInTree(head.get(), value);
}

Binomial::Node* Binomial::findInTree(Node* root, const Element& value) {
  if (!root) return nullptr;
  if (root->value == value) return root;
  if (Node* found = findInTree(root->sibling.get(), value)) return found;
  return findInTree(root->child.get(), value);
}

void Binomial::increaseKey(Node* node, const int newPriority) {
  node->value.setPriority(newPriority);
  while (node->parent && node->value > node->parent->value) {
    std::swap(node->value, node->parent->value);
    node = node->parent;
  }
}