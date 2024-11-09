#ifndef PARSER_PRAVILA_H
#define PARSER_PRAVILA_H

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class ParserPravila {
private:
    map<string, int> getSymbolIndex();
public:
    vector<string> nezavrsniZnakovi;
    vector<string> zavrsniZnakovi;
    vector<string> sinkronizacijskiZnakovi;
    map<string, vector<vector<string>>> produkcije;

    map<string, int> symbolIndex;

    ParserPravila();
    void printPravila();
};

#endif
