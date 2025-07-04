#ifndef ANALYSIS_HPP
#define ANALYSIS_HPP
#include <map>
#include <memory>

#include "Heap.hpp"

class Analysis {
 private:
  enum class CollectionType {
    BINARY = 0,
    BINOMIAL = 1,
    PAIRING = 2,
    TWO_THREE = 3,
    FIBONACCI = 4
  };
  static constexpr int START_SIZE = 100'000;
  static constexpr int END_SIZE = 2'000'000;
  static constexpr int ITERATIONS = 20;
  static void printSubTest(int size, int iteration);
  static Element prepareToTest(CollectionType type, int size, int iteration,
                               std::unique_ptr<Heap> &collection);

  static void printTestHeader(CollectionType type, std::string title);

  static std::map<int, long> analyzeInsert(CollectionType type);
  static std::map<int, long> analyzeExtractMax(CollectionType type);
  static std::map<int, long> analyzeIncreaseKey(CollectionType type);
  static std::map<int, long> analyzeMeld(CollectionType type);
  static std::map<int, long> analyzePeek(CollectionType type);
  static void writeToFile(const std::string &filename,
                          const std::map<int, long> &data);

 public:
  static void analyze();
};

#endif  // ANALYSIS_HPP
