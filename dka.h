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

struct StateDFA {

    vector<string> prodLeftSide;
    vector<int> dotIndex;

    vector<vector<string>> prodRightSide;
    vector<set<string>> starts;

    map<string, set<int>> transition;
};

class DKA {
private:
    map<set<int>, map<string, set<int>>> speedyCache;
    vector<StateDFA> structure;
    map<string, int> indexMapping;
    void removeStateElement(StateDFA& state, int position);
    void combineStates(StateDFA& base, StateDFA added);
    void appendNfaToState(StateDFA& base, StateNFA from);
    void appendEnvToState(StateDFA& base, string symbol, set<int> lastEnv);
    string stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts);
    string stringifyTransition(set<int> transition);

public:
    DKA(NFA nfa, vector<string> nezavrsniZnakovi, vector<string> zavrsniZnakovi);
    void print();
};

#endif
