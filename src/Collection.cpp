#include "Collection.hpp"
void Collection::print() const {
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
void Collection::fillWithRandom(Collection &collection, const int size) {
  for (int i = 0; i < size; i++) {
    collection.insert(Element(Utils::rng(0, size), Utils::rng(0, 5 * size)));
  }
}
void Collection::fillFromFile(Collection &collection,
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
Element Collection::getRandomElement() const {
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