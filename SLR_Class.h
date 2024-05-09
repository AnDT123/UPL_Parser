#include <vector>
#include <map>
#include <string>
#pragma once
class Symbol {
public:
    static enum NonTerminal{
        program = 276,
        stmtlist = 277,
        block = 278,
        stmt = 279,
        decl_stmt = 280,
        assign_stmt = 281,
        ctrl_stmt = 282,
        loop_stmt = 283,
        decl_type = 284,
        decl_body = 285,
        e = 286,
        cond = 287,
        rop = 288,
        ctrl_stmt1 = 289,
        t = 290,
        f = 291
    };
	static enum Terminal {
        START = 258,
        END = 259,
        IF = 260,
        THEN = 261,
        ELSE = 262,
        PLUS = 263,
        TIMES = 264,
        INT = 265,
        BOOL = 266,
        DO = 267,
        WHILE = 268,
        PRINT = 269,
        GT = 270,
        GE = 271,
        EQ = 272,
        ASSIGN = 273,
        ID = 274,
        INT_NUM = 275
    };
    static bool isNonTerminal(int i) {
        return (i >=276) && (i <= 291);
    }
    static bool isTerminal(int i) {
        return ((i >=258) && (i <= 275) ) || (i == '(' || i ==')' || i == ';' || i == '{' || i == '}');
    }
    std::map<int, std::vector<int>> followTable;
    Symbol() {
        // follow(program) = {$}
        std::vector<int> follow0{ '$' };
        followTable[program] = follow0;

        // follow(block) = {END,},{,ID,PRINT,IF,DO,INT,BOOL,ELSE,WHILE}
        std::vector<int> follow1{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL, ELSE, WHILE };
        followTable[block] = follow1;

        // follow(stmtlist) = {END,}}
        std::vector<int> follow2{ END, '}' };
        followTable[stmtlist] = follow2;

        // follow(stmt) = {END,},{,ID,PRINT,IF,DO,INT,BOOL}
        std::vector<int> follow3{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL };
        followTable[stmt] = follow3;

        // follow(decl_stmt) = {END,},{,ID,PRINT,IF,DO,INT,BOOL}
        std::vector<int> follow4{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL };
        followTable[decl_stmt] = follow4;

        // follow(decl_body) = {;}
        std::vector<int> follow5{ ';' };
        followTable[decl_body] = follow5;

        // follow(decl_type) = {ID}
        std::vector<int> follow6{ID};
        followTable[decl_type] = follow6;

        // follow(assign_stmt) = {END,},{,ID,PRINT,IF,DO,INT,BOOL}
        std::vector<int> follow7{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL };
        followTable[assign_stmt] = follow7;

        // follow(ctrl_stmt) = {END,},{,ID,PRINT,IF,DO,INT,BOOL}
        std::vector<int> follow8{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL };
        followTable[ctrl_stmt] = follow8;

        // follow(cond) = {THEN,),;,PLUS}
        std::vector<int> follow9{ THEN, ')', ';', PLUS };
        followTable[cond] = follow9;

        // follow(rop) = {ID,INT_NUM}
        std::vector<int> follow10{ ID, INT_NUM };
        followTable[rop] = follow10;

        // follow(ctrl_stmt1) = {END,},{,ID,PRINT,IF,DO,INT,BOOL}
        std::vector<int> follow11{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL };
        followTable[ctrl_stmt1] = follow11;

        // follow(loop_stmt) = {END,},{,ID,PRINT,IF,DO,INT,BOOL}
        std::vector<int> follow12{ END, '}', '{', ID, PRINT, IF, DO, INT, BOOL };
        followTable[loop_stmt] = follow12;

        // follow(e) = {;,PLUS}
        std::vector<int> follow13{ ';', PLUS };
        followTable[e] = follow13;

        // follow(t) = {;,PLUS,TIMES}
        std::vector<int> follow14{ ';', PLUS, TIMES };
        followTable[t] = follow14;

        // follow(f) = {GT,GE,EQ,THEN,;,PLUS,TIMES,)}
        std::vector<int> follow15{ GT, GE, EQ, THEN, ';', PLUS, TIMES, ')' };
        followTable[f] = follow15;

    }
};
class Production {
public:
    int LHS;
    std::vector<int> RHS;
    Production(int LHS, std::vector<int>RHS) : LHS(LHS), RHS(RHS) {}
    bool operator==(const Production& other) const {
        if (other.LHS != LHS) return false;
        if (other.RHS.size()!= RHS.size()) return false;
        for (size_t i = 0; i < RHS.size(); ++i) {
            if (RHS[i] != other.RHS[i])
                return false;
        }
        return true;
    }
};
class Rules {
public:
    std::vector<Production> productions;
    Rules() {
        //program-> START stmtlist END 
        int lhs0 = Symbol::program;
        std::vector<int> rhs0{Symbol::START,Symbol::stmtlist, Symbol::END }; 
        productions.push_back(Production(lhs0, rhs0));

        //block-> { stmtlist }
        int lhs1 = Symbol::block;
        std::vector<int> rhs1{'{',Symbol::stmtlist, '}'};
        productions.push_back(Production(lhs1, rhs1));

        //stmtlist-> ''
        int lhs2 = Symbol::stmtlist;
        std::vector<int> rhs2;
        productions.push_back(Production(lhs2, rhs2));

        //stmtlist-> stmt stmtlist  
        int lhs3 = Symbol::stmtlist;
        std::vector<int> rhs3{ Symbol::stmt, Symbol::stmtlist };
        productions.push_back(Production(lhs3, rhs3));

        //stmt-> decl_stmt     
        int lhs4 = Symbol::stmt;
        std::vector<int> rhs4{ Symbol::decl_stmt };
        productions.push_back(Production(lhs4, rhs4));

        //stmt-> assign_stmt  
        int lhs5 = Symbol::stmt;
        std::vector<int> rhs5{ Symbol::assign_stmt };
        productions.push_back(Production(lhs5, rhs5));

        //stmt-> ctrl_stmt    
        int lhs6 = Symbol::stmt;
        std::vector<int> rhs6{ Symbol::ctrl_stmt };
        productions.push_back(Production(lhs6, rhs6));

        //stmt-> loop_stmt     
        int lhs7 = Symbol::stmt;
        std::vector<int> rhs7{ Symbol::loop_stmt };
        productions.push_back(Production(lhs7, rhs7));

        //stmt-> block         
        int lhs8 = Symbol::stmt;
        std::vector<int> rhs8{ Symbol::block };
        productions.push_back(Production(lhs8, rhs8));

        //decl_stmt->  decl_type  decl_body ; 
        int lhs9 = Symbol::decl_stmt;
        std::vector<int> rhs9{ Symbol::decl_type, Symbol::decl_body, ';' };
        productions.push_back(Production(lhs9, rhs9));

        //decl_body-> ID  
        int lhs10 = Symbol::decl_body;
        std::vector<int> rhs10{ Symbol::ID };
        productions.push_back(Production(lhs10, rhs10));

        //decl_body-> ID ASSIGN e    
        int lhs11 = Symbol::decl_body;
        std::vector<int> rhs11{ Symbol::ID, Symbol::ASSIGN, Symbol::e };
        productions.push_back(Production(lhs11, rhs11));

        //decl_type->  INT
        int lhs12 = Symbol::decl_type;
        std::vector<int> rhs12{ Symbol::INT };
        productions.push_back(Production(lhs12, rhs12));

        //decl_type-> BOOL   
        int lhs13 = Symbol::decl_type;
        std::vector<int> rhs13{ Symbol::BOOL };
        productions.push_back(Production(lhs13, rhs13));

        //assign_stmt -> ID ASSIGN e ;  
        int lhs14 = Symbol::assign_stmt;
        std::vector<int> rhs14{ Symbol::ID, Symbol::ASSIGN, Symbol::e, ';' };
        productions.push_back(Production(lhs14, rhs14));

        //assign_stmt-> PRINT e ;  
        int lhs15 = Symbol::assign_stmt;
        std::vector<int> rhs15{ Symbol::PRINT, Symbol::e, ';' };
        productions.push_back(Production(lhs15, rhs15));

        //ctrl_stmt-> IF cond ctrl_stmt1    
        int lhs16 = Symbol::ctrl_stmt;
        std::vector<int> rhs16{ Symbol::IF, Symbol::cond, Symbol::ctrl_stmt1 };
        productions.push_back(Production(lhs16, rhs16));

        //cond-> f rop f 
        int lhs17 = Symbol::cond;
        std::vector<int> rhs17{ Symbol::f, Symbol::rop, Symbol::f };
        productions.push_back(Production(lhs17, rhs17));

        //rop-> GT      
        int lhs18 = Symbol::rop;
        std::vector<int> rhs18{ Symbol::GT };
        productions.push_back(Production(lhs18, rhs18));

        //rop-> GE    
        int lhs19 = Symbol::rop;
        std::vector<int> rhs19{ Symbol::GE };
        productions.push_back(Production(lhs19, rhs19));

        //rop-> EQ    
        int lhs20 = Symbol::rop;
        std::vector<int> rhs20{ Symbol::EQ };
        productions.push_back(Production(lhs20, rhs20));

        //ctrl_stmt1-> THEN block    
        int lhs21 = Symbol::ctrl_stmt1;
        std::vector<int> rhs21{ Symbol::THEN, Symbol::block };
        productions.push_back(Production(lhs21, rhs21));

        //ctrl_stmt1-> THEN block ELSE block 
        int lhs22 = Symbol::ctrl_stmt1;
        std::vector<int> rhs22{ Symbol::THEN, Symbol::block, Symbol::ELSE, Symbol::block };
        productions.push_back(Production(lhs22, rhs22));

        //loop_stmt->  DO block WHILE ( cond ) ; 
        int lhs23 = Symbol::loop_stmt;
        std::vector<int> rhs23{ Symbol::DO, Symbol::block, Symbol::WHILE, '(', Symbol::cond, ')', ';' };
        productions.push_back(Production(lhs23, rhs23));

        //e-> e PLUS t  
        int lhs24 = Symbol::e;
        std::vector<int> rhs24{ Symbol::e, Symbol::PLUS, Symbol::t };
        productions.push_back(Production(lhs24, rhs24));

        //e-> t        
        int lhs25 = Symbol::e;
        std::vector<int> rhs25{ Symbol::t };
        productions.push_back(Production(lhs25, rhs25));

        //e-> cond      
        int lhs26 = Symbol::e;
        std::vector<int> rhs26{ Symbol::cond };
        productions.push_back(Production(lhs26, rhs26));

        //t-> t TIMES f 
        int lhs27 = Symbol::t;
        std::vector<int> rhs27{ Symbol::t, Symbol::TIMES, Symbol::f };
        productions.push_back(Production(lhs27, rhs27));

        //t-> f        
        int lhs28 = Symbol::t;
        std::vector<int> rhs28{ Symbol::f };
        productions.push_back(Production(lhs28, rhs28));

        //f-> ID        
        int lhs29 = Symbol::f;
        std::vector<int> rhs29{ Symbol::ID };
        productions.push_back(Production(lhs29, rhs29));

        //f-> INT_NUM  
        int lhs30 = Symbol::f;
        std::vector<int> rhs30{ Symbol::INT_NUM };
        productions.push_back(Production(lhs30, rhs30));
    }
    int findProductionNumber(Production p) {
        auto it = std::find(productions.begin(), productions.end(), p);
        if (it != productions.end()) {
            return std::distance(productions.begin(), it);
        }
        else {
            return -1;
        }
    }
};
struct Item {
    int position;
    Production p;
    bool operator==(const Item& other) const {
        
        return (position == other.position) && (p == other.p);
    }
};
class State {
public:
    bool isReducible = false;
    std::vector<Item> items;
    bool operator==(const State& other) const {
        if (items.size() != other.items.size()) return false;
        for (size_t i = 0; i < items.size(); ++i) {
            bool found = false;
            for (size_t j = 0; j < items.size(); ++j) {
                if (items[i] == other.items[j]) {
                    found = true;
                    break;
                }
            }
            if (!found)
                return false;
        }
        return true;
    }
};
class StateFactory {
private:
    Rules rules;
    Symbol symbols;
    std::vector<State> states;
    std::map<int, std::map<int, std::pair<std::string, int>>> actionTable;
    std::map<int, std::map<int, int>> gotoTable;
public:
    StateFactory();
    void printTables();
    int findState(const State state);
    void addState(const State state);
    State GOTO(State state, int sym);
    State closure(State s);
};