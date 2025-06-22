#include "Analysis.hpp"

#include <ncurses.h>

#include <chrono>
#include <fstream>

#include "Binary.hpp"
#include "Fibonacci.hpp"
#include "Pairing.hpp"
#include "TwoThreeHeap.hpp"
#include "Utils.hpp"

void Analysis::printSubTest(const int size, const int iteration) {
  move(1, 0);
  clrtoeol();
  mvprintw(1, 0, "%s",
           std::format("{} test for {}\n", iteration, size).c_str());
  refresh();
}
Element Analysis::prepareToTest(const CollectionType type, const int size,
                                const int iteration,
                                std::unique_ptr<Heap> &collection) {
  using enum CollectionType;
  printSubTest(size, iteration);
  switch (type) {
    case BINARY: {
      collection = std::make_unique<Binary>(size);
      break;
    }
    case BINOMIAL: {
      collection = std::make_unique<Binomial>();
      break;
    }
    case PAIRING: {
      collection = std::make_unique<Pairing>();
      break;
    }
    case TWO_THREE: {
      collection = std::make_unique<TwoThreeHeap>();
      break;
    }
    case FIBONACCI: {
      collection = std::make_unique<Fibonacci>();
      break;
    }
  }
  Heap::fillFromFile(*collection, "numbers.txt", size);
  return {Utils::rng(0, END_SIZE), Utils::rng(0, END_SIZE)};
}

void Analysis::printTestHeader(const CollectionType type, std::string title) {
  using enum CollectionType;
  std::string collectionType;
  switch (type) {
    case BINARY: {
      collectionType = "Binary";
      break;
    }
    case BINOMIAL: {
      collectionType = "Binomial";
      break;
    }
    case PAIRING: {
      collectionType = "Pairing";
      break;
    }
    case TWO_THREE: {
      collectionType = "2-3";
      break;
    }
    case FIBONACCI: {
      collectionType = "Fibonacci";
      break;
    }
  }
  const auto string = std::format("Analyzing {} of {}", title, collectionType);
  mvprintw(0, 0, "%s", string.c_str());
}

std::map<int, long> Analysis::analyzeInsert(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Insertion");
  for (int i = START_SIZE; i <= END_SIZE; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Heap> collection;
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
  for (int i = START_SIZE; i <= END_SIZE; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Heap> collection;
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
std::map<int, long> Analysis::analyzeIncreaseKey(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Increase key");
  for (int i = START_SIZE; i <= END_SIZE; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Heap> collection;
      prepareToTest(type, i, j, collection);
      Element element = collection->getRandomElement();
      const auto start = std::chrono::high_resolution_clock::now();
      collection->increaseKey(element, Utils::gauss(0, END_SIZE));
      const auto end = std::chrono::high_resolution_clock::now();
      average +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }
    result.insert(std::pair(i, average / ITERATIONS));
  }
  return result;
}
std::map<int, long> Analysis::analyzeMeld(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Meld");
  for (int i = START_SIZE; i <= END_SIZE; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Heap> collection1;
      std::unique_ptr<Heap> collection2;
      prepareToTest(type, i / 2, j, collection1);
      prepareToTest(type, i / 2, j, collection2);
      const auto start = std::chrono::high_resolution_clock::now();
      collection1->meld(*collection2);
      const auto end = std::chrono::high_resolution_clock::now();
      average +=
          std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
              .count();
    }
    result.insert(std::pair(i, average / ITERATIONS));
  }
  return result;
}
std::map<int, long> Analysis::analyzePeek(const CollectionType type) {
  clear();
  std::map<int, long> result;
  printTestHeader(type, "Peek");
  for (int i = START_SIZE; i <= END_SIZE; i += 100'000) {
    long average = 0;
    for (int j = 0; j < ITERATIONS; j++) {
      std::unique_ptr<Heap> collection;
      prepareToTest(type, i, j, collection);
      const auto start = std::chrono::high_resolution_clock::now();
      collection->peek();
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
  std::ofstream ofs("results/" + filename);
  ofs << "size;time" << std::endl;
  for (const auto &[key, value] : data) {
    ofs << key << ";" << value << std::endl;
  }
  ofs.close();
}
void Analysis::analyze() {
  using enum CollectionType;
  std::map<int, long> data;
  // data = analyzeInsert(BINARY);
  // writeToFile("BinaryInsert.txt", data);
  // data = analyzeExtractMax(BINARY);
  // writeToFile("BinaryExtractMax.txt", data);
  // data = analyzeMeld(BINARY);
  // writeToFile("BinaryMeld.txt", data);
  // data = analyzePeek(BINARY);
  // writeToFile("BinaryPeek.txt", data);

  // data = analyzeInsert(BINOMIAL);
  // writeToFile("BinomialInsert.txt", data);
  // data = analyzeExtractMax(BINOMIAL);
  // writeToFile("BinomialExtractMax.txt", data);
  // data = analyzeMeld(BINOMIAL);
  // writeToFile("BinomialMeld.txt", data);
  // data = analyzePeek(BINOMIAL);
  // writeToFile("BinomialPeek.txt", data);

  data = analyzeInsert(PAIRING);
  writeToFile("PairingInsert.txt", data);
  data = analyzeExtractMax(PAIRING);
  writeToFile("PairingExtractMax.txt", data);
  data = analyzeMeld(PAIRING);
  writeToFile("PairingMeld.txt", data);
  data = analyzePeek(PAIRING);
  writeToFile("PairingPeek.txt", data);

  data = analyzeInsert(TWO_THREE);
  writeToFile("TwoThreeInsert.txt", data);
  data = analyzeExtractMax(TWO_THREE);
  writeToFile("TwoThreeExtractMax.txt", data);
  data = analyzeMeld(TWO_THREE);
  writeToFile("TwoThreeMeld.txt", data);
  data = analyzePeek(TWO_THREE);
  writeToFile("TwoThreePeek.txt", data);
}