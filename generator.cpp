#include "nfa.h"
#include "parserPravila.h"
#include <iostream>

using namespace std;

int main()
{
    ParserPravila pp;
    // pp.printPravila();

    NFA nfa;
    nfa.build(pp.nezavrsniZnakovi, pp.zavrsniZnakovi, pp.produkcije);
    nfa.printNFA();
}
