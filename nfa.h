#ifndef NFA_H
#define NFA_H

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <set>

#define EPSILON "epsilon"
// #define NFA_STRUCTURE vector<map<string, vector<int>>>

using namespace std;

struct StateNFA {
    string prodLeftSide;

    vector<string> prodRightSide;
    int dotIndex;

    set<string> starts;

    int normalTransition;
    vector<int> epsilonTransitions;
};

struct SWTable {
    vector<vector<int>> startsWithChar;
    map<string, int> charIndex;
    vector<int> emptyChars;
};

class NFA {
private:

    void recursiveBuild(int stateIndex, vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions);
    template<typename T> bool exists(vector<T> array, T symbol);
    int createState(string stateName);
    void addTransition(int from, int to, string znak);
    string stringifyStateProduction(StateNFA state);
    string stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts);
public:
    vector<StateNFA> structure;
    map<string, int> existingStates;
    SWTable swtable;
    vector<StateNFA> structure; //privremeno
    void addTransition(int from, int to, string znak);
    int createState(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts); // Dodano!
    int createState(string stateName);

    void build(vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions);
    // Prvo se isprinta indeks stanja, pa pridruzena produkcija, pa ZAPOCINJE znakovi i onda prijelazi (jedan obicni i ostali epsilon prijelazi)
    void printNFA();
    set<int> resolveEpsilonEnviroment(set<int> current);
    string normalTransitionSymbol(int stateIndex);
    string normalTransitionSymbol(StateNFA state);

    int transition(int from, int to, string simbol);
};

#endif
