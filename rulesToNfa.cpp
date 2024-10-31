#include "rulesToNfa.h"

#include "nfa.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "parserPravila.h"

#define NFA_STRUCTURE vector<map<string, vector<int>>>

using namespace std;

RulesToNfa::RulesToNfa(ParserPravila pp)
{

    nfa.addState("q0"); // to je q0
    stateNames.insert({ "q0", 0 });

    /// DODAVANJE STANJA ZA SVAKU PRODUKCIJU SA LUDOM TOCKOM I MAPIRANJE INDEKSA STANJA PO PRODUKCIJI U STRINGU
    for (auto i : pp.produkcije) { // i je par u mapi, i.first je ime stanja
        for (auto j : i.second) { // j je vector vektora stringa

            // i -> j1
            // i -> j2...

            for (int k = 0; k < j.size(); k++) {
                string name = ""; // ime tranzicije
                for (int i = 0; i <= j.size(); i++) { // ima ih k+1 jer za produkciju s k znakova se stvara k+1 stanja zbog svih kombinacija s tockom
                    if (i == k)
                        name.append("0"); // ona luda tocka
                    if (k < i)
                        name.append(j[k]);
                }
                stateNames.insert({ name, nfa.addState() });
            }
        }
    }

    nfa.addTransition(0, 1, EPSILON); // ona prva bezveze tranzicija

    for (int i = 0; i < nfa.structure.size(); i++) {
    }
}