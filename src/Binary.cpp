#include "Binary.hpp"

#include <cmath>
Element Binary::getRandomElement() const {
  const auto levels = getLevels();
  std::vector<Element> allElements;
  for (const auto &level : levels) {
    for (Element element : level) {
      if (element != Element(-1, -1)) {
        allElements.push_back(element);
      }
    }
  }
  return allElements[Utils::rng(0, static_cast<int>(allElements.size()) - 1)];
}
void Binary::print() const {
  clear();
  const auto levels = getLevels();
  std::vector<size_t> spaces;
  for (int i = static_cast<int>(levels.size()) - 1; i >= 0; i--) {
    std::string levelString;
    auto prevSpaces = spaces;
    spaces.clear();
    for (int j = 0; j < levels[i].size(); j++) {
      auto element = levels[i][j];
      auto elementString = element.toString();
      int numOfSpaces = 1;
      if (!prevSpaces.empty()) {

        numOfSpaces = static_cast<int>(prevSpaces[j] - levelString.size());
        levelString += std::string(static_cast<int>(prevSpaces[j] -
                                                    elementString.find(';') -
                                                    levelString.size()),
                                   ' ');
      }
      if (j % 2 == 1) {
        spaces.push_back(levelString.size() -
                         static_cast<int>(std::ceil(numOfSpaces / 2.0)));
      }
      levelString += std::format("{} ", element.toString());
    }
    if (i != 0) {
      std::vector<size_t> semicolonPositions;

      size_t pos = levelString.find(';', 0);
      while (pos != std::string::npos) {
        semicolonPositions.push_back(pos);
        pos = levelString.find(';', pos + 1);
      }
      for (int j = 0; j < semicolonPositions.size(); j++) {
        j % 2 == 0
            ? mvprintw(2 * i - 1, static_cast<int>(semicolonPositions[j]) + 1,
                       "/")
            : mvprintw(2 * i - 1, static_cast<int>(semicolonPositions[j]) - 1,
                       "\\");
      }
    }
    size_t termination = levelString.find("(-1;-1)");
    while (termination < levelString.size()) {
      levelString = levelString.replace(termination, 7, 7, ' ');
      termination = levelString.find("(-1;-1)");
    }
    mvprintw(2 * i, 0, "%s", levelString.c_str());
  }
}
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