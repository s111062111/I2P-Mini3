#include <cstdlib>

#include "../state/state.hpp"
#include "./greedy.hpp"
#include <iostream>

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */
Move Greedy::get_move(State *state, int depth){
  if(!state->legal_actions.size())
    state->get_legal_actions();
  
  auto actions = state->legal_actions;

  int max=-1000000;
  int val=state->evaluate();

  Move next_move;
  for(auto act:actions){
    int value=state->next_state(act)->evaluate();
    if(value>max){
        max=value;
        next_move=act;
    }
  }

  return next_move;
}