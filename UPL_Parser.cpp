// UPL_Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <FlexLexer.h>
#include <sstream>
#include "Common.h"
#include "parser.tab.h"

extern SyntaxTree* root;

void printT(const std::string& prefix, SyntaxTree* node, bool isLeft)
{
	if (node != nullptr)
	{
		std::cout << prefix;

		std::cout << "|--";

		// print the value of the node
		std::cout << node->nodetype << std::endl;

		// enter the next tree level - left and right branch
		printT(prefix + (isLeft ? "|   " : "    "), node->l, true);
		printT(prefix + (isLeft ? "|   " : "    "), node->r, false);
	}
}
int main(int /* argc */, char** /* argv */)
{
	std::string str = R"(
		begin
			int x;
			int y=x+9;
			/*	comments

			*/
			if x>=a then{
				x=x+1;
			}
			bool a;//comment
			if x>a then{
				int c=1;
			}
			else{
				y=x;
				x=x+1;
			}
			print(a);
			
			bool x=a==b;
			do{
				int b=1;
				b=b*10;
				a=(b+10)*b;
			}while(a>1);
			print(a+1);
		end

	)";
	std::stringstream ss(str);
	FlexLexer* lexer = new yyFlexLexer;
	auto buffer = lexer->yy_create_buffer(ss, str.size());
	lexer->yy_switch_to_buffer(buffer);
	while (lexer->yylex() != 0)
		;
	lexer->yy_delete_buffer(buffer); // Delete the buffer

	while (yyparse() != 0);
	std::cout << "success\n";
	printT("", root,false);
	
}