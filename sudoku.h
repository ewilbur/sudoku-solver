#ifndef SUDOKU_H_
#define SUDOKU_H_

#include <vector>

typedef unsigned int tile_t;

enum TileStatus {
  Blank,
  Fixed,
  Written,
};

class Sudoku {
  public:
    int ticks;
    int boardValue;
    int boardDimension;
    std::vector< std::vector<tile_t> > board;
    std::vector< std::vector<enum TileStatus> > boardStatus;

    enum TileStatus getBoardStatus(int, int) const;
    tile_t getBoardVal(int, int) const;
    bool isFull() const;
    std::vector<tile_t> possibleMoves(int, int) const;
    bool placePiece(tile_t, int, int);
    bool solveSudoku(int, int);

  public:
    Sudoku();
    Sudoku(int, std::vector<tile_t>);
    ~Sudoku();

    void trace() const;
    void print() const;
    bool solve();
};

#endif // SUDOKU_H_
