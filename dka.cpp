#include "dka.h"

using namespace std;

void DKA::combineStates(StateDFA& base, StateDFA added)
{
    base.prodLeftSide.insert(base.prodLeftSide.end(), added.prodLeftSide.begin(), added.prodLeftSide.end());
    base.prodRightSide.insert(base.prodRightSide.end(), added.prodRightSide.begin(), added.prodRightSide.end());
    base.dotIndex.insert(base.dotIndex.end(), added.dotIndex.begin(), added.dotIndex.end());
    base.starts.insert(base.starts.end(), added.starts.begin(), added.starts.end());
}

void DKA::addProductionToState(StateDFA& base, StateNFA from)
{
    base.dotIndex.push_back(from.dotIndex);
    base.prodLeftSide.push_back(from.prodLeftSide);
    base.prodRightSide.push_back(from.prodRightSide);
    base.starts.push_back(from.starts);
}

void DKA::addTransitionToState(StateDFA& base, string symbol, set<int> lastEnv)
{
    base.transition[symbol].insert(lastEnv.begin(), lastEnv.end());
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

string DKA::stringifyProduction(string prodLeftSide, vector<string> prodRightSide, int dotIndex, set<string> starts)
{
    string structure = "";
    structure.append(prodLeftSide + " -> ");
    if (prodRightSide.size() == 1 && prodRightSide[0] == "$") {
        structure.append("0");
        return structure;
    }
    bool wroteZero = false;
    for (int i = 0; i < prodRightSide.size(); i++) {
        if (i == dotIndex) {
            structure.append("0 " + prodRightSide[i] + " ");
            wroteZero = true;
            continue;
        }
        structure.append(prodRightSide[i] + " ");
    }
    if (!wroteZero && dotIndex != -1) {
        structure.append("0 ");
    }
    structure.append("{ ");
    for (auto it : starts) {
        structure.append(it + " ");
    }
    structure.append("}");
    return structure;
}

string DKA::stringifyTransition(set<int> transition)
{
    string structure = "";
    if (transition.empty())
        return structure;
    for (auto it : transition) {
        structure.append(to_string(it) + ",");
    }
    structure.pop_back();
    return structure;
}

DKA::DKA(NFA nfa, vector<string>& nezavrsniZnakovi, vector<string>& zavrsniZnakovi)
{
    structure.push_back(StateDFA {});
    vector<string> sviZnakovi = nezavrsniZnakovi;
    sviZnakovi.insert(sviZnakovi.end(), zavrsniZnakovi.begin(), zavrsniZnakovi.end());
    int count = 0;
    vector<StateDFA> tempStructure;
    vector<StateNFA> firstStateEnv;
    for (StateNFA state : nfa.structure) {
        set<int> epsilonEnv = nfa.resolveEpsilonEnviroment(state.epsilonTransitions);
        epsilonEnv.insert(count);
        if (count == 0)
            for (int eps : epsilonEnv)
                addProductionToState(structure[0], nfa.structure.at(eps));
        StateDFA tempState;
        addProductionToState(tempState, nfa.structure[count]);
        for (string znak : sviZnakovi) {
            for (int eps : epsilonEnv) {
                if (nfa.structure[eps].normalTranSymbol == znak) {
                    set<int> lastEnv = nfa.resolveEpsilonEnviroment(set<int> { nfa.structure[eps].normalTransition });
                    addTransitionToState(tempState, nfa.structure[eps].normalTranSymbol, lastEnv);
                    if (count == 0)
                        addTransitionToState(structure[0], nfa.structure[eps].normalTranSymbol, lastEnv);
                }
            }
        }
        count += 1;
        tempStructure.push_back(tempState);
    }

    indexMapping["0"] = 0;
    int index = 1;
    int startLine = 0;

    bool isModified = true;
    while (isModified || startLine < structure.size()) {
        isModified = false;
        for (auto se = structure[startLine].transition.cbegin(); se != structure[startLine].transition.cend(); se++) {
            StateDFA tempState;
            string tran = stringifyTransition(se->second);
            if (tran == "" || indexMapping.count(tran) == 1)
                continue;
            indexMapping[tran] = index++;
            for (int th : se->second) {
                addProductionToState(tempState, nfa.structure[th]);
                for (auto el : tempStructure[th].transition)
                    addTransitionToState(tempState, el.first, el.second);
            }
            structure.push_back(tempState);
            isModified = true;
        }
        startLine += 1;
    }
    zavrsniZnakovi.push_back("$");
    nezavrsniZnakovi.insert(nezavrsniZnakovi.begin(), "inicijalno");
    cout << structure.size() << endl;
}

bool DKA::isAcceptState(StateDFA state)
{
    for (int i = 0; i < state.prodLeftSide.size(); i++)
        if (state.prodLeftSide[i] == "inicijalno" && state.dotIndex[i] == 1)
            return true;
    return false;
}

vector<vector<Pair>> DKA::getTable(map<int, vector<string>>& stavke, vector<string> nezavrsniZnakovi, vector<string> zavrsniZnakovi, map<string, int> symbolIndex)
{
    vector<vector<Pair>> table(structure.size(), vector<Pair>(nezavrsniZnakovi.size() + zavrsniZnakovi.size(), Pair(odbaci, 0)));
    map<vector<string>, int> stavkeLookup;
    for (int i = 0; i < structure.size(); i++) {
        StateDFA state = structure.at(i);

        // ispunjavanje NovoStanje dijela tablice
        for (auto prijelaz : state.transition)
            if (find(nezavrsniZnakovi.begin(), nezavrsniZnakovi.end(), prijelaz.first) != nezavrsniZnakovi.end()) // state ima prijelaz s nezavrsnim simbolom
                table[i][symbolIndex.at(prijelaz.first)] = Pair(stavi, indexMapping[stringifyTransition(prijelaz.second)]);

        // ispunjavanje Akcija dijela tablice
        for (int j = 0; j < state.dotIndex.size(); j++) {
            int dotIndex = state.dotIndex.at(j);
            string prodLeftSide = state.prodLeftSide.at(j);
            vector<string> prodRightSide = state.prodRightSide.at(j);
            set<string> starts = state.starts.at(j);

            if (dotIndex >= prodRightSide.size()) { // reduciraj (tocka je skroz desno)
                for (auto simbolcic : starts) {
                    if (simbolcic == "$" && isAcceptState(state)) { // Slucaj za zavrsno stanje
                        table[i][symbolIndex.at(simbolcic)] = Pair(prihvati, 0);
                        continue;
                    }
                    if (find(zavrsniZnakovi.begin(), zavrsniZnakovi.end(), simbolcic) != zavrsniZnakovi.end()) { // Reduciraj, simbolcic mora biti zavrsni znak
                        vector<string> stavkeVec;
                        stavkeVec.insert(stavkeVec.begin(), prodLeftSide);
                        if (prodRightSide.size() > 0)
                            stavkeVec.insert(stavkeVec.end(), prodRightSide.begin(), prodRightSide.end());
                        else
                            stavkeVec.push_back("$");
                        if (stavkeLookup.find(stavkeVec) != stavkeLookup.end()) {
                            table[i][symbolIndex.at(simbolcic)] = Pair(reduciraj, stavkeLookup.at(stavkeVec));
                            continue;
                        }
                        int n = stavke.size();
                        stavke.insert({ n, stavkeVec });
                        stavkeLookup.insert({ stavkeVec, n });
                        table[i][symbolIndex.at(simbolcic)] = Pair(reduciraj, stavke.size() - 1);
                    }
                }
            } else { // pomakni
                string zavrsniZnakic = string { prodRightSide[dotIndex] };
                if (state.transition.find(zavrsniZnakic) != state.transition.end() && find(zavrsniZnakovi.begin(), zavrsniZnakovi.end(), zavrsniZnakic) != zavrsniZnakovi.end()) { // postoji prijelaz iz stanja state sa simbolom zavrsniZnakic i zavrsniZnakic jest zavrsan
                    int transition = indexMapping[stringifyTransition(state.transition.at(zavrsniZnakic))];
                    table[i][symbolIndex.at(zavrsniZnakic)] = Pair(pomakni, transition);
                }
            }
        }
    }
    return table;
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