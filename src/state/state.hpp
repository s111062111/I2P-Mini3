#ifndef __STATE_H__
#define __STATE_H__

#include <string>
#include <cstdlib>
#include <vector>
#include <utility>

#include "../config.hpp"

typedef std::pair<size_t, size_t> Point;
typedef std::pair<Point, Point> Move;
class Board{
  public:
    char board[2][BOARD_H][BOARD_W] = {{
      //white
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 1, 1, 1, 1},
      {2, 3, 4, 5, 6},
    }, {
      //black
      {6, 5, 4, 3, 2},
      {1, 1, 1, 1, 1},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0},
    }};
};


//Jeremy TEst
const int piece_square_table_pawn[2][BOARD_H][BOARD_W] = {{
    //black
    {0,0,0,0,0},//no pawns
    {50,50,50,50,50},
    {10,20,30,20,10},
    {10,10,20,20,10},
    {5,10,-30,-50,7},
    {0,0,0,0,0},//no pawns
  }, {
    //white
    {0,0,0,0,0},
    {7,-50,-30,10,5},
    {10,20,20,10,10},
    {10,20,30,20,10},
    {50,50,50,50,50},
    {0,0,0,0,0},
  }
};
const int piece_square_table_knight[2][BOARD_H][BOARD_W]={{
    {-50,-40,-30,-40,-50},
    {-40,-15,0,-15,-40},
    {-30,10,20,10,-30},
    {-10,0,5,0,-30},
    {-40,-20,0,2,-40},
    {-50,-40,-30,-40,-50}
  },{
    {-50,-40,-30,-40,-50},
    {-40,2,0,-20,-40},
    {-30,0,5,0,-10},
    {-30,10,20,10,-30},
    {-40,-15,0,-15,-40},
    {-50,-40,-30,-40,-50}
  }
};
const int piece_square_table_bishop[2][BOARD_H][BOARD_W]={{
    {0,-10,0,-10,0},
    {0,0,0,0,0},
    {0,7,0,7,0,},
    {-10,0,10,0,-10},
    {0,5,0,5,0},
    {-20,0,-10,0,-20}
  },{
    {-20,0,-10,0,-20},
    {0,5,0,5,0},
    {-10,0,10,0,-10},
    {0,7,0,7,0,},
    {0,0,0,0,0},
    {0,-10,0,-10,0},
  }
};
const int piece_square_table_rook[2][BOARD_H][BOARD_W]={{
    {0,0,0,0,0},
    {5,10,10,10,5},
    {-5,0,0,0,-5},
    {-5,0,0,0,-5},
    {-5,0,0,0,-5},
    {0,5,5,5,0}
  },{
    {0,5,5,5,0},
    {-5,0,0,0,-5},
    {-5,0,0,0,-5},
    {-5,0,0,0,-5},
    {5,10,10,10,5},
    {0,0,0,0,0},
  }
};
const int piece_square_table_queen[2][BOARD_H][BOARD_W]={{
    {-10,-5,-2,-5,-10},
    {-5,0,0,0,-5},
    {-4,0,2,0,-4},
    {-1,0,2,0,-1},
    {-5,0,5,0,0},
    {-20,-10,-5,-10,-20}
  },{
    {-20,-10,-5,-10,-20},
    {0,5,0,0,-10},
    {-2,0,5,0,-2},
    {-7,0,5,0,-7},
    {-10,0,0,0,-10},
    {-20,-10,-5,-10,-20},
  }
};
const int piece_square_table_king[2][BOARD_H][BOARD_W]={{
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-10,-20,-20,-20,-10},
    {25,15,0,15,25}
  },{
    {25,15,0,15,25},
    {-10,-20,-20,-20,-10},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
    {-30,-40,-50,-40,-30},
  }
};
//


enum GameState {
  UNKNOWN = 0,
  WIN,
  DRAW,
  NONE
};


class State{
  public:
    //You may want to add more property for a state
    GameState game_state = UNKNOWN;
    Board board;
    int player = 0;
    std::vector<Move> legal_actions;
    
    State(){};
    State(int player): player(player){};
    State(Board board): board(board){};
    State(Board board, int player): board(board), player(player){};
    
    int evaluate();
    int Jevaluate();
    State* next_state(Move move);
    void get_legal_actions();
    std::string encode_output();
    std::string encode_state();
};

#endif