#include "SLR_Class.h"
#include <map>
#include <vector>
#include <queue>
#include <iostream>

State StateFactory::GOTO(State state, int sym) {
    State nextState;
    for (Item item : state.items) {
        if (item.position >= item.p.RHS.size()) continue; // if the dot reach rightmost position then continue
        if (item.p.RHS[item.position] == sym) { // if the next symbol in the production is sym
            Item newItem = { item.position + 1, item.p }; //create new item with the position moved forward
            nextState.items.push_back(newItem); //add new item to the next state
            if (newItem.position == newItem.p.RHS.size()) nextState.isReducible = true;
        }
    }
    return closure(nextState); // return the closure of the moved items inside the nextState
}
State StateFactory::closure(State s) {
    State closedState = s;
    bool changed;
    do {
        changed = false;
        for (Item item : closedState.items) { //loop throught state's items
            if (item.position >= item.p.RHS.size()) continue;
            if (Symbol::isNonTerminal(item.p.RHS[item.position])) { // add productions for non-terminals right after the dot
                for (Production prod : rules.productions) {  // loop through every production
                    if (prod.LHS == item.p.RHS[item.position]) { // if production's LHS == Nonterminal right after the dot
                        Item newItem = { 0, prod }; // create a new item for that production
                        // if the item is not already in the state
                        if (std::find(closedState.items.begin(), closedState.items.end(), newItem) == closedState.items.end()) {
                            closedState.items.push_back(newItem);   // Add the new item to the state
                            changed = true;
                        }
                    }
                }
            }
        }
    } while (changed);
    return closedState;
}
int StateFactory::findState(State state) {
    auto it = std::find(states.begin(), states.end(), state);
    if (it != states.end()) {
        return std::distance(states.begin(), it);
    }
    else {
        return -1;
    }
}
void StateFactory::addState(State state) {
    int pos = findState(state);
    if (pos == -1) {
        states.push_back(state);
    }
}
StateFactory::StateFactory(){
    std::vector<int> inputs;
    for (int i = 276; i <= 291; i++)
        inputs.push_back(i);
    for (int i = 258; i <= 275; i++)
        inputs.push_back(i);
    inputs.push_back('(');
    inputs.push_back(')');
    inputs.push_back(';');
    inputs.push_back('{');
    inputs.push_back('}');



    State startState;
    startState.items.push_back(Item{ 0, Production{'@', {Symbol::program}}});
    startState = closure(startState);
    addState(startState);
    std::queue<State> stateQueue;
    stateQueue.push(startState);

    while (!stateQueue.empty()) {
        State frontState = stateQueue.front();
        stateQueue.pop();
        int frontQueueStateNumber = findState(frontState);

        for (int sym: inputs) {
            State gotoState = GOTO(frontState, sym);
            if (!gotoState.items.empty()) {
                int nextStateNumber = findState(gotoState);
                if (nextStateNumber == -1)
                {
                    stateQueue.push(gotoState);
                    addState(gotoState);
                    nextStateNumber = findState(gotoState);
                }
                if (Symbol::isNonTerminal(sym))
                    gotoTable[frontQueueStateNumber][sym] = nextStateNumber;
                else if(Symbol::isTerminal(sym))
                    actionTable[frontQueueStateNumber][sym] = { "shift", nextStateNumber };
            }
        }
    }

    for (State s : states) {
        if (s.isReducible) {
            for(Item i : s.items)
                if (i.position == i.p.RHS.size()) {
                    int stateNumber = findState(s);
                    int pNumber = rules.findProductionNumber(i.p);
                    for(int sym: symbols.followTable[i.p.LHS])
                        actionTable[stateNumber][sym] = { "reduce", pNumber};
                    break;
                }
        }
    }
    printTables();

}
void StateFactory::printTables() {
    std::cout << "Action Table:\n";
    for (const auto& stateActionPair : actionTable) {
        int state = stateActionPair.first;
        std::cout << "State " << state << ": ";
        for (const auto& actionPair : stateActionPair.second) {
            int symbol = actionPair.first;
            std::string action = actionPair.second.first;
            int number = actionPair.second.second;
            std::cout << "| Symbol: " << symbol << ", Action: " << action << ", Number: " << number << " | ";
        }
        std::cout << "\n";
    }

    std::cout << "\nGoto Table:\n";
    for (const auto& stateGotoPair : gotoTable) {
        int state = stateGotoPair.first;
        std::cout << "State " << state << ": ";
        for (const auto& gotoPair : stateGotoPair.second) {
            int symbol = gotoPair.first;
            int nextState = gotoPair.second;
            std::cout << "| Symbol: " << symbol << ", Next State: " << nextState << " | ";
        }
        std::cout << "\n";
    }
}
