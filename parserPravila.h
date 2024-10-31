#ifndef PARSER_PRAVILA_H
#define PARSER_PRAVILA_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

class ParserPravila {
private:
public:
    vector<string> nezavrsniZnakovi;
    vector<string> zavrsniZnakovi;
    vector<string> sinkronizacijskiZnakovi;
    map<string, vector<vector<string>>> produkcije;

    ParserPravila();
    void printPravila();
};

#endif
