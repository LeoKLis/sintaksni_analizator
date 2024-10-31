#ifndef rulesToNfa_H
#define rulesToNfa_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "parserPravila.h"
#include "nfa.h"

using namespace std;

class RulesToNfa {
private:
    NFA nfa;
    map<string, int> stateNames;
    // map<int, vector<string>> stateNames ??
public:
    RulesToNfa(ParserPravila pp);
};

#endif
