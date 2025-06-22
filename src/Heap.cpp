#include "Heap.hpp"

#include <iostream>

#include "Utils.hpp"
void Heap::fillWithRandom(Heap &collection, const int size) {
  for (int i = 0; i < size; i++) {
    collection.insert(Element(Utils::rng(0, size), Utils::rng(0, 5 * size)));
  }
}
void Heap::fillFromFile(Heap &collection,
                              const std::string &filename, const int size) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    std::cerr << "Failed to open file " << filename << std::endl;
  }
  int number;
  int priority;
  int i = 0;
  while (ifs >> number && ifs >> priority) {
    if (i >= size) {
      break;
    }
    const auto element = Element(number, priority);
    collection.insert(element);
    i++;
  }
  ifs.close();
}