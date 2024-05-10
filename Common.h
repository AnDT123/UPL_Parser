#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct Token {
    int type;
    int value;
};
extern std::vector<Token> tokens;
extern std::vector<std::pair<int, int>> lineMap;
extern bool parseSuccess;
class SyntaxTree {
public:
    std::string nodetype;
    SyntaxTree* l;
    SyntaxTree* r;
    SyntaxTree(std::string type, SyntaxTree* left, SyntaxTree* right) {
        nodetype = type;
        l = left;
        r = right;
    }
};
class NumLeaf : public SyntaxTree {
public:
    int value;
    NumLeaf(int number) : SyntaxTree("Leaf", nullptr, nullptr) {
        value = number;
    }
};
SyntaxTree* Tree(std::string nodetype, struct ast* l, struct ast* r);
SyntaxTree* Leaf(int i);
extern SyntaxTree* root;

//long hashFunction(const std::string& str) {
//    long hash = 5381;
//    for (char c : str) {
//        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//    }
//    return hash;
//}

class HashTable {
public:
    std::unordered_map<long, std::pair<int, int>> table;

    void insert(long key, int value, int type) {
        table[key] = std::make_pair(value, type);
    }
    bool getValue(long key, int &value) {
        if (table.find(key) != table.end()) {
            value = table[key].first;
            return true;
        }
        else {
            return false;
        }
    }

    bool getType(long key, int& type) {
        if (table.find(key) != table.end()) {
            type =  table[key].second;
            return true;
        }
        else {
            return false;
        }
    }
};

class Env {
private:
    HashTable* table;
public:
    Env* prev;
    Env(Env *p) {
        table = new HashTable();
        prev = p;
    }
    void put(long key, int value, int type) {
        table->insert(key, value, type);
    }
    bool getValue(long key, int& value) {
        for (Env* e = this; e != nullptr; e = e->prev)
            if (e->table->getValue(key, value))
                return true;
        return false;
    }
    bool getType(long key, int& type) {
        for (Env* e = this; e != nullptr; e = e->prev)
            if (e->table->getType(key, type))
                return true;
        return false;
    }
};