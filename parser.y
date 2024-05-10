%{
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <FlexLexer.h>
#include <vector>
#include "Common.h"
#include "parser.tab.h"
//#include "testxxLexer.flex.cpp"
extern std::vector<Token> tokens;
extern SyntaxTree* newTree(std::string nodetype, SyntaxTree* l, SyntaxTree* r);
extern SyntaxTree* newLeaf(int i);
extern std::vector<std::pair<int, int>> lineMap;
bool parseSuccess = true;
SyntaxTree *root;
int currentToken = 0;
int yylex() {
    if (currentToken >= tokens.size()) {
        return 0;
    }
	//std::cout << "Token type: " << tokens[currentToken].type << ", value: " << tokens[currentToken].value << std::endl;
    if(tokens[currentToken].type == INT_NUM)
        yylval.i = tokens[currentToken].value;
    return tokens[currentToken++].type;
}
void yyerror(const char *s) {
  //printf("%s\n", s);
}
int tokenLineSearch(int tokenNumber){
    for (auto p : lineMap) {
        if (p.first >= tokenNumber) return p.second - (p.first == tokenNumber ? 0 : 1);
    }
    return -1;
}
%}
%union {
    SyntaxTree *st;
    int i;
}
%token START END IF THEN ELSE PLUS TIMES INT BOOL DO WHILE 
%token PRINT GT GE EQ ASSIGN ID INT_NUM
%type <st> program stmtlist block stmt decl_stmt decl_type decl_body assign_stmt ctrl_stmt loop_stmt cond rop ctrl_stmt1 e t f
%type <i> INT_NUM

%left PLUS 
%left TIMES 

%start program
%%

program: START stmtlist END { root = newTree("program", $2, nullptr); 
                                $$ = root;} 
        | error { $$ = newTree("some_error", nullptr, nullptr); 
                                std::cout << "some terrible error at line " <<  tokenLineSearch(currentToken) << '\n';  
                                parseSuccess = false;
                            }        
        | START stmtlist error { $$ = newTree("error_missing_end", nullptr, nullptr); 
            std::cout << "missing end at line " <<  tokenLineSearch(currentToken) << '\n';  
            parseSuccess = false;
        }
;
block : '{' stmtlist '}'    { $$ = newTree("stmtlist", $2, nullptr); } 
        |'{' stmtlist error { $$ = newTree("error_missing_close_curly_bracket", $2, nullptr); 
                                std::cout << "Missing } at line " <<  tokenLineSearch(currentToken) << '\n';  
                                parseSuccess = false;
                            }
;
stmtlist:   { $$ = newTree("epsilon", nullptr, nullptr); } 
        | stmt stmtlist  { $$ = newTree("stmtlist", $1, $2); } 
;
stmt: decl_stmt     { $$ = newTree("decl_stmt", $1, nullptr); } 
    | assign_stmt   { $$ = newTree("assign_stmt", $1, nullptr); } 
    | ctrl_stmt     { $$ = newTree("ctrl_stmt", $1, nullptr); } 
    | loop_stmt     { $$ = newTree("loop_stmt", $1, nullptr); } 
    | block         { $$ = newTree("block", $1, nullptr); } 
;
decl_stmt:  decl_type  decl_body ';' { $$ = newTree("decl", $1, $2); } 
            | decl_type  decl_body error  { $$ = newTree("error_missing_semicolon", nullptr, nullptr); 
                                            std::cout << "Missing ; at line " <<  tokenLineSearch(currentToken) << '\n';  
                                            parseSuccess = false;
                                          }
;
decl_body: ID  { $$ = newTree("ID", nullptr,nullptr); }
            | ID ASSIGN e   { $$ = newTree("decl_assign", newTree("ID", nullptr,nullptr), $3); } 
;
decl_type:  INT     { $$ = newTree("int_type", nullptr, nullptr); }
            | BOOL   { $$ = newTree("bool_type", nullptr, nullptr); }
;
assign_stmt : ID ASSIGN e ';'   { $$ = newTree("assign", newTree("ID", nullptr,nullptr), $3); }
        | PRINT e ';'   { $$ = newTree("print", $2, nullptr); }
        | ID ASSIGN e error   { $$ = newTree("error_missing_semicolon", nullptr, nullptr); 
                                std::cout << "Missing ; at line " <<  tokenLineSearch(currentToken) << '\n';  
                                parseSuccess = false;
                            }
        | PRINT e error   { $$ = newTree("error_missing_semicolon", nullptr, nullptr); 
                            std::cout << "Missing ; at line " <<  tokenLineSearch(currentToken) << '\n';  
                            parseSuccess = false;
                          }
;
ctrl_stmt   : IF cond ctrl_stmt1    { $$ = newTree("if", $2, $3); }
            | IF error ctrl_stmt1 { $$ = newTree("error_missing_cond", nullptr, nullptr); 
                                    std::cout << "Missing condition op at line " <<  tokenLineSearch(currentToken) << '\n';                                       
                                }
;
cond:   f rop f { $$ = newTree("rop", $1, $3); }
;
rop :   GT      { $$ = newTree(">", nullptr, nullptr); }
        | GE    { $$ = newTree(">=",nullptr, nullptr); }
        | EQ    { $$ = newTree("==", nullptr, nullptr); }
;
ctrl_stmt1  : THEN block    { $$ = newTree("then", $2, nullptr); }
            | THEN block ELSE block { $$ = newTree("then_else", $2,$4); }
;
loop_stmt:  DO block WHILE '(' cond ')' ';' { $$ = newTree("do_while", $2,$5); }
            | DO block WHILE '(' cond ')' error { $$ = newTree("error_missing_semicolon", nullptr, nullptr); 
                                                  std::cout << "Missing ; at line " <<  tokenLineSearch(currentToken) << '\n';                                       
                                                }
            | DO block WHILE '(' cond error ';' { $$ = newTree("error_missing_close_round_bracket", nullptr, nullptr); 
                                                  std::cout << "Missing ) at line " <<  tokenLineSearch(currentToken) << '\n';                                       
                                                }
;
e   :     e PLUS t  { $$ = newTree("+", $1,$3); }
        | t         { $$ = newTree("t", $1, nullptr); }
        | cond      { $$ = newTree("cond", $1, nullptr); }
;
t   :     t TIMES f { $$ = newTree("*", $1, $3); }
        | f         { $$ = newTree("f", $1, nullptr); }
;
f   :     ID        { $$ = newTree("ID", nullptr, nullptr); }
        | INT_NUM   { $$ = newTree("INT_NUM", nullptr,nullptr); }
        | '(' e ')' { $$ = newTree("e", $2, nullptr); }
        | '(' e error { $$ = newTree("error_missing_close_round_bracket", $2, nullptr); 
                          std::cout << "Missing ) at line " <<  tokenLineSearch(currentToken) << '\n';                                                               
                        }
;
%%