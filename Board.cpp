#include "Board.h"

#include <iostream>

const Piece Piece::NONE(-1, -1);

bool operator==(const Piece& lhs, const Piece& rhs)
{
  return ( (lhs.rank == rhs.rank) && (lhs.player == rhs.player) );
}
bool operator!=(const Piece& lhs, const Piece& rhs)
{
  return !(lhs == rhs);
}


bool operator==(const Board::Coord& lhs, const Board::Coord& rhs)
{
  return ( (lhs.row == rhs.row) && (lhs.col == rhs.col) );
}
bool operator!=(const Board::Coord& lhs, const Board::Coord& rhs)
{
  return !(lhs == rhs);
}

Board::Board()
  : a(8), b(8), c(8), d(8), e(8), f(8), g(8), h(8), verbose(false)
{
  // now set up a normal 2-player game
  set(Piece(1), Coord('a', 1));
  set(Piece(1), Coord('a', 3));
  set(Piece(1), Coord('a', 5));
  set(Piece(1), Coord('a', 7));
  set(Piece(1), Coord('b', 2));
  set(Piece(1), Coord('b', 4));
  set(Piece(1), Coord('b', 6));
  set(Piece(1), Coord('b', 8));
  set(Piece(1), Coord('c', 1));
  set(Piece(1), Coord('c', 3));
  set(Piece(1), Coord('c', 5));
  set(Piece(1), Coord('c', 7));
  setPlayerDirection(1, Direction::A_TO_H);

  set(Piece(2), Coord('f', 2));
  set(Piece(2), Coord('f', 4));
  set(Piece(2), Coord('f', 6));
  set(Piece(2), Coord('f', 8));
  set(Piece(2), Coord('g', 1));
  set(Piece(2), Coord('g', 3));
  set(Piece(2), Coord('g', 5));
  set(Piece(2), Coord('g', 7));
  set(Piece(2), Coord('h', 2));
  set(Piece(2), Coord('h', 4));
  set(Piece(2), Coord('h', 6));
  set(Piece(2), Coord('h', 8));
  setPlayerDirection(2, Direction::H_TO_A);
}

Board::~Board()
{
}

void Board::clear()
{
  for (int i=0; i<8; i++)
  {
    a[i] = Piece::NONE;
    b[i] = Piece::NONE;
    c[i] = Piece::NONE;
    d[i] = Piece::NONE;
    e[i] = Piece::NONE;
    f[i] = Piece::NONE;
    g[i] = Piece::NONE;
    h[i] = Piece::NONE;
    playerDirections.clear();
  }
}
Piece Board::get(const Coord& coord)
{
  int col = normalizeColumn(coord.col);

  // Find it
  switch (coord.row)
  {
    case 'a': return a[col];
    case 'b': return b[col];
    case 'c': return c[col];
    case 'd': return d[col];
    case 'e': return e[col];
    case 'f': return f[col];
    case 'g': return g[col];
    case 'h': return h[col];
    default:
      throw "Unrecognized row request";
  }
}
void Board::set(Piece piece, const Coord& coord)
{
  int col = normalizeColumn(coord.col);
  switch (coord.row)
  {
    case 'a': a[col] = piece; break;
    case 'b': b[col] = piece; break;
    case 'c': c[col] = piece; break;
    case 'd': d[col] = piece; break;
    case 'e': e[col] = piece; break;
    case 'f': f[col] = piece; break;
    case 'g': g[col] = piece; break;
    case 'h': h[col] = piece; break;
    default:
      throw "Unrecognized row request";
  }
}
int Board::normalizeColumn(int col)
{
  // Adjust to be within the range 1 to 8
  while (col > 8)
    col -= 8;
  while (col < 1)
    col += 8;
  
  // Adjust for 0-offset
  col = col - 1;

  return col;
}

bool Board::isStalemate()
{
  return false;
}
int Board::isPlayerVictory()
{
  return -1;
}

void Board::setPlayerDirection(int player, Direction dir)
{
  playerDirections[player] = dir;
}
bool Board::isLastRow(const Piece& p, const Coord& c)
{
  Direction dir = playerDirections[p.player];
  if ( (dir == Direction::A_TO_H) && (c.row == 'h') )
    return true;
  if ( (dir == Direction::H_TO_A) && (c.row == 'a') )
    return true;
  return false;
}
bool Board::legalMove(const Coord& constFrom, const Coord& constTo)
{
  Coord from = const_cast<Coord&>(constFrom);
  Coord to = const_cast<Coord&>(constTo);

  // Are we moving from and to the same place?
  if (from == to)
    return false;

  // Is there a piece at "from"? (Can't move a piece if it doesn't exist)
  Piece movingPiece = get(from);
  if (movingPiece == Piece::NONE)
    return false;

  // Is there a piece at "to"? (Can't land in an occupied square)
  if (get(to) != Piece::NONE)
    return false;

  // Is this piece a Pawn or a King?
  if (movingPiece.isPawn())
  { // This is a Pawn -- it can only move forwards
    Direction playerDir = playerDirections[movingPiece.player];
    
    // Is "to" one space away on the diagonals?
    if (playerDir == A_TO_H && 
        ( (to == from.lowerRight()) || (to == from.lowerLeft()) )
       )
    {
      return true;
    }
    if (playerDir == H_TO_A &&
        ( (to == from.upperRight()) || (to == from.upperLeft()) )
       )
    {
      return true;
    }

    // Is "to" two spaces away on the diagonals, is that space empty,
    // and is there a piece in between?
    if (playerDir == A_TO_H)
    {
      if (to == from.lowerRight().lowerRight() && 
          get(from.lowerRight()).player != movingPiece.player)
      {
        // This is a jump
        return true;
      }
      if (to == from.lowerLeft().lowerLeft() && 
          get(from.lowerLeft()).player != movingPiece.player)
      {
        // This is a jump
        return true;
      }
    }
    if (playerDir == H_TO_A)
    {
      if (to == from.upperRight().upperRight() &&
          get(from.upperRight()).player != movingPiece.player)
      {
        // This is a jump
        return true;
      }
      if (to == from.upperLeft().upperLeft() && 
          get(from.upperLeft()).player != movingPiece.player)
      {
        // This is a jump
        return true;
      }
    }
  }
  else 
  { // This is a King -- it can move backwards

    // Is "to" one space away on the diagonals, and is that space empty?
    if (
        (to == from.upperRight()) || (to == from.upperLeft()) ||
        (to == from.lowerRight()) || (to == from.lowerLeft())
       )
    {
      return true;
    }

    // Is "to" two spaces away on the diagonals
    // and is there a piece in between?
    if (to == from.upperRight().upperRight() &&
        get(from.upperRight()).player != movingPiece.player)
    {
      // This is a jump
      return true;
    }
    if (to == from.upperLeft().upperLeft() && 
        get(from.upperLeft()).player != movingPiece.player)
    {
      // This is a jump
      return true;
    }
    if (to == from.lowerRight().lowerRight() && 
        get(from.lowerRight()).player != movingPiece.player)
    {
      // This is a jump
      return true;
    }
    if (to == from.lowerLeft().lowerLeft() && 
        get(from.lowerLeft()).player != movingPiece.player)
    {
      // This is a jump
      return true;
    }
  }

  return false;
}
bool Board::move(const Coord& cfrom, const Coord& cto)
{
  Coord from = const_cast<Coord&>(cfrom);
  Coord to = const_cast<Coord&>(cto);

  if (verbose)
    cout << "MOVE: " << from.row << from.col << 
      " TO " << to.row << to.col << endl;

  if (!legalMove(from, to))
  {
    if (verbose) cout << "*** REJECTED" << endl;
    return false;
  }

  Piece piece = get(from);

  // Is this a jump?
  if (from.row - to.row == 2 ||
      from.row - to.row == -2)
  {
    // This is a jump; remove that piece
    if (verbose) cout << "JUMP!!" << endl;
    if (to == from.upperRight().upperRight())
    {
      set(Piece::NONE, from.upperRight());
    }
    if (to == from.upperLeft().upperLeft())
    {
      set(Piece::NONE, from.upperLeft());
    }
    if (to == from.lowerRight().lowerRight())
    {
      set(Piece::NONE, from.lowerRight());
    }
    if (to == from.lowerLeft().lowerLeft())
    {
      set(Piece::NONE, from.lowerLeft());
    }
  }
  set(Piece::NONE, from);
  set(piece, to);

  // If this piece has reached the other player's side of the
  // board, and if this piece is a Pawn, it is now promoted
  // to be a King
  if ( (piece.isPawn()) && isLastRow(piece, to) )
  {
    set(Piece(piece.player, 1), to);
  }

  if (verbose) cout << dump() << endl;

  return true;
}

string Board::dump()
{
  string retval = "Board: 1     2     3     4     5     6     7     8\n";

  retval += "a" + dumpRow(a);
  retval += "b" + dumpRow(b);
  retval += "c" + dumpRow(c);
  retval += "d" + dumpRow(d);
  retval += "e" + dumpRow(e);
  retval += "f" + dumpRow(f);
  retval += "g" + dumpRow(g);
  retval += "h" + dumpRow(h);

  return retval;
}
string Board::dumpRow(vector<Piece>& row)
{
  string retval = ":";
  for (int i=0; i<8; i++)
    retval += " || " + (row[i]).dump();
  retval += " ||\n";
  return retval;
}

const Board::Coord Board::A1('a',1);
const Board::Coord Board::A2('a',2);
const Board::Coord Board::A3('a',3);
const Board::Coord Board::A4('a',4);
const Board::Coord Board::A5('a',5);
const Board::Coord Board::A6('a',6);
const Board::Coord Board::A7('a',7);
const Board::Coord Board::A8('a',8);
const Board::Coord Board::B1('b',1);
const Board::Coord Board::B2('b',2);
const Board::Coord Board::B3('b',3);
const Board::Coord Board::B4('b',4);
const Board::Coord Board::B5('b',5);
const Board::Coord Board::B6('b',6);
const Board::Coord Board::B7('b',7);
const Board::Coord Board::B8('b',8);
const Board::Coord Board::C1('c',1);
const Board::Coord Board::C2('c',2);
const Board::Coord Board::C3('c',3);
const Board::Coord Board::C4('c',4);
const Board::Coord Board::C5('c',5);
const Board::Coord Board::C6('c',6);
const Board::Coord Board::C7('c',7);
const Board::Coord Board::C8('c',8);
const Board::Coord Board::D1('d',1);
const Board::Coord Board::D2('d',2);
const Board::Coord Board::D3('d',3);
const Board::Coord Board::D4('d',4);
const Board::Coord Board::D5('d',5);
const Board::Coord Board::D6('d',6);
const Board::Coord Board::D7('d',7);
const Board::Coord Board::D8('d',8);
const Board::Coord Board::E1('e',1);
const Board::Coord Board::E2('e',2);
const Board::Coord Board::E3('e',3);
const Board::Coord Board::E4('e',4);
const Board::Coord Board::E5('e',5);
const Board::Coord Board::E6('e',6);
const Board::Coord Board::E7('e',7);
const Board::Coord Board::E8('e',8);
const Board::Coord Board::F1('f',1);
const Board::Coord Board::F2('f',2);
const Board::Coord Board::F3('f',3);
const Board::Coord Board::F4('f',4);
const Board::Coord Board::F5('f',5);
const Board::Coord Board::F6('f',6);
const Board::Coord Board::F7('f',7);
const Board::Coord Board::F8('f',8);
const Board::Coord Board::G1('g',1);
const Board::Coord Board::G2('g',2);
const Board::Coord Board::G3('g',3);
const Board::Coord Board::G4('g',4);
const Board::Coord Board::G5('g',5);
const Board::Coord Board::G6('g',6);
const Board::Coord Board::G7('g',7);
const Board::Coord Board::G8('g',8);
const Board::Coord Board::H1('h',1);
const Board::Coord Board::H2('h',2);
const Board::Coord Board::H3('h',3);
const Board::Coord Board::H4('h',4);
const Board::Coord Board::H5('h',5);
const Board::Coord Board::H6('h',6);
const Board::Coord Board::H7('h',7);
const Board::Coord Board::H8('h',8);
