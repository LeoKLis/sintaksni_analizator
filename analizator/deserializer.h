#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum Action {
    pomakni, reduciraj, prihvati, stavi
};

struct Pair {
    Action action;
    int stavka;
};

class Deserializer {
private:
    void parseLine(vector<string> *arr, string line);
    void generateSymbMap();
public:
    vector<string> nezavrsniZnakovi;
    vector<string> zavrsniZnakovi;
    vector<string> sinkronizacijskiZnakovi;
    map<string, int> symbolIndex;
    
    vector<vector<Pair>> tablica;

    Deserializer();
    void printData();
};

#endif