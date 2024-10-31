#ifndef NFA_H
#define NFA_H

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define EPSILON "epsilon"
// #define NFA_STRUCTURE vector<map<string, vector<int>>>

using namespace std;

struct StateNFA {
    string prodLeftSide;

    vector<string> prodRightSide;
    int dotIndex;

    int normalTransition;
    vector<int> epsilonTransitions;
};

class NFA {
private:
    void recursiveBuild(int stateIndex, vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions);
    bool isFinal(vector<string> finalChars, string symbol);

public:
    // NFA_STRUCTURE nfaStructure;
    vector<StateNFA> structure;
    map<string, int> existingStates;

    int createState(string prodLeftSide, vector<string> prodRightSide, int dotIndex); // Dodano!
    int createState(string stateName);
    void addTransition(int from, int to, string znak);
    string stringifyStateProduction(StateNFA state);
    string stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex);
    void build(vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions);
    void printNFA();
};

#endif