#include "dka.h"

using namespace std;

void DKA::combineStates(StateDFA& base, StateDFA added)
{
    // for (int i = 0; i < added.dotIndex.size(); i++) {

    //     base.prodLeftSide.push_back(added.prodLeftSide.at(i));

    //     base.starts.push_back(added.starts.at(i));

    //     base.dotIndex.push_back(added.dotIndex.at(i));

    //     base.prodRightSide.push_back(added.prodRightSide.at(i));
    // }

    base.prodLeftSide.insert(base.prodLeftSide.end(), added.prodLeftSide.begin(), added.prodLeftSide.end());
    base.prodRightSide.insert(base.prodRightSide.end(), added.prodRightSide.begin(), added.prodRightSide.end());
    base.dotIndex.insert(base.dotIndex.end(), added.dotIndex.begin(), added.dotIndex.end());
    base.starts.insert(base.starts.end(), added.starts.begin(), added.starts.end());

    // base.transition.insert(added.transition.begin(), added.transition.end());
    // for (auto el = added.transition.begin(); el != added.transition.end(); el++) {
    //     base.transition[el->first].insert(el->second.begin(), el->second.end());
    // }
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

/* DKA::DKA(NFA nfa)
{
    map<vector<int>, int> existingStates; // vektor je sorted skup tih stanja, int je index spojenog stanja

    for (auto state : nfa.structure) { // dodana sva stanja s normalnim tranzicijama, bez epsilon tranzicija
        // cout << "Proso epselon" << endl;
        StateDFA dfa;
        dfa.prodLeftSide.push_back(state.prodLeftSide);
        dfa.starts.push_back(state.starts);
        dfa.dotIndex.push_back(state.dotIndex);
        dfa.prodRightSide.push_back(state.prodRightSide);

        // cout << "Proso epselon ovo" << endl;
        if (state.normalTransition != -1) {
            string simbol = nfa.normalTransitionSymbol(state);
            dfa.transitionSymbol.push_back(simbol);
            dfa.transition.push_back(state.normalTransition);
        }

        structure.push_back(dfa);

        // vector<int> stateIndex;
        vector<int> combinedStates;
        // stateIndex.push_back(structure.size()-1);
        combinedStates.push_back(structure.size() - 1);
        existingStates.insert({ combinedStates, structure.size() - 1 });
    }

    /// rjesavanje epsilon tranzicija
    for (int i = 0; i < structure.size(); i++) {
        set<int> epsilon;
        epsilon.insert(i);
        epsilon = nfa.resolveEpsilonEnviroment(epsilon);
        for (auto indexOfNewState : epsilon) {
            StateNFA new_state = nfa.structure.at(indexOfNewState); // stanje u epsilon okruzenju
            if (new_state.normalTransition != -1) { // ako ima normalTransition dalje, dodaj to u orginalno stanje
                string simbol = nfa.normalTransitionSymbol(new_state);
                structure[i].transition.push_back(new_state.normalTransition);
                structure[i].transitionSymbol.push_back(simbol);
            }
        }
    } // ovo je sada obican nka, bez epsilona
    int i, n = 0;


    bool new_state_added = true;

    while (new_state_added == true) {


        new_state_added = false;
        i = n;
        n = structure.size();

        for (; i < n; i++) {
            // StateDFA state = structure.at(i);
            vector<string> transitionSymbol; // ovo ce kasnije state.transitionSymbol = transitionSymbol
            vector<int> transition; // ovo ce kasnije state.transition = transition

            map<string, vector<int>> sortedTransitions;

            for (int j = 0; j < structure.at(i).transition.size(); j++) { // analiziranje svakog prijelaza stanja State
                string simbol = structure.at(i).transitionSymbol.at(j);
                int nextState = structure.at(i).transition.at(j);
                if (sortedTransitions.find(simbol) != sortedTransitions.end())
                    (sortedTransitions.at(simbol)).push_back(nextState);
                else {
                    vector<int> vec;
                    vec.push_back(nextState);
                    sortedTransitions.insert({ simbol, vec });
                }
            }

            for (auto simbol_vektorNovihStanja : sortedTransitions) {
                string simbol = simbol_vektorNovihStanja.first;
                vector<int> stateKey; // = simbol_vektorNovihStanja.second;
                for (auto k : simbol_vektorNovihStanja.second)
                    stateKey.push_back(k);

                sort(stateKey.begin(), stateKey.end());

                if (existingStates.find(stateKey) != existingStates.end()) { // postoji takvo stanje
                    int combinedState = existingStates.at(stateKey);
                    transition.push_back(combinedState);
                    transitionSymbol.push_back(simbol);
                } else { // potrebno je stvoriti novo stanje koje ce biti kombinacija prvobitnih stanja

                    new_state_added = true;
                    StateDFA combinedState;
                    int index = structure.size();
                    existingStates.insert({ (stateKey), (index) });
                    //structure.push_back(combinedState);

                    //cout<<"kombiniram: ";
                    for (auto metaInfo : stateKey) {
                        //cout<<metaInfo<<", ";
                        combineStates(combinedState, structure.at(metaInfo));
                    } //cout<<endl;

                    structure.push_back(combinedState);

                    transition.push_back(structure.size() - 1);
                    transitionSymbol.push_back(simbol);
                }
            }
            structure.at(i).transitionSymbol.clear();
            structure.at(i).transition.clear();
            structure.at(i).transitionSymbol = transitionSymbol;
            structure.at(i).transition = transition;
        }
    }

} */

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
    map<string, int> indexMapping;
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

    for (auto it = 0; it < output.size(); it++) {
        cout << it << " -> " << endl;
        StateDFA s = output[it];
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
    // cout << "Ukupno vrijeme odsjecka " << sumTime << endl;
    cout << "Output size: " << output.size() << endl;
}

void DKA::print()
{
    int count = 0;
    for (auto it : structure) {

        cout << endl
             << count++;
        if (it.prodLeftSide.empty())
            cout << "\t-" << endl;
        for (int i = 0; i < it.prodLeftSide.size(); i++) {
            string prod = stringifyProduction(it.prodLeftSide[i], it.prodRightSide[i], it.dotIndex[i], it.starts[i]);
            cout << "\t" << prod << endl;
        }
        cout << endl;
        for (auto se = it.transition.cbegin(); se != it.transition.cend(); se++) {
            cout << "\t" << se->first << " -> ";
            for (int th : se->second) {
                cout << th << " ";
            }
            cout << endl;
        }
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
