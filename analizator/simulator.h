#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <stack>
#include <string>
#include <vector>

#include "deserializer.h"
#include "lrTree.h"

using namespace std;

class Simulator {
private:
    Deserializer d;
    stack<pair<Node*, int>> stog;
    LrTree tree;

    void strip(string& line);
    void parseLine(string line, string* uniform, int* rowNum, string* lexUnit);

public:
    Simulator(Deserializer d);
    void simulate();
};

#endif