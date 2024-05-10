#include "SLR_Class.h"
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <stack>;
State StateFactory::GOTO(State state, int sym) {
    State nextState;
    for (Item item : state.items) {
        if (item.position >= item.p.RHS.size()) continue; // if the dot reach rightmost position then continue
        if (item.p.RHS[item.position] == sym) { // if the next symbol in the production is sym
            Item newItem = { item.position + 1, item.p }; //create new item with the position moved forward
            nextState.items.push_back(newItem); //add new item to the next state
            if (newItem.position == newItem.p.RHS.size()) nextState.isReducible = true; // this line only will not detect the reducible by epsilon
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
            if (item.position >= item.p.RHS.size()) {
                if (item.p.RHS.size() == 0) closedState.isReducible = true; // add condition to detect epsilon derive production
                continue;
            }
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
StateFactory::StateFactory(std::vector<Token> vec){
    tokenVec = vec;
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
    actionTable[1]['$'] = { "accept", 0 };
    for (State s : states) {
        if (s.isReducible) {
            for(Item i : s.items)
                if (i.position == i.p.RHS.size()) {
                    int stateNumber = findState(s);
                    int pNumber = rules.findProductionNumber(i.p);
                    if(symbols.followTable.find(i.p.LHS) != symbols.followTable.end())
                        for(int sym: symbols.followTable[i.p.LHS])
                            actionTable[stateNumber][sym] = { "reduce", pNumber};
                    break;
                }
        }
    }
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
            std::cout << "| Terminal: " << symbol << ", Action: " << action << number << " | ";
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
            std::cout << "| Nonterminal: " << symbol << ", Next State: " << nextState << " | ";
        }
        std::cout << "\n";
    }
}
Token StateFactory::yylex() {
    if (currentToken >= tokens.size()) {
        return { '$',0 };
    }
    if (tokens[currentToken].type == '}')
        int a = 0;
    std::cout << "Token type: " << Symbol::enumToString(tokens[currentToken].type) << ", value: " << tokens[currentToken].value << std::endl;
    return tokens[currentToken++];
}
void StateFactory::yyparse() {
    std::stack<int> stateStack;
    stateStack.push(0);
    Token t = yylex();
    bool correct = false;
    while (1) {
        //std::cout << "loop\n";
        if (actionTable.find(stateStack.top()) != actionTable.end()) {
            if (actionTable[stateStack.top()].find(t.type) != actionTable[stateStack.top()].end()) {
                auto action = actionTable[stateStack.top()][t.type];
                //std::cout << action.first << " " << action.second << "\n";
                if (action.first == "accept") {
                    std::cout << "Accepted\n";
                    correct = true;
                    break;
                }
                else if (action.first == "shift") {
                    stateStack.push(action.second);
                    t = yylex();
                }
                else if (action.first == "reduce") {
                    int reduceProdNumber = action.second;
                    Production reduceProd = rules.productions[reduceProdNumber];
                    std::cout << Symbol::enumToString(reduceProd.LHS) << " -> ";
                    for(auto sym: reduceProd.RHS)
                    {
                        std::cout << Symbol::enumToString(sym) <<" ";
                    }
                    std::cout << std::endl;
                    for (int i = 0; i < reduceProd.RHS.size(); i++)
                        stateStack.pop();
                    if (gotoTable.find(stateStack.top()) != gotoTable.end()) {
                        if (gotoTable[stateStack.top()].find(reduceProd.LHS) != gotoTable[stateStack.top()].end()) {
                            stateStack.push(gotoTable[stateStack.top()][reduceProd.LHS]);
                        }
                        else {
                            //std::cout << "Could not find goto1\n";
                            break;
                        }
                    }
                    else {
                        //std::cout << "Could not find goto2\n";
                        break;
                    }
                }
            }
            else {
                //std::cout << "Could not find action1\n";
                break;
            }
        }
        else {
            //std::cout << "Could not find action2\n";
            break;
        }
    }
    if (correct)
        std::cout << "Parse success.\n";
    else
        std::cout << "Parse failed.\n";
}