#include "Binary.hpp"

#include <cmath>
std::vector<std::vector<Element>> Binary::getLevels() const {
  std::vector<std::vector<Element>> levels;
  const int size = getSize();
  if (size == 0) return levels;

  int levelNumber = 0;
  while (true) {
    const int start = static_cast<int>(std::pow(2, levelNumber)) - 1;
    if (start >= size) break;

    int end = static_cast<int>(std::pow(2, levelNumber + 1)) - 1;
    const int heapEnd = std::min(end, size);
    std::vector<Element> level;
    for (int i = start; i < heapEnd; i++) {
      level.push_back(elements[i]);
    }
    for (int i = heapEnd; i < end; i++) {
      level.emplace_back(-1, -1);
    }
    levels.push_back(level);
    levelNumber++;
  }
  return levels;
}
void Binary::insert(const Element element) {
  ensureCapacity();
  elements[getSize()] = element;
  heapifyUp(getSize());
  setSize(getSize() + 1);
}
Element Binary::extractMax() {
  const Element max = elements[0];
  elements[0] = elements[getSize() - 1];
  setSize(getSize() - 1);
  heapifyDown(0);
  return max;
}

int Binary::findElement(const Element &element) const {
  for (int i = 0; i < getSize(); i++) {
    if (elements[i] == element) {
      return i;
    }
  }
  return -1;
}
Element Binary::peek() const { return elements[0]; }
void Binary::increaseKey(const Element &element, const int newPriority) {
  const int index = findElement(element);
  if (index == -1) {
    return;
  }
  elements[index].setPriority(newPriority);
  heapifyUp(index);
}
void Binary::meld(Heap &otherHeap) {
  int sumSize = getSize() + otherHeap.getSize();
  while (getCapacity() < sumSize) {
    grow();
    sumSize = getSize() + otherHeap.getSize();
  }
  const int otherSize = otherHeap.getSize();
  for (int i = 0; i < otherSize; i++) {
    elements[getSize()] = otherHeap.extractMax();
    setSize(getSize() + 1);
  }
  for (int i = getSize() / 2 - 1; i >= 0; i--) {
    heapifyDown(i);
  }
}
void Binary::heapifyDown(const int index) {  // NOLINT(*-no-recursion)
  int largest = index;
  const int leftIndex = left(index);
  const int rightIndex = right(index);
  if (leftIndex < getSize() && elements[index] < elements[leftIndex]) {
    largest = left(index);
  }
  if (rightIndex < getSize() && elements[largest] < elements[rightIndex]) {
    largest = right(index);
  }
  if (largest != index) {
    std::swap(elements[index], elements[largest]);
    heapifyDown(largest);
  }
}
void Binary::heapifyUp(const int index) {
  int currentIndex = index;
  while (currentIndex > 0 &&
         elements[parent(currentIndex)] < elements[currentIndex]) {
    std::swap(elements[currentIndex], elements[parent(currentIndex)]);
    currentIndex = parent(currentIndex);
  }
}
int Binary::left(const int index) { return 2 * index + 1; }
int Binary::right(const int index) { return 2 * index + 2; }
int Binary::parent(const int index) { return (index - 1) / 2; }
void Binary::ensureCapacity() {
  if (capacity == getSize()) {
    grow();
  }
}
void Binary::grow() {
  auto newArr = std::make_unique<Element[]>(getSize() * 2);
  for (long i = 0; i < getSize(); i++) {
    newArr[i] = elements[i];
  }
  elements = std::move(newArr);
  capacity = getSize() * 2;
}