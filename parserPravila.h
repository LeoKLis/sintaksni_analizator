#ifndef PARSER_PRAVILA_H
#define PARSER_PRAVILA_H

#include<iostream>
#include<string>
#include<vector>
#include<map>

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
