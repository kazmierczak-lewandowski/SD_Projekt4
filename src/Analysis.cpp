#include "Analysis.hpp"

#include <ncurses.h>

#include <chrono>
#include <fstream>
#include "Binary.hpp"

void Analysis::printSubTest(const int size, const int iteration) {
  move(1, 0);
  clrtoeol();
  mvprintw(1, 0, "%s",
           std::format("{} test for {}\n", iteration, size).c_str());
  refresh();
}
Element Analysis::prepareToTest(const CollectionType type, const int size,
                                const int iteration,
                                std::unique_ptr<Collection> &collection) {
  printSubTest(size, iteration);
  //TODO: Collection switch
  Collection::fillFromFile(
      *collection, "../src/numbers.txt",
      size);
  return {Utils::rng(0, 5'000'000), Utils::rng(0, 25'000'000)};
}

void Analysis::printTestHeader(const CollectionType type, std::string title) {
  const auto string =
      std::format("Analyzing {} of {}", title,
                  type == CollectionType::HEAP ? "Heap" : "BST");
  mvprintw(0, 0, "%s", string.c_str());
}

std::map<int, long> Analysis::analyzeInsert(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Insertion");
  for (int i = 100'000; i <= 5'000'000; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Collection> collection;
      const auto element = prepareToTest(type, i, j, collection);
      const auto start = std::chrono::high_resolution_clock::now();
      collection->insert(element);
      const auto end = std::chrono::high_resolution_clock::now();
      average +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }
    result.insert(std::pair(i, average / ITERATIONS));
  }
  return result;
}
std::map<int, long> Analysis::analyzeExtractMax(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Extract max");
  for (int i = 100'000; i <= 5'000'000; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Collection> collection;
      prepareToTest(type, i, j, collection);
      const auto start = std::chrono::high_resolution_clock::now();
      collection->extractMax();
      const auto end = std::chrono::high_resolution_clock::now();
      average +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }
    result.insert(std::pair(i, average / ITERATIONS));
  }
  return result;
}
std::map<int, long> Analysis::analyzeincreaseKey(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Modify key");
  for (int i = 100'000; i <= 5'000'000; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Collection> collection;
      prepareToTest(type, i, j, collection);
      Element element = collection->getRandomElement();
      const auto start = std::chrono::high_resolution_clock::now();
      collection->increaseKey(element, Utils::gauss(0, 25'000'000));
      const auto end = std::chrono::high_resolution_clock::now();
      average +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }
    result.insert(std::pair(i, average / ITERATIONS));
  }
  return result;
}
void Analysis::writeToFile(const std::string &filename,
                           const std::map<int, long> &data) {
  std::ofstream ofs("../results/" +
                    filename);
  ofs << "size;time" << std::endl;
  for (const auto &[key, value] : data) {
    ofs << key << ";" << value << std::endl;
  }
  ofs.close();
}
void Analysis::analyze() {
  using enum CollectionType;
  std::map<int, long> data;
}