#include "nfa.h"
#include "parserPravila.h"
#include "dka.h"

using namespace std;

int main()
{
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    ParserPravila pp;

    NFA nfa;
    nfa.build(pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.produkcije);
    nfa.printNFA();
    cout << endl << endl;

    DKA dka(nfa, pp.nezavrsniZnakovi, pp.zavrsniZnakovi);
    dka.print();

    map<int, vector<string>> stavke;
    pp.zavrsniZnakovi.insert(pp.zavrsniZnakovi.begin(), "inicijalno");
    vector<vector<Pair>> table = dka.getTable(stavke, pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.symbolIndex);
    
    cout << endl;
    for(auto it : table){
        for(auto se : it){
            if(se.action == odbaci)
                cout << "----, ";
            else
                cout << se.action << "->" << se.stavka << ", ";
        }
        cout << endl;
    }

    for(auto it : stavke){
        cout << it.first << " -> ";
        for(auto se : it.second){
            cout << se << " ";
        }
        cout << endl;
    }
}
