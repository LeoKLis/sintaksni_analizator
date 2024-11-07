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

    vector<string> prodLeftSide; // bit ce ih vise jer se kao spoje
    vector<int> dotIndex;

    vector<vector<string>> prodRightSide;
    vector<set<string>> starts;

    // vector<string> transitionSymbol;
    // vector<set<int>> transition;

    map<string, set<int>> transition;
};


enum Action{pomakni, reduciraj, prihvati, stavi};

struct Pair{
Action action;
int stavka;
};


class DKA {
private:
    map<set<int>, map<string, set<int>>> speedyCache;
    vector<StateDFA> structure;

    void removeStateElement(StateDFA& state, int position);
    void combineStates(StateDFA& base, StateDFA added);
    void appendNfaToState(StateDFA& base, StateNFA from);
    void appendEnvToState(StateDFA& base, string symbol, set<int> lastEnv);
    Pair pair(Action a,int b){
        Pair par;
        par.action = a;
        par.stavka = b;
    };

public:
    void combineStates(StateDFA &base, StateDFA &added);
    DKA(NFA nfa, vector<string> nezavrsni, vector<string> zavrsni);

    string stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts);
    string stringifyTransition(set<int> transition);

    void print();
    vector<vector<Pair>> get_table(map<int, vector<string>> &stavke, vector<string> nezavrsniZnakovi, vector<string> zavrsniZnakovi, map<string, int> symbolIndex);
};

#endif
