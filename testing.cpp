#include <assert.h>
#include <iostream>
#include <tuple>
using namespace std;

#include "Board.h"

void pieceCanBeDumped()
{
  Piece p(1);
  assert(p.dump() == "1P");

  Piece p2(2, 1);
  assert(p2.dump() == "2K");

  Piece n = Piece::NONE;
  assert(n.dump() == "  ");
}

void coordDiagonalsAreCorrect()
{
  Board::Coord D4('d',4);
  assert(D4.upperLeft() == Board::Coord('c',3));
  assert(D4.upperRight() == Board::Coord('c',5));
  assert(D4.lowerLeft() == Board::Coord('e',3));
  assert(D4.lowerRight() == Board::Coord('e',5));

  Board::Coord F8('f',8);
  assert(F8.upperLeft() == Board::Coord('e',7));
  assert(F8.upperRight() == Board::Coord('e',1));
  assert(F8.lowerLeft() == Board::Coord('g',7));
  assert(F8.lowerRight() == Board::Coord('g',1));
}

void boardCanBeCleared()
{
  Board board;
  board.clear();

  assert(board.get(Board::Coord('a',1)) == Piece::NONE);
  assert(board.get(Board::Coord('a',3)) == Piece::NONE);
  assert(board.get(Board::Coord('g',1)) == Piece::NONE);
  assert(board.get(Board::Coord('g',3)) == Piece::NONE);
}

void boardHoldsEmptyPieces()
{
  Board board;

  Piece a1 = board.get(Board::Coord('a', 2));
  assert(a1 == Piece::NONE);
}

void boardCanNormalizeColumns()
{
  Board board;
  Piece unique(3, 3); // nonsensical Piece for easy testing
  board.set(unique, Board::Coord('a', 1));

  Piece a1 = board.get(Board::Coord('a', 1));
  assert(a1 == unique);
  Piece a9 = board.get(Board::Coord('a', 9));
  assert(a9 == unique);
  Piece aNeg7 = board.get(Board::Coord('a', -7));
  assert(aNeg7 == unique);
}

void boardIsSetUpForTwoPlayers()
{
  Board board;

  // Player 1's pieces are in the a/b/c rows
  assert(board.get(Board::Coord('a', 1)) == Piece(1));
  assert(board.get(Board::Coord('b', 8)) == Piece(1));
  assert(board.get(Board::Coord('c', 5)) == Piece(1));

  // Player 2's pieces are in the f/g/h rows
  assert(board.get(Board::Coord('g', 1)) == Piece(2));
}

void doNotAllowMovingNonpieces()
{
  Board board;

  assert(board.legalMove(Board::Coord('e',2), Board::Coord('d',3)) == false);
}

void doNotAllowNonsensicalMoves()
{
  Board board;

  // No piece there to move!
  assert(board.legalMove(Board::Coord('e',2), Board::Coord('e',2)) == false);
  // Cannot move on top of your own piece!
  assert(board.legalMove(Board::Coord('a',1), Board::Coord('b',2)) == false);
  // Cannot jump your own piece!
  assert(board.legalMove(Board::Coord('b',2), Board::Coord('d',4)) == false);
}

void piecesPromoteOnLastRow()
{
  Board board;
  board.clear();
  board.setPlayerDirection(1, Board::Direction::A_TO_H);
  board.setPlayerDirection(2, Board::Direction::H_TO_A);

  const Board::Coord G1('g',1);
  const Board::Coord H2('h',2);
  board.set(Piece(1), G1);

  const Board::Coord A1('a',1);
  const Board::Coord B2('b',2);
  board.set(Piece(2), B2);

  assert(board.legalMove(G1, H2) == true);
  assert(board.move(G1,H2) == true);
  assert(board.get(H2).player == 1);
  assert(board.get(H2).isKing());

  assert(board.legalMove(B2, A1) == true);
  assert(board.move(B2,A1) == true);
  assert(board.get(A1).player == 2);
  assert(board.get(A1).isKing());
}

void basicPawnMovement()
{
  Board board;

  // Player 1's pawns move in an A_TO_H direction
  assert(board.move(Board::C1, Board::D2) == true);
  assert(board.get(Board::D2) != Piece::NONE);
  assert(board.get(Board::D2).player == 1);
  assert(board.get(Board::D2).isPawn() == true);
  assert(board.get(Board::C1) == Piece::NONE);

  // Player 2's pawns move in an H_TO_A direction
  assert(board.move(Board::F2, Board::E3) == true);
  assert(board.get(Board::E3) != Piece::NONE);
  assert(board.get(Board::E3).player == 2);
  assert(board.get(Board::E3).isPawn() == true);
  assert(board.get(Board::F2) == Piece::NONE);

  // Either pawns can wrap around
  assert(board.move(Board::B8, Board::C1) == true);
  assert(board.get(Board::C1) != Piece::NONE);
  assert(board.get(Board::C1).player == 1);
  assert(board.get(Board::C1).isPawn() == true);
  assert(board.get(Board::B8) == Piece::NONE);

  assert(board.move(Board::F8, Board::E1) == true);
  assert(board.get(Board::E1) != Piece::NONE);
  assert(board.get(Board::E1).player == 2);
  assert(board.get(Board::E1).isPawn() == true);
  assert(board.get(Board::F8) == Piece::NONE);
}

void basicKingMovement()
{
  Board board;
  board.clear();
  board.set(Piece(1, 1), Board::Coord('d',4));
  //cout << endl << board.dump() << endl;

  assert(board.legalMove(Board::Coord('d',4), Board::Coord('c',3)) == true);
  assert(board.legalMove(Board::Coord('d',4), Board::Coord('c',5)) == true);
  assert(board.legalMove(Board::Coord('d',4), Board::Coord('e',3)) == true);
  assert(board.legalMove(Board::Coord('d',4), Board::Coord('e',5)) == true);

  assert(board.legalMove(Board::Coord('d',4), Board::Coord('c',4)) == false);
  assert(board.legalMove(Board::Coord('d',4), Board::Coord('c',1)) == false);
  assert(board.legalMove(Board::Coord('d',4), Board::Coord('h',8)) == false);
}

void edgesPawnMovement()
{
  Board board;
  assert(board.move(Board::C1, Board::D8) == true);
}

void edgesKingMovement()
{
  Board board;
  board.clear();
  board.set(Piece(1, 1), Board::Coord('c',8));

  assert(board.legalMove(Board::Coord('c',8), Board::Coord('d',1)) == true);
  assert(board.legalMove(Board::Coord('c',8), Board::Coord('d',7)) == true);
  assert(board.legalMove(Board::Coord('c',8), Board::Coord('b',1)) == true);
  assert(board.legalMove(Board::Coord('c',8), Board::Coord('b',7)) == true);
}

void pawnsCanJump()
{

}

void kingsCanJump()
{

}

int main(int argc, char* argv[])
{
  cout << "Testing..." << endl;
  cout << "."; pieceCanBeDumped();
  cout << "."; coordDiagonalsAreCorrect();
  cout << "."; boardHoldsEmptyPieces();
  cout << "."; boardCanBeCleared();
  cout << "."; boardCanNormalizeColumns();
  cout << "."; boardIsSetUpForTwoPlayers();
  cout << "."; doNotAllowMovingNonpieces();
  cout << "."; doNotAllowNonsensicalMoves();
  cout << "."; piecesPromoteOnLastRow();
  cout << "."; basicPawnMovement();
  cout << "."; basicKingMovement();
  cout << "."; edgesPawnMovement();
  cout << "."; edgesKingMovement();
  cout << "."; pawnsCanJump();
  cout << "."; kingsCanJump();
  cout << endl << "End testing" << endl;

  return 0;
}