#include <ncurses.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "Analysis.hpp"
#include "Binary.hpp"
#include "Heap.hpp"

void menuLoop(const std::vector<std::string> &choices, int &highlight) {
  int choice = 0;
  while (choice != 10) {
    for (int i = 0; i < choices.size(); i++) {
      if (i == highlight) {
        attron(A_REVERSE);
      }
      mvprintw(i, 0, "%s", choices[i].c_str());
      attroff(A_REVERSE);
    }
    choice = getch();

    switch (choice) {
      case KEY_UP: {
        if (highlight != 0) highlight--;
        break;
      }
      case KEY_DOWN: {
        if (highlight != choices.size() - 1) highlight++;
        break;
      }
      default:
        break;
    }
  }
}

int getInput(const std::string &message) {
  clear();
  int input = 0;
  while (true) {
    printw("%s", message.c_str());
    scanw("%d", &input);
    if (input < 1) {
      printw("Niepoprawny wybor. Sprobuj ponownie.\n");
      continue;
    }
    return input;
  }
}

int main() {
  Heap *h1 = new Binary();
  Heap *h2 = new Binary();
  Heap::fillWithRandom(*h1, 10);
  Heap::fillWithRandom(*h2, 10);
  h1->meld(*h2);
  for (int i = 0; i < h1->getSize(); i++) {
    std::cout << h1->extractMax().toString() << std::endl;
  }
  return 0;
}
