#include "Fibonacci.hpp"

#include <vector>
Fibonacci::Node* Fibonacci::find(const Element key) const {
  if (!max_node) return nullptr;
  return find(key, max_node.get());
}

Fibonacci::Node* Fibonacci::find(const Element key, Node* start) {
  Node* current = start;
  do {
    if (current->key == key) return current;
    if (current->child) {
      if (Node* found_in_child = find(key, current->child)) return found_in_child;
    }
    current = current->right;
  } while (current != start);
  return nullptr;
}

void Fibonacci::consolidate() {
    std::vector<Node*> degree_table;
    std::vector<std::unique_ptr<Node>> root_owners;

    if (max_node) {
        Node* current = max_node.get();
        do {
            if (!current->left) {
                current->left = std::unique_ptr<Node>(current);
                current->right = current;
            }
            auto node_ptr = std::move(current->left);
            root_owners.push_back(std::move(node_ptr));
            current = current->right;
        } while (current != max_node.get());
        max_node.reset();
    }

    for (auto& root_owner : root_owners) {
        Node* x = root_owner.get();
        int degree = x->degree;

        while (degree >= degree_table.size()) {
            degree_table.push_back(nullptr);
        }

        while (degree_table[degree]) {
            Node* y = degree_table[degree];
            if (x->key < y->key) {
                std::swap(x, y);
                std::swap(root_owner, y->left);
            }
            link(y, x);
            degree_table[degree] = nullptr;
            degree++;
            if (degree >= degree_table.size()) {
                degree_table.push_back(nullptr);
            }
        }
        degree_table[degree] = x;
        root_owner.release();
    }

    for (const auto node : degree_table) {
        if (node) {
            if (!max_node) {
                max_node.reset(node);
                node->left.reset(node);
                node->right = node;
            } else {
                if (!max_node->left) {
                    max_node->left = std::unique_ptr<Node>(max_node.get());
                    max_node->right = max_node.get();
                }
                auto new_node = std::unique_ptr<Node>(node);
                new_node->left = std::move(max_node->left);
                new_node->right = max_node.get();
                if (new_node->left) {
                    new_node->left->right = new_node.get();
                }
                max_node->left = std::move(new_node);
                if (node->key > max_node->key) {
                    max_node.swap(max_node->left);
                }
            }
        }
    }
}
void Fibonacci::link(Node* y, Node* x) {
  if (y->left) {
    y->left->right = y->right;
  }
  if (y->right) {
    y->right->left = std::move(y->left);
  }

  y->parent = x;
  if (x->child == nullptr) {
    x->child = y;
    y->left = std::unique_ptr<Node>(y);
    y->right = y;
  } else {
    Node* child = x->child;
    y->right = child->right;
    child->right = y;
    y->left = std::move(child->left);
    child->left = std::unique_ptr<Node>(y);
  }
  x->degree++;
  y->marked = false;
}

Element Fibonacci::peek() const {
  return max_node != nullptr ? max_node->key : Element{-1, -1};
}
Element Fibonacci::extractMax() {
  if (!max_node) throw std::runtime_error("Heap is empty");

  Node* z = max_node.get();
  const Element max_val = z->key;

  if (z->child) {
    Node* child = z->child;
    const Node* start = child;
    do {
      Node* next_child = child->right;
      child->parent = nullptr;
      addToRootList(child);
      child = next_child;
    } while (child != start);
    z->child = nullptr;
  }

  if (z->right == z) {
    max_node.reset();
  } else {
    Node* right = z->right;
    Node* left = z->left.get();

    right->left = std::move(z->left);
    if (right->left) {
      right->left->right = right;
    }

    max_node.release();
    max_node.reset(right);

    consolidate();
  }

  setSize(getSize() - 1);
  return max_val;
}
void Fibonacci::increaseKey(Node* node, const Element new_key) {
  if (new_key < node->key) {
    throw std::runtime_error("New key is smaller than current key");
  }

  node->key = new_key;

  if (Node* parent = node->parent; parent && node->key > parent->key) {
    cut(node, parent);
    cascadingCut(parent);
  }

  if (max_node && node->key > max_node->key) {
    max_node = getNodeOwnership(node);
  }
}

void Fibonacci::cut(Node* node, Node* parent) const {
  if (node->right == node) {
    parent->child = nullptr;
  } else {
    node->right->left = std::move(node->left);
    if (parent->child == node) {
      parent->child = node->right;
    }
  }

  parent->degree--;

  addToRootList(node);
  node->parent = nullptr;
  node->marked = false;
}

void Fibonacci::cascadingCut(Node* node) {
  if (Node* parent = node->parent) {
    if (!node->marked) {
      node->marked = true;
    } else {
      cut(node, parent);
      cascadingCut(parent);
    }
  }
}

std::unique_ptr<Fibonacci::Node> Fibonacci::getNodeOwnership(Node* node) {
  if (node->right && node->right->left.get() == node) {
    return std::move(node->right->left);
  }
  if (node->left && node->left->right == node) {
    return std::move(node->left);
  }
  throw std::runtime_error("Node ownership not found");
}


void Fibonacci::addToRootList(Node* node) const {
  node->left = std::move(max_node->left);
  node->right = max_node.get();
  max_node->left->right = node;
  max_node->left = std::unique_ptr<Node>(node);
}
void Fibonacci::increaseKey(const Element& element, int newPriority) {
  Node* node = find(element);
  if (!node) {
    throw std::runtime_error("Key not found in heap");
  }
  increaseKey(node, {node->key.getValue(), newPriority});

}
void Fibonacci::insert(Element element) {
  auto node = std::make_unique<Node>(element);
  Node* raw_node = node.get();

  raw_node->left = std::move(node);
  raw_node->right = raw_node;

  if (!max_node) {
    max_node = std::move(raw_node->left);
    setSize(getSize() + 1);
    return;
  }

  if (!max_node->left) {
    max_node->left = std::unique_ptr<Node>(max_node.get());
    max_node->right = max_node.get();
  }

  auto temp = std::move(raw_node->left);
  raw_node->left = std::move(max_node->left);
  raw_node->right = max_node.get();

  if (raw_node->left) {
    raw_node->left->right = raw_node;
  }

  max_node->left = std::move(temp);

  if (raw_node->key > max_node->key) {
    max_node.swap(max_node->left);
  }

  setSize(getSize() + 1);
}
void Fibonacci::meld(Heap& otherHeap) {
  Fibonacci other = std::move(dynamic_cast<Fibonacci&>(otherHeap));
  if (!other.max_node) {
    return;
  }

  if (!max_node) {
    max_node = std::move(other.max_node);
    setSize(other.getSize());
    return;
  }
  Node* this_left = max_node->left.get();
  Node* other_left = other.max_node->left.get();

  this_left->right = other.max_node.get();
  other.max_node->left = std::move(max_node->left);
  max_node->left = std::move(other_left->left);
  other_left->right = max_node.get();

  if (other.max_node->key > max_node->key) {
    auto new_max = other.max_node.release();
    max_node.reset(new_max);
  }
  setSize(getSize() + other.getSize());
  other.max_node.reset();
  other.setSize(0);
}
void Fibonacci::print() const {}
Element Fibonacci::getRandomElement() const {}
