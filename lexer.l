	// An example of using the flex C++ scanner class.

%option C++ noyywrap

%{
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include "Common.h"
#include "parser.tab.h"
std::vector<Token> tokens;
std::vector<std::pair<int, int>> lineMap;

int mylineno = 0;
%}

string	\"[^\n"]+\"

ws	[ \t]+

letter 	[A-Za-z]
digit  	[0-9]
%%

{ws}	/* skip blanks and tabs */

"/*"		{
		int c;

		while((c = yyinput()) != 0)
			{
			if(c == '\n'){
				++mylineno;
				lineMap.push_back({tokens.size(), mylineno});
			}

			else if(c == '*')
				{
				if((c = yyinput()) == '/')
					break;
				else
					unput(c);
				}
			}
		}
"//" {
		int c;

		while((c = yyinput()) != 0)
		{
			if(c == '\n'){
				++mylineno;
				lineMap.push_back({tokens.size(), mylineno});
				break;
			}
		}
	}
\n		{	mylineno++;
			lineMap.push_back({tokens.size(), mylineno});
		}

"begin" { Token t; t.type = START; t.value = START; tokens.push_back(t);}
"end" { Token t; t.type = END; t.value = END; tokens.push_back(t); }
";" { Token t; t.type = ';'; t.value = ';'; tokens.push_back(t);}
"if" { Token t; t.type = IF; t.value = IF; tokens.push_back(t);}
"then" { Token t; t.type = THEN; t.value = THEN; tokens.push_back(t);}
"else" { Token t; t.type = ELSE; t.value = ELSE; tokens.push_back(t);}
"+" { Token t; t.type = PLUS; t.value = PLUS; tokens.push_back(t);}
"*" { Token t; t.type = TIMES; t.value = TIMES; tokens.push_back(t);}
">" { Token t; t.type = GT; t.value = GT; tokens.push_back(t);}
">=" { Token t; t.type = GE; t.value = GE; tokens.push_back(t);}
"==" { Token t; t.type = EQ; t.value = EQ; tokens.push_back(t);}
{digit}+ { Token t; t.type = INT_NUM; t.value = std::atoi(YYText()); tokens.push_back(t);}
"int" { Token t; t.type = INT; t.value = INT; tokens.push_back(t);}
"bool" { Token t; t.type = BOOL; t.value = BOOL; tokens.push_back(t);}
"do" { Token t; t.type = DO; t.value = DO; tokens.push_back(t);}
"while" { Token t; t.type = WHILE; t.value = WHILE; tokens.push_back(t);}
"print" { Token t; t.type = PRINT; t.value = PRINT; tokens.push_back(t);}
{letter}+{digit}* { Token t; t.type = ID; t.value = ID; tokens.push_back(t);}
"{" { Token t; t.type = '{'; t.value = '{'; tokens.push_back(t);}
"}" { Token t; t.type = '}'; t.value = '}'; tokens.push_back(t);}
"(" { Token t; t.type = '('; t.value = '('; tokens.push_back(t);}
")" { Token t; t.type =  ')'; t.value = ')'; tokens.push_back(t);}
"=" { Token t; t.type = ASSIGN; t.value = ASSIGN; tokens.push_back(t);}
. { std::cout << "Error" << '\n'; }
%%
