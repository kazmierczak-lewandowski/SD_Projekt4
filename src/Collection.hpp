#ifndef COLLECTION_HPP
#define COLLECTION_HPP
#include "Element.hpp"
#include "Utils.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <vector>

class Collection {
public:
  virtual ~Collection() = default;
  [[nodiscard]] virtual std::vector<std::vector<Element>> getLevels() const = 0;
  virtual void insert(Element element) = 0;
  virtual Element extractMax() = 0;
  [[nodiscard]] virtual Element peek() const = 0;
  virtual void increaseKey(const Element &element, int newPriority) = 0;
  virtual void print() const;
  [[nodiscard]] int getSize() const { return size; }
  static void fillWithRandom(Collection &collection, int size);
  static void fillFromFile(Collection &collection, const std::string &filename,
                           int size);
  [[nodiscard]] Element getRandomElement() const;
  [[nodiscard]] bool isEmpty() const { return size == 0; }

protected:
  void setSize(const int newSize) { size = newSize; }

private:
  int size = 0;
};

#endif // COLLECTION_HPP
