#include "nfa.h"
#include "dka.h"

#include "nfa.cpp"
#include "dka.cpp"

using namespace std;

#define EPSILON "epsilon"

int main(){

    NFA nfa;

    nfa.createState("q0");
    nfa.createState("q1");
    nfa.createState("q2");

    nfa.transition(0,1,EPSILON);
    //nfa.addTransition(0,1,"b");
    nfa.transition(1,2,"a");

    DKA dka(nfa);
    dka.print();


return 0;
}
