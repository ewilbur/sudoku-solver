#include <vector>
#include <iostream>
#include <algorithm>
#include "sudoku.h"

using namespace std;

Sudoku::Sudoku() 
  : boardValue(3)
  , ticks(0)
  , boardDimension(9) {

  vector<tile_t> row(boardDimension, 0);
  vector<enum TileStatus> rowStatus(boardDimension, Blank);
  for ( int i = 0; i < boardDimension; ++i ) {
    board.push_back(row);
    boardStatus.push_back(rowStatus);
  }
}

/* | This takes the board value (the length of the smallest square in a sudoku
 * puzzle) and the vector of puzzle values. If there are an insufficient number
 * of tiles then blank tiles will be appended until the size is appropriate. If
 * there are too many tiles then they will be truncated once the limit is
 * reached
 */
Sudoku::Sudoku(int boardValue, std::vector<tile_t> board)
  : boardValue(boardValue)
  , ticks(0)
  , boardDimension(boardValue * boardValue) {

  vector<tile_t> row;
  vector<enum TileStatus> rowStatus;

  if ( board.size() > (boardDimension * boardDimension) )
    board.resize(boardDimension * boardDimension);

  while ( board.size() < boardDimension * boardDimension )
    board.push_back(0);

  for ( int i = 0; i < boardDimension; ++i ) {
    for ( int j = 0; j < boardDimension; ++j ) {
      row.push_back(board[i * boardDimension + j]);
      if ( board[i * boardDimension + j] == 0 ) rowStatus.push_back(Blank);
      else rowStatus.push_back(Fixed);
    }
    this->board.push_back(row);
    this->boardStatus.push_back(rowStatus);
    row.clear();
    rowStatus.clear();
  }
}

Sudoku::~Sudoku() {}

enum TileStatus Sudoku::getBoardStatus(int row, int col) const {
  return this->boardStatus[row][col];
}

tile_t Sudoku::getBoardVal(int row, int col) const {
  return this->board[row][col];
}

bool Sudoku::isFull() const {
  for ( int i = 0; i < boardDimension; ++i )
    for ( int j = 0; j < boardDimension; ++j )
      if ( boardStatus[i][j] == Blank )
        return false;
  return true;
}

vector<tile_t> Sudoku::possibleMoves(int row, int col) const {
  vector<tile_t> maybeTiles;
  for ( tile_t i = 0; i < boardDimension; ++i )
    maybeTiles.push_back(i + 1);

  /* @ Authors note: while this can be made more efficient by handling this all
   * in a single `for` loop, I'm choosing not to because the speed increase will
   * be negligible and the readability significantly decreases by doing that */

  // Remove tiles that are in the same row
  for ( int i = 0; i < board[row].size(); ++i ) {
    if ( (i == col) || boardStatus[row][i] == Blank ) 
      continue;
    else
     maybeTiles.erase(
         remove(maybeTiles.begin()
               , maybeTiles.end()
               , board[row][i])
         , maybeTiles.end());
  }

  // Remove tiles that are in the same column
  for ( int i = 0; i < board.size(); ++i ) {
    if ( (i == row) || boardStatus[i][col] == Blank ) 
      continue;
    else
      maybeTiles.erase(
          remove(maybeTiles.begin()
                , maybeTiles.end()
                , board[i][col])
          , maybeTiles.end());
  }

  int localRow = (row / boardValue) * boardValue;
  int localCol = (col / boardValue) * boardValue;

  // Remove tiles that are in the same local square
  for ( int i = localRow; i < localRow + boardValue; ++i )
    for ( int j = localCol; j < localCol + boardValue; ++j ) {
      if ( (i == row && j == col) || boardStatus[i][j] == Blank ) 
        continue;
      else
        maybeTiles.erase(
            remove(maybeTiles.begin()
                  , maybeTiles.end()
                  , board[i][j])
            , maybeTiles.end());
    }

  return maybeTiles;
}

bool Sudoku::placePiece(tile_t tile, int row, int col) {
  if ( boardStatus[row][col] == Fixed || tile < 0 || tile > boardDimension)
      return false;
  if ( tile == 0 ) {
    board[row][col] = tile;
    boardStatus[row][col] = Blank;
    return true;
  }

  vector<tile_t> moves = possibleMoves(row, col);
  if ( find(moves.begin(), moves.end(), tile) != moves.end() ) {
    board[row][col] = tile;
    boardStatus[row][col] = Written;
    return true;
  }
  
  return false;
}

bool Sudoku::solveSudoku(int row, int col) {
  ticks++;
  int nextRow = 0;
  int nextCol = 0;
  vector<tile_t> moves;

  if ( col + 1 >= boardDimension ) {
    if ( row + 1 >= boardDimension )
      ;
    else {
      nextRow = row + 1;
      nextCol = 0;
    }
  } else {
    nextRow = row;
    nextCol = col + 1;
  }

  if ( isFull() )
    return true;

  if ( getBoardStatus(row, col) == Fixed )
    return solveSudoku(nextRow, nextCol);

  moves = possibleMoves(row, col);

  for ( int i = 0; i < moves.size(); ++i ) {
    placePiece(moves[i], row, col);
    if ( solveSudoku(nextRow, nextCol) )
      return true;
    placePiece(Blank, row, col);
  }

  return false;
}

bool Sudoku::solve() {
  return solveSudoku(0,0);
}

void Sudoku::trace() const {
  cout << "Board value: " << boardValue << endl;
  cout << "Board dimension: " << boardDimension << endl;
  cout << "Ticks: " << ticks << endl;

  cout << "Game board:                     Status board:" << endl;

  for ( int i = 0; i < this->board.size(); ++i ) {
    for ( int j = 0; j < this->board[i].size(); ++j )
      cout << board[i][j] << "  ";

    /* cout << "     "; */
    /* for ( int j = 0; j < this->boardStatus[i].size(); ++j ) */
    /*   cout << boardStatus[i][j] << "  "; */
    cout << endl;
  }
}

void Sudoku::print() const {
  for ( int i = 0; i < board.size(); ++i ) {
    for ( int j = 0; j < board.size(); ++j ) {
      if ( j % boardValue == 0 )
        cout << " | ";
      else
        cout << "  ";
      cout << board[i][j];
    }
    cout << " |";
    if ( (i + 1) % boardValue == 0 )
      cout << endl;
    cout << endl;
  }
}
