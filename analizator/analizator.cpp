#include "deserializer.h"
#include "simulator.h"

int main(){
    Deserializer d;
    Simulator s(d);
    s.simulate();
}