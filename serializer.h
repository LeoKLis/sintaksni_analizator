#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "dka.h"

using namespace std;

class Serializer {
private:
public:
    Serializer(vector<vector<Pair>> table, map<int, vector<string>> stavke, vector<string> zavrsniZnakovi, vector<string> nezavrsniZnakovi, vector<string> sinkronizacijskiZnakovi, map<string, int> symbolIndex);
};

#endif