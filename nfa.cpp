#include "nfa.h"

int NFA::createState(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts)
{
    StateNFA state;
    state.prodLeftSide = prodLeftSide;
    state.normalTransition = -1;
    if (prodRightSide.size() == 1 && prodRightSide[0] == "$") {
        state.dotIndex = 0;
    } else {
        state.prodRightSide = prodRightSide;
        state.dotIndex = dotIndex;
        if (prodRightSide.size() > dotIndex)
            state.normalTranSymbol = prodRightSide[dotIndex];
    }
    state.starts = starts;
    structure.push_back(state);
    return structure.size() - 1;
}

void NFA::addTransition(int from, int to, string znak)
{
    int dotIndex = structure[from].dotIndex;
    if (znak == EPSILON) {
        structure[from].epsilonTransitions.insert(to);
    } else if (dotIndex < structure[from].prodRightSide.size() && znak == structure[from].prodRightSide[dotIndex]) {
        structure[from].normalTransition = to;
    } else {
        cout << "Greska kod dodavanja tranzicije (znak == " << znak << ", tranZnak == " << structure[from].prodRightSide[dotIndex] << ")";
    }
}

string NFA::stringifyStateProduction(StateNFA state)
{
    return stringifyProduction(state.prodLeftSide, state.prodRightSide, state.dotIndex, state.starts);
}

string NFA::stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts)
{
    string output = "";
    output.append(prodLeftSide + " -> ");
    if (prodRightSide.size() == 1 && prodRightSide[0] == "$") {
        output.append("0 ");
        output.append("{ ");
        for (auto it : starts) {
            output.append(it + " ");
        }
        output.append("}");
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
    // output.pop_back();
    output.append("{ ");
    for (auto it : starts) {
        output.append(it + " ");
    }
    output.append("}");
    return output;
}

template <typename T>
bool NFA::exists(vector<T> array, T symbol)
{
    return find(array.cbegin(), array.cend(), symbol) != array.end();
}

void NFA::build(vector<string> nonFinalChars, vector<string> finalChars, map<string, vector<vector<string>>> productions)
{
    int numNonFinal = nonFinalChars.size();
    int numFinal = finalChars.size();
    int numChars = numNonFinal + numFinal;

    vector<vector<int>> startsWithChar(numChars, vector<int>(numChars));
    for (int i = 0; i < numChars; i++) {
        for (int j = 0; j < numChars; j++) {
            if (i == j) {
                startsWithChar[i][j] = 2;
                continue;
            }
            startsWithChar[i][j] = 0;
        }
    }

    map<string, int> charIndex;
    for (int i = 0; i < numNonFinal; i++) {
        charIndex.insert({ nonFinalChars[i], i });
    }
    for (int i = 0; i < numFinal; i++) {
        charIndex.insert({ finalChars[i], i + numNonFinal });
    }

    vector<int> emptyChars;
    int count = 0;
    for (auto it : nonFinalChars) {
        for (auto se : productions[it]) {
            if (exists(se, string { "$" })) {
                emptyChars.push_back(count);
                break;
            }
        }
        count++;
    }

    for (auto it : nonFinalChars) {
        for (auto se : productions[it]) {
            if (se.size() == 1 && se[0] == "$")
                continue;
            for (auto th : se) {
                startsWithChar[charIndex[it]][charIndex[th]] = 1;
                if (!exists(emptyChars, charIndex[th]))
                    break;
            }
        }
    }

    for (int i = 0; i < numNonFinal; i++) {
        for (int j = 0; j < numChars; j++) {
            if (startsWithChar[i][j] == 1) {
                for (int k = 0; k < numChars; k++) {
                    if (startsWithChar[j][k] == 1) {
                        startsWithChar[i][k] = 1;
                    }
                }
            }
        }
    }

    swtable.startsWithChar = startsWithChar;
    swtable.charIndex = charIndex;
    swtable.emptyChars = emptyChars;

    string initialState = nonFinalChars[0];
    int init = createState("inicijalno", vector<string> { initialState }, 0, set<string> { "$" });
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
    int nonEpsStateIndex = createState(state.prodLeftSide, state.prodRightSide, dotIndex + 1, state.starts);
    addTransition(stateIndex, nonEpsStateIndex, signOnIndex);
    recursiveBuild(nonEpsStateIndex, nonFinalChars, finalChars, productions);
    if (exists(finalChars, signOnIndex)) {
        return;
    }
    set<string> starts;
    if (dotIndex + 1 >= state.prodRightSide.size()) {
        starts = state.starts;
    } else {
        int idx;
        bool hadEmpty = false;
        for (int i = dotIndex + 1; i < state.prodRightSide.size(); i++) {
            string ch = state.prodRightSide[i];
            idx = swtable.charIndex[ch];
            for (int j = 0; j < finalChars.size(); j++) {
                if (swtable.startsWithChar[idx][j + nonFinalChars.size()] != 0) {
                    starts.insert(finalChars[j]);
                }
            }
            if (exists(swtable.emptyChars, idx))
                hadEmpty = true;
            if (!exists(swtable.emptyChars, idx))
                break;
        }
        if (hadEmpty)
            starts.insert("$");
        if (starts.empty())
            starts = state.starts;
    }
    for (auto it : productions[signOnIndex]) {
        string prod = stringifyProduction(signOnIndex, it, 0, starts);
        if (existingStates.count(prod) != 0) {
            addTransition(stateIndex, existingStates[prod], EPSILON);
            continue;
        }
        int epsStateIndex = createState(signOnIndex, it, 0, starts);
        string epsStateProduction = stringifyStateProduction(structure[epsStateIndex]);
        existingStates.insert({ epsStateProduction, epsStateIndex });
        addTransition(stateIndex, epsStateIndex, EPSILON);
        recursiveBuild(epsStateIndex, nonFinalChars, finalChars, productions);
    }
}

void NFA::printNFA()
{
    cout << structure.size() << endl;
    int count = 0;
    for (auto it : structure) {
        cout << count++ << ": " << stringifyStateProduction(it) << " ==> normal: " << it.normalTransition << ", epsilon: ";
        for (auto se : it.epsilonTransitions) {
            cout << se << " ";
        }
        cout << endl;
    }
}

set<int> NFA::resolveEpsilonEnviroment(set<int> currentEnv)
{
    if (currentEnv.empty())
        return currentEnv;
    if (epsEnvBigCache.find(currentEnv) != epsEnvBigCache.end()) {
        return epsEnvBigCache[currentEnv];
    }
    vector<int> outputEnv;
    outputEnv.insert(outputEnv.begin(), currentEnv.begin(), currentEnv.end());
    bool modified = true;
    int staticIndex = 0;
    while (modified) {
        modified = false;
        vector<int> tempEnv;
        for (int i = staticIndex; i < outputEnv.size(); i++) {
            for (int tran : structure[outputEnv[i]].epsilonTransitions) {
                if (find(outputEnv.begin(), outputEnv.end(), tran) == outputEnv.end()) {
                    tempEnv.push_back(tran);
                    modified = true;
                }
            }
        }
        staticIndex = outputEnv.size();
        outputEnv.insert(outputEnv.end(), tempEnv.begin(), tempEnv.end());
    }
    set<int> outputSet = set<int>(outputEnv.begin(), outputEnv.end());
    epsEnvBigCache.insert({ currentEnv, outputSet });
    return outputSet;
}

string NFA::normalTransitionSymbol(int stateIndex)
{

    StateNFA state = structure[stateIndex];
    return normalTransitionSymbol(state);
}

string NFA::normalTransitionSymbol(StateNFA state)
{

    string simbol = "ovo je kraj cijelog niza, svaka cast";
    if (state.dotIndex >= state.prodRightSide.size())
        return simbol;

    simbol = state.prodRightSide.at(state.dotIndex);
    return simbol;
}
