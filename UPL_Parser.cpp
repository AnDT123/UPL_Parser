// UPL_Parser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <FlexLexer.h>
#include <sstream>
#include "Common.h"
#include "parser.tab.h"
#include "SLR_Class.h"

//extern SyntaxTree* root;
//extern std::vector<Token> tokens;
//std::vector<std::pair<int, int>> lineMap;
void printT(const std::string& prefix, SyntaxTree* node)
{
	if (node != nullptr)
	{
		std::cout << prefix;

		std::cout << "|--";

		// print the value of the node
		std::cout << node->nodetype << std::endl;

		// enter the next tree level - left and right branch
		if (node->  l != nullptr)  printT(prefix + "|   " , node->l);
		if (node -> r != nullptr) printT(prefix + "|   ", node->r);
	}
}
void bisonTest() {
	while (yyparse() != 0);
	printT("", root);
	if(parseSuccess)
		std::cout << "Parse success.\n";
	else
		std::cout << "Parse failed.\n";
}
int main(int /* argc */, char** /* argv */)
{
	std::string str = R"(
		begin
			int b=1;
			b=b*10;
			a=(b+10)*b;
			int b=1;
			b=b*10;
			a=(b+10)*b;
			int b=1;
			b=b*10;
			a=(b+10)*b;
			do{
					int b=1;
					b=b*10;
					a=(b+10)*b;
				}while(a>1);

		end

	)";
	std::stringstream ss(str);
	FlexLexer* lexer = new yyFlexLexer;
	auto buffer = lexer->yy_create_buffer(ss, str.size());
	lexer->yy_switch_to_buffer(buffer);
	while (lexer->yylex() != 0)
		;
	lexer->yy_delete_buffer(buffer); // Delete the buffer

	StateFactory stateFactory(tokens);
	stateFactory.yyparse();
	//bisonTest();
}