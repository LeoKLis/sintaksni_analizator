#include "nfa.h"

int NFA::createState(string prodLeftSide, vector<string> prodRightSide, int dotIndex)
{
    // map<string, vector<int>> mapa;
    // nfaStructure.push_back(mapa);
    StateNFA state;
    state.prodLeftSide = prodLeftSide;
    state.normalTransition = -1;
    if (prodRightSide.size() == 1 && prodRightSide[0] == "$") {
        state.dotIndex = 0;
    } else {
        state.prodRightSide = prodRightSide;
        state.dotIndex = dotIndex;
    }

    structure.push_back(state);
    return structure.size() - 1;
}

int NFA::createState(string stateName)
{
    StateNFA state;
    state.prodLeftSide = stateName;
    state.normalTransition = -1;
    state.dotIndex = -1;

    structure.push_back(state);
    return structure.size() - 1;
}

void NFA::addTransition(int from, int to, string znak)
{
    // nfaStructure.at(from).at(znak).push_back(to);
    int dotIndex = structure[from].dotIndex;
    if (znak == EPSILON) {
        structure[from].epsilonTransitions.push_back(to);
    } else if (dotIndex < structure[from].prodRightSide.size() && znak == structure[from].prodRightSide[dotIndex]) {
        structure[from].normalTransition = to;
    } else {
        cout << "Greska kod dodavanja tranzicije (znak == " << znak << ", tranZnak == " << structure[from].prodRightSide[dotIndex] << ")";
    }
}

string NFA::stringifyStateProduction(StateNFA state)
{
    return stringifyProduction(state.prodLeftSide, state.prodRightSide, state.dotIndex);
}

string NFA::stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex)
{
    string output = "";
    output.append(prodLeftSide + " -> ");
    if(prodRightSide.size() == 1 && prodRightSide[0] == "$"){
        output.append("0");
        return output;
    }
    bool wroteZero = false;
    for (int i = 0; i < prodRightSide.size(); i++) {
        if (i == dotIndex) {
            output.append("0 " + prodRightSide[i] + " ");
            wroteZero = true;
            continue;
        }
        output.append(prodRightSide[i] + " ");
    }
    if (!wroteZero && dotIndex != -1) {
        output.append("0 ");
    }
    output.pop_back();
    return output;
}

bool NFA::isFinal(vector<string> finalChars, string symbol)
{
    return find(finalChars.cbegin(), finalChars.cend(), symbol) != finalChars.end();
}

void NFA::build(vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions)
{
    string initialState = nonFinalChars[0];
    int nul = createState("nulto");
    int init = createState("inicijalno", vector<string> { initialState }, 0);
    addTransition(nul, init, EPSILON);
    recursiveBuild(init, nonFinalChars, finalChars, productions);
}

void NFA::recursiveBuild(int stateIndex, vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions)
{
    StateNFA state = structure[stateIndex];
    int dotIndex = state.dotIndex;
    if (state.prodRightSide.size() <= dotIndex) {
        return;
    }
    string signOnIndex = state.prodRightSide[dotIndex];
    int nonEpsStateIndex = createState(state.prodLeftSide, state.prodRightSide, state.dotIndex + 1);
    addTransition(stateIndex, nonEpsStateIndex, signOnIndex);
    recursiveBuild(nonEpsStateIndex, nonFinalChars, finalChars, productions);
    if (isFinal(finalChars, signOnIndex)) {
        return;
    }
    for (auto it : productions[signOnIndex]) {
        string prod = stringifyProduction(signOnIndex, it, 0);
        if (existingStates.count(prod) != 0) {
            addTransition(stateIndex, existingStates[prod], EPSILON);
            continue;
        }
        int epsStateIndex = createState(signOnIndex, it, 0);
        string epsStateProduction = stringifyStateProduction(structure[epsStateIndex]);
        existingStates.insert({ epsStateProduction, epsStateIndex });
        addTransition(stateIndex, epsStateIndex, EPSILON);
        recursiveBuild(epsStateIndex, nonFinalChars, finalChars, productions);
    }
}

void NFA::printNFA()
{
    int count = 0;
    for (auto it : structure) {
        cout << count++ << ": " << stringifyStateProduction(it) << " ==> normal: " << it.normalTransition << ", epsilon: ";
        for (auto se : it.epsilonTransitions) {
            cout << se << " ";
        }
        cout << endl;
    }
}