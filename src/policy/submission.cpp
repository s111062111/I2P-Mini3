#include <cstdlib>

#include "../state/state.hpp"
#include "./submission.hpp"
using namespace std;


/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

//move <value, a or b>
pair<Move, int> AlphaBetaTree(State* root, int player, int depth, int alpha, int beta)
{
    if(!root->legal_actions.size())
        root->get_legal_actions();
    auto actions=root->legal_actions;

    Move move=actions[0];
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
        pair<Move, int> p=AlphaBetaTree(s, s->player, depth-1, alpha, beta);
        
        if(!player){
            if(p.second>alpha){ //???
                move=act;
                alpha=p.second;
            }
            if(alpha>=beta)
                break;
        }
        else if(player){
            if(p.second<beta){
                move=act;
                beta=p.second;
            }
            if(alpha>=beta)
                break;
        }
    }
    if(!player)
        return {move, alpha};
    else if(player)
        return {move, beta};
}

Move AlphaBeta::get_move(State *state, int depth){
  return AlphaBetaTree(state, state->player, depth, -1000000, 1000000).first;
}