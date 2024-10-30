#ifndef rulesToNfa_H
#define rulesToNfa_H

#include "nfa.h"
#include "nfa.cpp"
#include<iostream>
#include<string>
#include<vector>
#include<map>

#include "parserPravila.h"
#include "parserPravila.cpp"

using namespace std;

class rulesToNfa{
private:
    NFA nfa;
    map<string, int> stateNames;
    //map<int, vector<string>> stateNames ??
public:
    rulesToNfa(ParserPravila pp)


};

#endif
