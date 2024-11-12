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
    Pair(Action action, int stavka)
    {
        this->action = action;
        this->stavka = stavka;
    }
    Pair(int action, int stavka)
    {
        switch (action) {
        case 0:
            this->action = pomakni;
            break;
        case 1:
            this->action = reduciraj;
            break;
        case 2:
            this->action = prihvati;
            break;
        case 3:
            this->action = stavi;
            break;
        default:
            cout << "Ubacen krivi index. Nije dobro";
            break;
        }
        this->stavka = stavka;
    }
};

class DKA {
private:
    vector<StateDFA> structure;
    map<string, int> indexMapping;

    void addProductionToState(StateDFA& base, StateNFA from);
    void addTransitionToState(StateDFA& base, string symbol, set<int> lastEnv);
    void combineStates(StateDFA& base, StateDFA added);
    void removeStateElement(StateDFA& state, int position);
    bool isAcceptState(StateDFA state);

    string stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts);
    string stringifyTransition(set<int> transition);
public:


    DKA(NFA nfa, vector<string> &nezavrsni, vector<string> &zavrsni);
    vector<vector<Pair>> getTable(map<int, vector<string>>& stavke, vector<string> nezavrsniZnakovi, vector<string> zavrsniZnakovi, map<string, int> symbolIndex);
    void print();
};

#endif
