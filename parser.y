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
SyntaxTree *root;
int currentToken = 0;
int yylex() {
    if (currentToken >= tokens.size()) {
        return 0;
    }
	std::cout << "Token type: " << tokens[currentToken].type << ", value: " << tokens[currentToken].value << std::endl;
    if(tokens[currentToken].type == INT_NUM)
        yylval.i = tokens[currentToken].value;
    return tokens[currentToken++].type;
}
void yyerror(const char *s) {
  printf("%s\n", s);
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

program: START stmtlist END { root = newTree("stmtlist", $2, nullptr); 
                                $$ = root;} 
        | error         { $$ = newTree("error", nullptr, nullptr); } 
;
block : '{' stmtlist '}'    { $$ = newTree("stmtlist", $2, nullptr); } 
        | '{' error '}'     { $$ = newTree("error", nullptr, nullptr); } 
;
stmtlist:   { $$ = newTree("epsilon", nullptr, nullptr); } 
        | stmt stmtlist  { $$ = newTree("stmtlist", $1, $2); } 
;
stmt: decl_stmt     { $$ = newTree("decl_stmt", $1, nullptr); } 
    | assign_stmt   { $$ = newTree("assign_stmt", $1, nullptr); } 
    | ctrl_stmt     { $$ = newTree("ctrl_stmt", $1, nullptr); } 
    | loop_stmt     { $$ = newTree("loop_stmt", $1, nullptr); } 
    | block         { $$ = newTree("block", $1, nullptr); } 
    | error ';'     { $$ = newTree("error", nullptr, nullptr); } 
;
decl_stmt:  decl_type  decl_body ';' { $$ = newTree("decl", $1, $2); } 
;
decl_body: ID  { $$ = newLeaf(INT_NUM); }
            | ID ASSIGN e   { $$ = newTree("decl_assign", newLeaf(INT_NUM), $3); } 
;
decl_type:  INT     { $$ = newTree("int_type", nullptr, nullptr); }
            | BOOL   { $$ = newTree("bool_type", nullptr, nullptr); }
;
assign_stmt : ID ASSIGN e ';'   { $$ = newTree("assign", newLeaf(INT_NUM), $3); }
        | PRINT e ';'   { $$ = newTree("print", $2, nullptr); }
;
ctrl_stmt   : IF cond ctrl_stmt1    { $$ = newTree("if", $2, $3); }
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
;
e   :     e PLUS t  { $$ = newTree("+", $1,$3); }
        | t         { $$ = newTree("t", $1, nullptr); }
        | cond      { $$ = newTree("cond", $1, nullptr); }
;
t   :     t TIMES f { $$ = newTree("*", $1, $3); }
        | f         { $$ = newTree("f", $1, nullptr); }
;
f   :     ID        { $$ = newLeaf(INT_NUM); }
        | INT_NUM   { $$ = newLeaf($1); }
        | '(' e ')' { $$ = newTree("e", $2, nullptr); }
        | '{' error '}' { $$ = newTree("error", nullptr, nullptr); } 
;
%%