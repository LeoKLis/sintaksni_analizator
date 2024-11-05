#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Deserializer {
private:
    void parseLine(vector<string> *arr, string line);
public:
    vector<string> nezavrsniZnakovi;
    vector<string> zavrsniZnakovi;
    vector<string> sinkronizacijskiZnakovi;
    vector<map<string, string>> akcija;
    vector<map<string, string>> novoStanje;

    Deserializer();
    void printData();
};

#endif