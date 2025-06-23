#include "Fibonacci.hpp"
#include <vector>
#include <stdexcept>

Fibonacci::Node* Fibonacci::find(const Element key) const {
    if (!max_node) return nullptr;
    return find(key, max_node.get());
}

Fibonacci::Node* Fibonacci::find(const Element key, Node* start) {
    Node* current = start;
    if (!current) return nullptr;
    do {
        if (current->key == key) return current;
        if (current->child) {
            if (Node* found = find(key, current->child)) return found;
        }
        current = current->right;
    } while (current && current != start);
    return nullptr;
}

void Fibonacci::consolidate() {
    if (!max_node) return;
    std::vector<Node*> degree_table(64, nullptr);
    std::vector<Node*> root_list;

    Node* curr = max_node.get();
    if (!curr) return;
    do {
        root_list.push_back(curr);
        curr = curr->right;
    } while (curr != max_node.get());

    max_node->left->right = nullptr;
    max_node->left = nullptr;

    for (Node* w : root_list) {
        Node* x = w;
        int d = x->degree;
        while (degree_table[d]) {
            Node* y = degree_table[d];
            if (x->key < y->key) std::swap(x, y);
            link(y, x);
            degree_table[d] = nullptr;
            ++d;
        }
        degree_table[d] = x;
    }

    max_node.reset();
    for (Node* n : degree_table) {
        if (n) {
            if (!max_node) {
                max_node.reset(n);
                n->left = n->right = n;
            } else {
                n->left = max_node->left;
                n->right = max_node.get();
                max_node->left->right = n;
                max_node->left = n;
                if (n->key > max_node->key) {
                    max_node.release();
                    max_node.reset(n);
                }
            }
        }
    }
}

void Fibonacci::link(Node* y, Node* x) {
    y->left->right = y->right;
    y->right->left = y->left;
    y->parent = x;
    if (!x->child) {
        x->child = y;
        y->left = y->right = y;
    } else {
        y->left = x->child->left;
        y->right = x->child;
        x->child->left->right = y;
        x->child->left = y;
    }
    x->degree++;
    y->marked = false;
}

Element Fibonacci::peek() const {
    return max_node ? max_node->key : Element{-1, -1};
}

Element Fibonacci::extractMax() {
  if (!max_node) throw std::runtime_error("Heap is empty");
  Node* z = max_node.get();
  const Element max_val = z->key;

  if (z->child) {
    Node* child = z->child;
    const Node* start = child;
    do {
      Node* next = child->right;
      child->parent = nullptr;
      addToRootList(child);
      child = next;
    } while (child != start);
    z->child = nullptr;
  }

  if (z->right == z) {
    max_node.reset();
  } else {
    Node* right = z->right;
    Node* left = z->left;
    right->left = left;
    left->right = right;
    max_node.release();
    max_node.reset(right);
    consolidate();
  }

  setSize(getSize() - 1);
  return max_val;
}

void Fibonacci::increaseKey(Node* node, const Element new_key) {
    node->key = new_key;
    if (Node* parent = node->parent; parent && node->key > parent->key) {
        cut(node, parent);
        cascadingCut(parent);
    }
    if (max_node && node->key > max_node->key) {
        max_node.release();
        max_node.reset(node);
    }
}

void Fibonacci::cut(Node* node, Node* parent) {
    if (node->right == node) {
        parent->child = nullptr;
    } else {
        node->right->left = node->left;
        node->left->right = node->right;
        if (parent->child == node) parent->child = node->right;
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

void Fibonacci::addToRootList(Node* node) {
    if (!max_node) {
        node->left = node->right = node;
        max_node.reset(node);
    } else {
        node->left = max_node->left;
        node->right = max_node.get();
        max_node->left->right = node;
        max_node->left = node;
    }
}

void Fibonacci::increaseKey(const Element& element, int newPriority) {
    Node* node = find(element);
    if (!node) throw std::runtime_error("Key not found in heap");
    increaseKey(node, {node->key.getValue(), newPriority});
}

void Fibonacci::insert(Element element) {
  auto node = std::make_unique<Node>(element);
  Node* raw_node = node.get();
  raw_node->left = raw_node->right = raw_node;
  if (!max_node) {
    max_node = std::move(node);
  } else {
    // Ensure max_node->left is valid
    Node* left = max_node->left ? max_node->left : max_node.get();
    raw_node->left = left;
    raw_node->right = max_node.get();
    left->right = raw_node;
    max_node->left = raw_node;
    if (raw_node->key > max_node->key) {
      max_node.release();
      max_node.reset(raw_node);
    }
  }
  setSize(getSize() + 1);
}

void Fibonacci::meld(Heap& otherHeap) {
    Fibonacci& other = dynamic_cast<Fibonacci&>(otherHeap);
    if (!other.max_node) return;
    if (!max_node) {
        max_node = std::move(other.max_node);
        setSize(other.getSize());
        other.setSize(0);
        return;
    }
    Node* this_left = max_node->left;
    Node* other_left = other.max_node->left;
    this_left->right = other.max_node.get();
    other.max_node->left = this_left;
    other_left->right = max_node.get();
    max_node->left = other_left;
    if (other.max_node->key > max_node->key) {
        max_node.release();
        max_node.reset(other.max_node.release());
    }
    setSize(getSize() + other.getSize());
    other.setSize(0);
}

void Fibonacci::print() const {}
Element Fibonacci::getRandomElement() const {
  return {-1, -1};
}