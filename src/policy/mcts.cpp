#include <cstdlib>

#include "../state/state.hpp"
#include "./mcts.hpp"
#include <vector>
#include <math.h>

/**
 * @brief Randomly get a legal action
 * 
 * @param state Now state
 * @param depth You may need this for other policy
 * @return Move 
 */

double UCB(double wi, double ni, double Ni)
{
    return wi/ni + pow(2, 0.5)*pow(log(Ni)/ni, 0.5);
}

struct node{
    int wi=0, ni=0;
    Move move;
    State* state;
    bool updated=false;
    node* parent=nullptr;
    std::vector<node*> children;

    node(State* s):state(s){}
};

node* MCTSRoot;

void BackPropagate(node* nd, int wplus, int nplus)
{
    nd->wi+=wplus;
    nd->ni+=nplus;
    while(nd->parent!=nullptr){
        nd->parent->wi+=wplus;
        nd->parent->ni+=nplus;
        nd=nd->parent;
    }
}

node* Select(node* root)
{
    if(root->children.empty()){
        return root;
    }

    double MaxUCB=-1000000000, ucb;
    node* select;
    for(auto children:root->children){
        if(root->ni==0){
            return Select(children);
        }
        if(!root->state->player)
            ucb=UCB(root->wi, root->ni, root->parent->ni);
        else if(root->state->player)
            ucb=UCB(root->ni - root->wi, root->ni, root->parent->ni);
        if(ucb>MaxUCB){
            select=children;
            MaxUCB=ucb;
        }
    }
    return Select(select);
}

void Rollout(node* nd)
{
    int steps=50;
    State s=*(nd->state);
    while(steps--){
        if(s.game_state==WIN){
            if(!s.player)
                BackPropagate(nd, 1, 1);
            else if(s.player)
                BackPropagate(nd, 0, 1);
            return;
        }

        if(!s.legal_actions.size())
            s.get_legal_actions();
  
        auto actions = s.legal_actions;
        Move act=actions[rand()%actions.size()];
        s=*(s.next_state(act));
    }
    int value=s.evaluate();
    if(value>0){
        BackPropagate(nd, 1, 1);
        return;
    }
    else if(value<=0){
        BackPropagate(nd, 0, 1);
        return;
    }
}

void Expand(node* root)
{
    if(!root->state->legal_actions.size())            
        root->state->get_legal_actions();
    auto actions = root->state->legal_actions;
    for(auto act:actions){
        node* child=new node(root->state->next_state(act));
        child->parent=root;
        child->move=act;
        root->children.push_back(child);
    }
}

void Free(node* root)
{
    if(root->children.empty())
        delete root;

    for(auto child:root->children){
        Free(child);
        delete root;
    }
}

Move MCTS::get_move(State *state, int depth){
    node* select;
    int simulation=3;
    double max=0;

    MCTSRoot=new node(state);
    MCTSRoot->updated=true;
    MCTSRoot->move={{0, 0}, {0, 0}};
    while(simulation--){    
        select=Select(MCTSRoot);
        if(!select->updated){
            select->updated=true;
            Rollout(select);
        }
        else if(select->updated){
            Expand(select);
        }
    }
    if(!state->legal_actions.size())            
        state->get_legal_actions();
    auto actions = state->legal_actions;
    Move move=actions[0];
    for(auto nd:MCTSRoot->children){
        if(!nd->ni) continue;
        if(nd->wi/nd->ni > max){
            max=nd->wi/nd->ni;
            move=nd->move;
        }
    }
    Free(MCTSRoot);

    return move;
}