#ifndef DKA_H
#define DKA_H

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "nfa.h"

#define EPSILON "epsilon"
// #define NFA_STRUCTURE vector<map<string, vector<int>>>

using namespace std;


struct StateDFA{

    vector<string> prodLeftSide; //bit ce ih vise jer se kao spoje
    vector<int> dotIndex;

    vector<vector<string>> prodRightSide;

    vector<vector<string>> starts;

    vector<string> transitionSymbol;
    vector<int> transition;
};

class DKA {
private:
    vector<StateDFA> structure;
public:
    static void combineStates(StateDFA base, StateDFA added);
    DKA(NFA nfa);

};

#endif
