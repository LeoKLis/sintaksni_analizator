#include "nfa.h"

NFA::NFA()
{
    prefiksirano = false;
    currentStates.insert(0);
    set<int> newStates = resolveEpsilonEnviroment(currentStates);
    currentStates.insert(newStates.begin(), newStates.end());
}

NFA::NFA(string nfaName, NFA_STRUCTURE stateTransitions,
    map<int, vector<string>> acceptStatesMap)
{
    name = nfaName;
    nfaStructure = stateTransitions;
    this->acceptStatesMap = acceptStatesMap;
    prefiksirano = false;
    currentStates.insert(0);
    set<int> newStates = resolveEpsilonEnviroment(currentStates);
    currentStates.insert(newStates.begin(), newStates.end());
}

int NFA::isFinished()
{ // 0 - nema finalnih, ali ima stanja; 1 - ima i finalnih i mozda obicnih; 2
    // - nema stanja
    set<int> newStates = resolveEpsilonEnviroment(currentStates);
    currentStates.insert(newStates.begin(), newStates.end());
    if (currentStates.size() == 0) {
        return 2;
    }
    if (currentStates.size() > 0) {
        for (auto i : currentStates) {
            if (acceptStatesMap.find(i) != acceptStatesMap.end())
                return 1;
        }
    }
    return 0;
}

set<int> NFA::resolveEpsilonEnviroment(set<int> current)
{
    if (current.empty())
        return current;

    set<int> nextStates;
    for (auto i : current) {
        if (nfaStructure[i].count(EPSILON)) {
            for (auto j : nfaStructure[i][EPSILON])
                nextStates.insert(j);
        }
    }

    set<int> more_next_states = resolveEpsilonEnviroment(nextStates);

    for (auto i : more_next_states)
        nextStates.insert(i);
    return nextStates;
}

void NFA::readChar(char symbol)
{
    string znak = string { symbol };
    if (symbol == ' ') {
        znak = "\\_";
    } else if (symbol == '\t') {
        znak = "\\t";
    } else if (symbol == '\n') {
        znak = "\\n";
    }
    // Epsilon okruzenje
    set<int> newStates = resolveEpsilonEnviroment(currentStates);
    currentStates.insert(newStates.begin(), newStates.end());

    set<int> newCurrent;
    for (auto i : currentStates) {
        if (nfaStructure[i].count(znak)) {
            for (auto i : nfaStructure[i][znak])
                newCurrent.insert(i);
        }
    }
    currentStates.clear();
    currentStates.insert(newCurrent.begin(), newCurrent.end());
}

void NFA::restart()
{
    currentStates.clear();
    currentStates.insert(0);
    set<int> newStates = resolveEpsilonEnviroment(currentStates);
    currentStates.insert(newStates.begin(), newStates.end());
}

vector<string> NFA::getAction()
{
    set<int> newStates = resolveEpsilonEnviroment(currentStates);
    currentStates.insert(newStates.begin(), newStates.end());

    vector<int> copy_current;
    for (auto i : currentStates) {
        if (acceptStatesMap.count(i))
            copy_current.push_back(i);
    }

    sort(copy_current.begin(), copy_current.end());

    if (copy_current.empty())
        return vector<string> {};
    return acceptStatesMap[copy_current[0]];
}
