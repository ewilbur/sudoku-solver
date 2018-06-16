#include "sudoku.h"

#include <random>
#include <vector>
#include <ctime>
#include <iostream>
#include <stdio.h>
#include <cstring>

using namespace std;

int charToInt(char);

int main(int argc, char **argv) {
  vector<tile_t> board;
  char buffer[256];

  while ( cin >> buffer ) {
    if ( !strncmp("Grid", buffer, 4) ) {
      if ( board.size() != 0 ) {
        Sudoku s(3, board);
        s.print();
        s.solve();
        cout << endl;
        s.trace();
        cout << "================================" << endl;
        board.clear();
      }
      cin >> buffer;
      continue;
    }

    for ( int i = 0; i < strnlen(buffer, 256); ++i)
      board.push_back(charToInt(buffer[i]));
  }
}

int charToInt(char c) {
  return (c - '0');
}
