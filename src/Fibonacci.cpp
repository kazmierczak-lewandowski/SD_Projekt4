#include "Fibonacci.hpp"

#include <cmath>
#include <stack>
#include <vector>
void Fibonacci::insert(const Element element) {
  const auto newNode = new Node(element);
  newNode->left = newNode;
  newNode->right = newNode;
  if (maxNode != nullptr) {
    (maxNode->left)->right = newNode;
    newNode->right = maxNode;
    newNode->left = maxNode->left;
    maxNode->left = newNode;
    if (newNode->key > maxNode->key) maxNode = newNode;
  } else {
    maxNode = newNode;
  }
  setSize(getSize() + 1);
}
void Fibonacci::meld(Heap& otherHeap) {
  Fibonacci& other = dynamic_cast<Fibonacci&>(otherHeap);

  if (!other.maxNode) {
    return;
  }

  if (!maxNode) {
    maxNode = other.maxNode;
  } else {
    Node* thisLeft = maxNode->left;
    Node* otherLeft = other.maxNode->left;

    thisLeft->right = other.maxNode;
    other.maxNode->left = thisLeft;

    maxNode->left = otherLeft;
    otherLeft->right = maxNode;

    if (other.maxNode->key > maxNode->key) {
      maxNode = other.maxNode;
    }
  }

  setSize(getSize() + other.getSize());
  other.maxNode = nullptr;
  other.setSize(0);
}
void Fibonacci::print() const {}
Element Fibonacci::getRandomElement() const {}
void Fibonacci::fibonnaciLink(Node* ptr2, Node* ptr1) {
  (ptr2->left)->right = ptr2->right;
  (ptr2->right)->left = ptr2->left;
  if (ptr1->right == ptr1) maxNode = ptr1;
  ptr2->left = ptr2;
  ptr2->right = ptr2;
  ptr2->parent = ptr1;
  if (ptr1->child == nullptr) ptr1->child = ptr2;
  ptr2->right = ptr1->child;
  ptr2->left = (ptr1->child)->left;
  ((ptr1->child)->left)->right = ptr2;
  (ptr1->child)->left = ptr2;
  if (ptr2->key > (ptr1->child)->key) ptr1->child = ptr2;
  ptr1->degree++;
}
Element Fibonacci::peek() const {
  return maxNode != nullptr ? maxNode->key : Element{-1, -1};
}
Element Fibonacci::extractMax() {
  if (!maxNode) return {-1, -1};

  Node* temp = maxNode;
  Element removed = temp->key;

  if (temp->child) {
    Node* child = temp->child;
    do {
      Node* next = child->right;
      maxNode->left->right = child;
      child->left = maxNode->left;
      child->right = maxNode;
      maxNode->left = child;
      child->parent = nullptr;
      child = next;
    } while (child != temp->child);
  }

  temp->left->right = temp->right;
  temp->right->left = temp->left;

  if (temp == temp->right) {
    maxNode = temp->child;
  } else {
    maxNode = temp->right;
    consolidate();
  }

  delete temp;
  setSize(getSize() - 1);
  return removed;
}


void Fibonacci::consolidate() {
  if (!maxNode) return;

  std::vector<Node*> degreeTable(static_cast<int>(log2(getSize())) + 2, nullptr);
  std::vector<Node*> roots;
  Node* current = maxNode;

  do {
    roots.push_back(current);
    current = current->right;
  } while (current != maxNode);

  for (Node* root : roots) {
    int degree = root->degree;
    while (degreeTable[degree]) {
      Node* other = degreeTable[degree];
      if (root->key < other->key) {
        std::swap(root, other);
      }
      fibonnaciLink(other, root);
      degreeTable[degree] = nullptr;
      degree++;
      if (degree >= degreeTable.size()) {
        degreeTable.resize(degree + 1, nullptr);
      }
    }
    degreeTable[degree] = root;
  }

  maxNode = nullptr;
  for (Node* node : degreeTable) {
    if (node) {
      if (!maxNode) {
        maxNode = node;
        node->left = node;
        node->right = node;
      } else {
        maxNode->left->right = node;
        node->left = maxNode->left;
        node->right = maxNode;
        maxNode->left = node;
        if (node->key > maxNode->key) {
          maxNode = node;
        }
      }
    }
  }
}
void Fibonacci::cut(Node* found, Node* temp) const {
  if (found == found->right) temp->child = nullptr;

  (found->left)->right = found->right;
  (found->right)->left = found->left;
  if (found == temp->child) temp->child = found->right;

  temp->degree = temp->degree - 1;
  found->right = found;
  found->left = found;
  (maxNode->left)->right = found;
  found->right = maxNode;
  found->left = maxNode->left;
  maxNode->left = found;
  found->parent = nullptr;
  found->mark = 'B';
}

void Fibonacci::cascadeCut(Node* temp) {
  if (Node* ptr = temp->parent; ptr != nullptr) {
    if (temp->mark == 'W') {
      temp->mark = 'B';
    } else {
      cut(temp, ptr);
      cascadeCut(ptr);
    }
  }
}

void Fibonacci::increaseKey(Node* found, const int val) {
  found->key.setPriority(val);

  if (Node* temp = found->parent; temp != nullptr && found->key > temp->key) {
    cut(found, temp);
    cascadeCut(temp);
  }
  if (found->key > maxNode->key) maxNode = found;
}

void Fibonacci::increaseKey(const Element& element, const int newPriority) {
  Node* found = nullptr;
  std::stack<Node*> stack;
  stack.push(maxNode);
  maxNode->c = 'Y';

  while (!stack.empty()) {
    Node* current = stack.top();
    stack.pop();

    if (current->key == element) {
      found = current;
      break;
    }

    if (current->child != nullptr) {
      Node* child = current->child;
      do {
        stack.push(child);
        child = child->right;
      } while (child != current->child);
    }

    if (current->right->c != 'Y') {
      stack.push(current->right);
      current->right->c = 'Y';
    }
  }

  Node* reset = maxNode;
  do {
    reset->c = 'N';
    reset = reset->right;
  } while (reset != maxNode);

  if (found) {
    increaseKey(found, newPriority);
  }
}