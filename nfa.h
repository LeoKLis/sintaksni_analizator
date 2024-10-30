#ifndef NFA_H
#define NFA_H

#include <bits/stdc++.h>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#define EPSILON "epsilon"

using namespace std;

#define NFA_STRUCTURE vector<map<string, vector<int>>>

class NFA {
private:
    bool prefiksirano;
    set<int> currentStates;
    set<int> resolveEpsilonEnviroment(set<int> current);

public:
    string name;
    NFA_STRUCTURE nfaStructure;
    map<int, vector<string>> acceptStatesMap;

    NFA();
    NFA(string nfaName, NFA_STRUCTURE stateTransitions, map<int, vector<string>> acceptStatesMap);
    int isFinished();
    void readChar(char symbol);
    vector<string> getAction();

    int addState(){     //Moramo dodat ovo fakat je lakse testirat
        map<string, vector<int>> mapa;
        nfaStructure.push_back(mapa);
    };
    void addTransition(int from, int to, string znak){
        ((nfaStructure.at(from)).at(znak)).push_back(to);
    };

    void restart();
};

#endif
