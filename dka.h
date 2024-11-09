#ifndef DKA_H
#define DKA_H

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "nfa.h"

using namespace std;

struct StateDFA {

    vector<string> prodLeftSide;
    vector<int> dotIndex;

    vector<vector<string>> prodRightSide;
    vector<set<string>> starts;

    map<string, set<int>> transition;
};

enum Action {
    pomakni,
    reduciraj,
    prihvati,
    stavi,
    odbaci
};

struct Pair {
    Action action;
    int stavka;
    Pair(Action a, int b)
    {
        action = a;
        stavka = b;
    }
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

public:
    DKA(NFA nfa, vector<string> nezavrsni, vector<string> zavrsni);

    string stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts);
    string stringifyTransition(set<int> transition);
    bool isAcceptState(StateDFA state);

    void print();
    vector<vector<Pair>> getTable(map<int, vector<string>>& stavke, vector<string> nezavrsniZnakovi, vector<string> zavrsniZnakovi, map<string, int> symbolIndex);
};

#endif
