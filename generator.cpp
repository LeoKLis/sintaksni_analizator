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
    // dka.print();


    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    auto time = chrono::duration_cast<chrono::microseconds>(end - begin).count();
    cout << "Vrijeme izvodenja: " << time << "[µs] " << endl;
}
