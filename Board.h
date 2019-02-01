#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace std;

struct Piece
{
public:
  int player; // 0: Red, 1: Black
  int rank; // 0: Pawn, 1: King

public:
  Piece(int p, int r) : player(p), rank(r) { }
  Piece(int p) : player(p), rank(0) { }
  Piece() : player(-1), rank(-1) { }

public:
  bool isPawn() { return rank == 0; }
  bool isKing() { return rank == 1; }

public:
  string dump() {
    return (player == -1 && rank == -1 ? "  " :
      (to_string(player) + (rank == 0 ? "P" : "K") )
    );
  }

public:
  friend bool operator==(const Piece& lhs, const Piece& rhs);
  friend bool operator!=(const Piece& lhs, const Piece& rhs);

public:
  static const Piece NONE;
};


/*
 * Board is a checkerboard of rows and columns holding Pieces.
 * Empty squares are denoted by the constant Piece::NONE.
 * Rows are denoted by "a" through "h".
 * Columns are numbered 1 through 8. Columns can "wrap", so that
 * a1 == a9 == a17 == a-7 == a=15. (This is what will make the
 * board a cylinder rather than a flat board.)
 */
class Board
{
public:
  struct Coord
  {
  public:
    char row;
    int col;

  public:
    Coord(char r, int c) : row(r), col(c) { normalize(); }
    Coord(string r, int c) : row(r[0]), col(c) { normalize(); }

  public:
    Coord upperRight() { return Coord(row-1, col+1); }
    Coord upperLeft() { return Coord(row-1, col-1); }
    Coord lowerRight() { return Coord(row+1, col+1); }
    Coord lowerLeft() { return Coord(row+1, col-1); }

  public:
    void normalize() 
    {
      if (row < 'a' || row > 'h') row = -1; 
      while (col < 1) col += 8;
      while (col > 8) col -= 8;
    }

  public:
    friend bool operator==(const Coord& lhs, const Coord& rhs);
    friend bool operator!=(const Coord& lhs, const Coord& rhs);
  };
  static const Coord A1, A2, A3, A4, A5, A6, A7, A8;
  static const Coord B1, B2, B3, B4, B5, B6, B7, B8;
  static const Coord C1, C2, C3, C4, C5, C6, C7, C8;
  static const Coord D1, D2, D3, D4, D5, D6, D7, D8;
  static const Coord E1, E2, E3, E4, E5, E6, E7, E8;
  static const Coord F1, F2, F3, F4, F5, F6, F7, F8;
  static const Coord G1, G2, G3, G4, G5, G6, G7, G8;
  static const Coord H1, H2, H3, H4, H5, H6, H7, H8;

  enum Direction
  {
    A_TO_H, H_TO_A
  };

public:
  Board();
  ~Board();

  /*
   * Board state
   */
public:
  void clear();
  Piece get(const Coord& coord);
  void set(Piece p, const Coord& coord);
private:
  int normalizeColumn(int col);

  /*
   * Game state
   */
public:
  bool isStalemate();
  int isPlayerVictory();
  int playerPiecesRemaining(int player);

  /*
   * Player piece movement
   */
public:
  void setPlayerDirection(int player, Direction dir);
  bool legalMove(const Coord& from, const Coord& to);
  bool move(const Coord& from, const Coord& to);
private:
  bool isLastRow(const Piece& p, const Coord& c);

  /*
   * Diagnostics
   */
public:
  string dump();
  void setVerbose() { verbose = true; }
  void clearVerbose() { verbose = false; }
private:
  string dumpRow(vector<Piece>& row);
  bool verbose;

private:
  /*
   * Row storage
   */
  vector<Piece> a;
  vector<Piece> b;
  vector<Piece> c;
  vector<Piece> d;
  vector<Piece> e;
  vector<Piece> f;
  vector<Piece> g;
  vector<Piece> h;

  map<int, Direction> playerDirections;
};