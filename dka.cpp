#include "dka.h"

using namespace std;

void DKA::combineStates(StateDFA& base, StateDFA added)
{
    base.prodLeftSide.insert(base.prodLeftSide.end(), added.prodLeftSide.begin(), added.prodLeftSide.end());
    base.prodRightSide.insert(base.prodRightSide.end(), added.prodRightSide.begin(), added.prodRightSide.end());
    base.dotIndex.insert(base.dotIndex.end(), added.dotIndex.begin(), added.dotIndex.end());
    base.starts.insert(base.starts.end(), added.starts.begin(), added.starts.end());
}

void DKA::appendNfaToState(StateDFA& base, StateNFA from)
{
    base.dotIndex.push_back(from.dotIndex);
    base.prodLeftSide.push_back(from.prodLeftSide);
    base.prodRightSide.push_back(from.prodRightSide);
    base.starts.push_back(from.starts);
}

void DKA::appendEnvToState(StateDFA& base, string symbol, set<int> lastEnv)
{
    base.transition[symbol].insert(lastEnv.begin(), lastEnv.end());
}

string DKA::stringifyTransition(set<int> transition)
{
    string output = "";
    if (transition.empty())
        return output;
    for (auto it : transition) {
        output.append(to_string(it) + ",");
    }
    output.pop_back();
    return output;
}

void DKA::removeStateElement(StateDFA& state, int position)
{
    auto di = state.dotIndex.begin() + position;
    auto lef = state.prodLeftSide.begin() + position;
    auto rig = state.prodRightSide.begin() + position;
    auto pos = state.starts.begin() + position;
    state.dotIndex.erase(di);
    state.prodLeftSide.erase(lef);
    state.prodRightSide.erase(rig);
    state.starts.erase(pos);
}

DKA::DKA(NFA nfa, vector<string> nezavrsniZnakovi, vector<string> zavrsniZnakovi)
{
    vector<string> sviZnakovi = nezavrsniZnakovi;
    sviZnakovi.insert(sviZnakovi.end(), zavrsniZnakovi.begin(), zavrsniZnakovi.end());

    int count = 0;
    vector<StateDFA> tempStructure;
    for (StateNFA state : nfa.structure) {
        set<int> epsilonEnv = nfa.resolveEpsilonEnviroment(state.epsilonTransitions);
        epsilonEnv.insert(count);
        StateDFA tempState;
        appendNfaToState(tempState, nfa.structure[count]);
        for (string znak : sviZnakovi) {
            for (int eps : epsilonEnv) {
                if (nfa.structure[eps].normalTranSymbol == znak) {
                    set<int> lastEnv = nfa.resolveEpsilonEnviroment(set<int> { nfa.structure[eps].normalTransition });
                    appendEnvToState(tempState, nfa.structure[eps].normalTranSymbol, lastEnv);
                }
            }
        }
        count += 1;
        tempStructure.push_back(tempState);
    }

    vector<StateDFA> output;
    output.push_back(tempStructure[0]);
    removeStateElement(output[0], 0);
    for (StateNFA state : nfa.structure) {
        if (state.dotIndex == 0) {
            appendNfaToState(output[0], state);
        }
    }
    indexMapping["0"] = 0;
    int index = 1;
    int iter = 0;

    int sumTime = 0;
    bool modified = true;
    while (modified) {
        modified = false;
        for (auto it = iter; it < output.size(); it++) {
            for (auto se = output[it].transition.cbegin(); se != output[it].transition.cend(); se++) {
                StateDFA tempState;
                string tran = stringifyTransition(se->second);
                if (tran == "" || indexMapping.count(tran) == 1)
                    continue;
                indexMapping[tran] = index++;
                for (int th : se->second) {
                    appendNfaToState(tempState, nfa.structure[th]);
                    // if(index < 50)
                    //     cout << th << " ";
                }
                // if(index < 50)
                //     cout << endl;
                // if(speedyCache.find(se->second) != speedyCache.end()){
                //     cout << "Found in cache!" << endl;
                //     tempState.transition = speedyCache.at(se->second);
                // }
                chrono::steady_clock::time_point begin = chrono::steady_clock::now();
                for (int th : se->second) {
                    for (auto el = tempStructure[th].transition.cbegin(); el != tempStructure[th].transition.cend(); el++) {
                        tempState.transition[el->first].insert(el->second.begin(), el->second.end());
                    }
                }
                if(se->second.size() > 2)
                    speedyCache[se->second] = tempState.transition;
                chrono::steady_clock::time_point end = chrono::steady_clock::now();
                auto time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
                sumTime += time;
                // cout << "Vrijeme izvodenja u petlji: " << time << "[µs] " << endl;
                output.push_back(tempState);
                modified = true;
            }
        }
        iter += 1;
    }
    // cout << "Ukupno vrijeme odsjecka " << sumTime << endl;
    cout << "Output size: " << output.size() << endl;
    structure = output;
}

void DKA::print()
{
    for (auto it = 0; it < structure.size(); it++) {
        cout << it << " -> " << endl;
        StateDFA s = structure[it];
        for (int i = 0; i < s.prodLeftSide.size(); i++) {
            string prod = stringifyProduction(s.prodLeftSide[i], s.prodRightSide[i], s.dotIndex[i], s.starts[i]);
            cout << "\t" << prod << endl;
        }
        cout << endl;
        for (auto se = s.transition.cbegin(); se != s.transition.cend(); se++) {
            if (se->second.empty())
                continue;
            cout << "\t" << se->first << " -> ";
            cout << indexMapping[stringifyTransition(se->second)] << endl;
        }
        cout << endl;
    }
}

string DKA::stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts)
{
    string output = "";
    output.append(prodLeftSide + " -> ");
    if (prodRightSide.size() == 1 && prodRightSide[0] == "$") {
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
    // output.pop_back();
    output.append("{ ");
    for (auto it : starts) {
        output.append(it + " ");
    }
    output.append("}");
    return output;
}
