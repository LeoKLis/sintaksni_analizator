#include "nfa.h"
#include "parserPravila.h"
#include "dka.h"
#include <iostream>

using namespace std;

int main()
{
    ParserPravila pp;

    NFA nfa;
    nfa.build(pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.produkcije);
    //nfa.printNFA();

    DKA dka(nfa);
    dka.print();
}
