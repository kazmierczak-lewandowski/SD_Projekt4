#ifndef HEAP_HPP
#define HEAP_HPP
#include "Element.hpp"
#include "Utils.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <vector>

class Heap {
public:
  virtual ~Heap() = default;
  [[nodiscard]] virtual Element peek() const = 0;
  virtual Element extractMax() = 0;
  virtual void increaseKey(const Element &element, int newPriority) = 0;
  virtual void insert(Element element) = 0;
  virtual void meld(Heap &otherHeap) = 0;
  virtual void print() const = 0;
  [[nodiscard]] int getSize() const { return size; }
  static void fillWithRandom(Heap &collection, int size);
  static void fillFromFile(Heap &collection, const std::string &filename,
                           int size);
  [[nodiscard]] virtual Element getRandomElement() const = 0;
  [[nodiscard]] bool isEmpty() const { return size == 0; }

protected:
  void setSize(const int newSize) { size = newSize; }

private:
  int size = 0;
};

#endif // HEAP_HPP
