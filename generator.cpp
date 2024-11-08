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
    // nfa.printNFA();

    DKA dka(nfa, pp.nezavrsniZnakovi, pp.zavrsniZnakovi);
    dka.print();

    //dka.print();

    map<string, int> stavke

    vector<vector<Pair>> table = dka.get_table(&stavke, pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.symbolIndex);

}
