/*
 * This is a console-based game of checkers, designed to act as an
 * interactive testbed for the CylCheckers codebase
 */

#include <tuple>
#include <fstream>
#include <iostream>
using namespace std;

#include "Board.h"

string getPlayerInput()
{
  string input = "";
  cout << "> ";
  cin >> input;
  return input;
}

void help()
{
  cout << "QUIT|quit|q   : Terminate the game" << endl;
  cout << "HELP|help|h   : Show this help" << endl;
  cout << "Moves take the form of coordinate,coordinate pairs, such as c1,d2" << endl;
  cout << "To trace moves to a file, put filename on the command-line arguments" << endl;
}

tuple<bool, Board::Coord, Board::Coord> parseCoords(const string& input)
{
  if (input[2] == ',')
  {
    char fromRow = input[0];
    int fromCol = input[1] - 48;  // ASCII value of 1 is 49
    char toRow = input[3];
    int toCol = input[4] - 48;

    if (fromRow >= 'a' && fromRow <= 'h' && toRow >= 'a' && toRow <= 'h' && 
      fromCol >= 1 && fromCol <= 8 && toCol >= 1 && toCol <= 8)
    {
      return make_tuple(true, Board::Coord(fromRow, fromCol), Board::Coord(toRow, toCol));
    }
  }

  return make_tuple(false, Board::Coord(-1, -1), Board::Coord(-1,-1));
}

int main(int argc, char* argv[])
{
  ofstream* tracefile = nullptr;

  cout << "Welcome to CyclinderCheckers 0.1" << endl;
  help();

  if (argc > 1)
  {
    tracefile = new ofstream(argv[1]);
  }

  Board board;
  while ( (board.isStalemate() == false) &&
          (board.isPlayerVictory() == -1) )
  {
    ofstream* fileOut = nullptr;

    cout << board.dump() << endl;

    auto input = getPlayerInput();
    // transform input into either command or from/to coords
    if (input == "QUIT" || input == "quit" || input == "q")
    {
      break;
    }
    else if (input == "HELP" || input == "help" || input == "h")
    {
      help();
    }

    tuple<bool, Board::Coord, Board::Coord> move = parseCoords(input);
    if (get<0>(move))
    {
      Board::Coord from = get<1>(move);
      Board::Coord to = get<2>(move);
      if (!board.move(from, to))
      {
        cout << "*** ERROR: Illegal move" << endl;
      }
      else
      {
        (*tracefile) << "board.move(Board::" << ((char)(from.row - 32)) << from.col
          << ",Board::" << ((char)(to.row - 32)) << to.col << ");" << endl;
      }
    }
    else
    {
      cout << "*** INPUT IGNORED" << endl;
    }
  }

  if (tracefile != nullptr)
    delete tracefile;
}
