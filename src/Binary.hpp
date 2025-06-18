#ifndef BINARY_HPP
#define BINARY_HPP
#include "Collection.hpp"
#include "Element.hpp"

#include <memory>
#include <span>
#include <vector>

class Binary final : public Collection {
public:
  explicit Binary() : elements(std::make_unique<Element[]>(10)), capacity(10) {};
  explicit Binary(const int capacity)
      : elements(std::make_unique<Element[]>(capacity)), capacity(capacity) {};
  ~Binary() override = default;
  [[nodiscard]] int getCapacity() const { return capacity; }
  [[nodiscard]] Element *getElements() const { return elements.get(); }
  [[nodiscard]] std::vector<std::vector<Element>> getLevels() const override;
  void insert(Element element) override;
  Element extractMax() override;
  [[nodiscard]] Element peek() const override;
  [[nodiscard]] int findElement(const Element &element) const;
  void increaseKey(const Element &element, int newPriority) override;

private:
  std::unique_ptr<Element[]> elements;
  int capacity = 0;
  void heapifyDown(int index);
  void heapifyUp(int index);
  [[nodiscard]] static int left(int index);
  [[nodiscard]] static int right(int index);
  [[nodiscard]] static int parent(int index);
  void ensureCapacity();
  void grow();

};

#endif // BINARY_HPP
