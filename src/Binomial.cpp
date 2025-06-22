#include "Binomial.hpp"
std::unique_ptr<Binomial::Node> Binomial::mergeTrees(
    std::unique_ptr<Node>& t1, std::unique_ptr<Node>& t2) {
  if (t1->value < t2->value) {
    mergeTrees(t2, t1);
  }
  t2->parent = t1.get();
  t2->sibling = std::move(t1->child);
  t1->child = std::move(t2);
  t1->degree++;
  return std::move(t1);
}
void Binomial::consolidate() {
  std::vector<std::unique_ptr<Node>> trees(64, nullptr);
  std::unique_ptr<Node> current = std::move(head);
  head = nullptr;

  while (current != nullptr) {
    std::unique_ptr<Node> next = std::move(current->sibling);
    int degree = current->degree;

    while (trees[degree] != nullptr) {
      current = mergeTrees(current, trees[degree]);
      trees[degree] = nullptr;
      degree++;
    }
    trees[degree] = std::move(current);
    current = std::move(next);
  }
  for (int i = trees.size() - 1; i >= 0; --i) {
    if (trees[i] != nullptr) {
      trees[i]->sibling = std::move(head);
      head = std::move(trees[i]);
    }
  }
}