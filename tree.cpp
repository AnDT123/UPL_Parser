#include <iostream>
#include <stdlib.h>
#include "Common.h"

SyntaxTree* newTree(std::string nodetype, SyntaxTree* l, SyntaxTree* r)
{
	SyntaxTree* t = new SyntaxTree(nodetype, l, r);
	if (!t) {
		std::cout << "out of space\n";
		exit(0);
	}
	return t;
}
SyntaxTree* newLeaf(int i)
{
	NumLeaf* a = new NumLeaf(i);
	if (!a) {
		std::cout << "out of space\n";

		exit(0);
	}
	return a;
}