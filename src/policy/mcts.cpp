#include <cstdlib>

#include "../state/state.hpp"
#include "./mcts.hpp"
#include <vector>
#include <math.h>
#include <iostream>

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

inline double UCB(int wi, int ni, int Ni)
{
    if(!ni)
        return 1000000;

    return (double)wi/ni + 1.414214*sqrt(log(Ni)/ni);
}

struct node{
    int wi=0, ni=0;
    int step;
    Move move;
    State* state;
    bool updated=false;
    node* parent=nullptr;
    std::vector<node*> children;

    node(State* s):state(s){}
};

void BackPropagate(node* nd, int wplus)
{
    nd->wi+=wplus;
    nd->ni++;
    while(nd->parent!=nullptr){
        nd->parent->wi+=wplus;
        nd->parent->ni++;
        nd=nd->parent;
    }
}

node* Select(node* root)
{
    node* rt=root;
    while(!rt->children.empty()){
        node* select=rt;
        double MaxUCB=-1, ucb;
        for(auto& children:rt->children){
            if(!children->state->player)
                ucb=UCB(children->wi, children->ni, rt->ni);
            else
                ucb=UCB(children->ni - children->wi, children->ni, rt->ni);
            if(ucb>MaxUCB){
                select=children;
                MaxUCB=ucb;
            }
        }
        rt=select;
    }
    return rt;
}

void Rollout(node* nd)
{
    int step=nd->step;
    if(step<=0) return;
    State* s=nd->state;
    while(true){
        if(s->game_state==WIN){
            if(!s->player)
                BackPropagate(nd, 1);
            else 
                BackPropagate(nd, 0);
            return;
        }
        else if(s->game_state==DRAW){
            BackPropagate(nd, 0);
            return;
        }
        else if(s->legal_actions.empty()){
            if(!s->player)
                BackPropagate(nd, 0);
            else 
                BackPropagate(nd, 1);
            return;
        }
  
        auto& actions = s->legal_actions;
        Move act=actions[rand()%actions.size()];
        s=s->next_state(act);
    }
    int value=s->evaluate();
    if(value>0){
        BackPropagate(nd, 1);
        return;
    }
    else{
        BackPropagate(nd, 0);
        return;
    }
}

void Expand(node* root)
{
    auto& actions = root->state->legal_actions;

    if(root->state->game_state==WIN){
        if(!root->state->player){
            BackPropagate(root, 1);
            return;
        }
        else{
            BackPropagate(root, 0);
            return;
        }
    }
    else if(root->state->game_state==DRAW){
        BackPropagate(root, 0);
        return;
    }
    else if(actions.empty()){
        if(!root->state->player){
            BackPropagate(root, 0);
            return;
        }
        else{
            BackPropagate(root, 1);
            return;
        }
    }

    for(auto& act:actions){
        State* s=root->state->next_state(act);
        node* child=new node(s);
        child->parent=root;
        child->move=act;
        child->step=root->step-1;
        root->children.emplace_back(child);
    }
    root->children[0]->updated=true;
    Rollout(root->children[0]);
}

void Free(node* root)
{
    if(root->children.empty()){
        return;
    }

    for(auto child:root->children){
        Free(child);
        delete child;
    }
}

Move MCTS::get_move(State *state, int depth){
    
    int simulation=100000;
    double max=-1, min=10000000;
    node* MCTSRoot;
    node* select;
    MCTSRoot=new node(state);
    MCTSRoot->updated=true;
    MCTSRoot->step=50;

    while(simulation--){  
        select=Select(MCTSRoot);
        if(!select->updated){
            select->updated=true;
            Rollout(select);
        }
        else{
            Expand(select);
        }
    }
    if(!state->legal_actions.size())            
        state->get_legal_actions();
    auto actions = state->legal_actions;
    Move Maxmove=actions[0];
    Move Minmove=actions[0];

    for(auto& nd:MCTSRoot->children){
        if(nd->ni==0) continue;
        if((double)nd->wi/nd->ni > max){
            max=nd->wi/nd->ni;
            Maxmove=nd->move;
        }
        if((double)nd->wi/nd->ni < min){
            min=nd->wi/nd->ni;
            Minmove=nd->move;
        }
    }
    //Free(MCTSRoot);
    if(!state->player)
        return Maxmove;
    else
        return Minmove;
}