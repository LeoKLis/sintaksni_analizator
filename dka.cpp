#include "dka.h"

using namespace std;


void DKA::combineStates(StateDFA base, StateDFA added){

    for(int i=0; i<added.dotIndex.size(); i++){
        base.prodLeftSide.push_back(added.prodLeftSide);
        base.starts.push_back(added.starts);
        base.dotIndex.push_back(added.dotIndex);
        base.prodRightSide.push_back(added.prodRightSide);
    }
}

DKA::DKA(NFA nfa){

    map<vector<int>, int> existingStates; //vektor je sorted skup tih stanja, int je index spojenog stanja

    for(auto state : nfa.structure){ //dodana sva stanja s normalnim tranzicijama, bez epsilon tranzicija
        StateDFA dfa;
        dfa.prodLeftSide.push_back(state.prodLeftSide);
        dfa.starts.push_back(state.starts);
        dfa.dotIndex.push_back(state.dotIndex);
        dfa.prodRightSide.push_back(state.prodRightSide);

        if(state.normalTransition != -1){

            string simbol = nfa.normalTransitionSymbol(state);
            dfa.transitionSymbol.push_back(simbol);
            dfa.transition.push_back(state.normalTransition);
        }

        structure.push_back(dfa);

        vector<int> stateIndex;
        vector<int> combinedStates;
        stateIndex.push_back(structure.size()-1);
        combinedStates.push_back(structure.size()-1);
        existingStates.insert({combinedStates, stateIndex});
    }

    ///rjesavanje epsilon tranzicija
    for(int i = 0; i < structure.size(); i++){
        StateDFA state = structure[i];
        set<int> epsilon;
        epsilon.insert(i);
        epsilon = nfa.resolveEpsilonEnviroment(epsilon):
        epsilon.erase(i);

        for(auto indexOfNewState : epsilon){
            StateEpsilonNFA new_state = nfa.structure //stanje u epsilon okruzenju

            if(new_state.normalTransition != -1){ //ako ima normalTransition dalje, dodaj to u orginalno stanje
                string simbol = nfa.normalTransitionSymbol(new_state);
                state.transition.push_back(new_state.normalTransition);
                state.transitionSymbol.push_back(simbol);
            }
        }
    } //ovo je sada obican nka, bez epsilona

    int i,n=0;

    bool new_state_added=true
    while(new_state_added==true){
        new_state_added=false;
        i = n;
        n = structure.size();

        for(; i<n; i++){
            StateDFA state = structure.at(i);
            vector<string> transitionSymbol; //ovo ce kasnije state.transitionSymbol = transitionSymbol
            vector<int> transition; // ovo ce kasnije state.transition = transition

            map<string, vector<int>> sortTransitions;

            for(int j=0; j<state.transition.size(); j++){ //analiziranje svakog prijelaza stanja State
                string simbol = state.transitionSymbol.at(j);
                int nextState = state.transition.at(j);
                if(sortedTransitions.find(simbol) != sortedTransitions.end())
                    (sortedTransitions.at(simbol)).push_back(new_state);
                else{
                    vector<int> vec;
                    vec.push_back(nextState);
                    sortedTransitions.insert({simbol, vec});
                }
            }

            for(auto simbol_vektorNovihStanja : sortTransitions){
                string simbol =simbol_vektorNovihStanja.first;
                vector<int> stateKey = simbol_vektorNovihStanja.second;
                sort(stateKey.begin(), stateKey.end());

                if(existingStates.find(stateKey) != existingStates.end()){ //postoji takvo stanje
                    int combinedState = existingStates.at(stateKey);
                    transition.push_back(combinedState);
                    transitionSymbol.push_back(simbol);
                }
                else{ //potrebno je stvoriti novo stanje koje ce biti kombinacija prvobitnih stanja
                    new_state_added=true;
                    StateDFA combinedState;
                    existingStates.insert(stateKey, structure.size());
                    structure.push_back(combinedState);

                    for(auto metaInfo : stateKey){
                        combineStates(combineStates, existingStates(metaInfo));
                    }
                    transition.push_back(structure.size()-1);
                    transitionSymbol.push_back(simbol);
                }
            }
            state.transitionSymbol.clear();
            state.transition.clear();
            state.transitionSymbol = transitionSymbol;
            state.transition = transition;
        }
    }




}
