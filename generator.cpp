#include "nfa.h"
#include "parserPravila.h"
#include "dka.h"
#include "serializer.h"

using namespace std;

int main()
{
    ParserPravila pp;

    NFA nfa;
    nfa.build(pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.produkcije);
    // nfa.printNFA();

    DKA dka(nfa, pp.nezavrsniZnakovi, pp.zavrsniZnakovi);
    // dka.print();

    map<int, vector<string>> stavke;
    vector<vector<Pair>> table = dka.getTable(stavke, pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.symbolIndex);
    
    Serializer s(table, stavke, pp.zavrsniZnakovi, pp.nezavrsniZnakovi, pp.sinkronizacijskiZnakovi, pp.symbolIndex);

}
