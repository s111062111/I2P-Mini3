#include <cstdlib>

#include "../state/state.hpp"
#include "./minimax.hpp"
using namespace std;


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */


pair<Move, int> MinMaxTree(State* root, int player, int depth)
{
    int max=-10000000, min=10000000, value;
    if(!root->legal_actions.size())
        root->get_legal_actions();
    auto actions=root->legal_actions;

    Move move={{0, 0}, {0, 0}};
    if(!depth)
        return {move, root->evaluate()};
    else if(root->game_state==WIN){
        if(!player) return {move, 10000000};
        else if(player) return {move, -10000000}; 
    }
    else if(actions.empty()){
        return {move, root->evaluate()};
    }

    for(auto act: actions){
        State* s=root->next_state(act);
        pair<Move, int> p=MinMaxTree(s, s->player, depth-1);
        
        if(!player){
            if(p.second>max){ //???
                move=act;
                value=p.second;
                max=p.second;
            }
        }
        else if(player){
            if(p.second<min){
                move=act;
                value=p.second;
                min=p.second;
            }
        }
    }
    return {move, value};
}

Move MinMax::get_move(State *state, int depth){
  return MinMaxTree(state, state->player, 4).first;
}