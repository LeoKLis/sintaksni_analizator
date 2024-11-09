#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

enum Action {
    pomakni,
    reduciraj,
    prihvati,
    stavi,
    odbaci
};

struct Pair {
    Action action;
    int stavka;
    Pair(Action action, int stavka)
    {
        this->action = action;
        this->stavka = stavka;
    }
    Pair(int action, int stavka)
    {
        switch (action) {
        case 0:
            this->action = pomakni;
            break;
        case 1:
            this->action = reduciraj;
            break;
        case 2:
            this->action = prihvati;
            break;
        case 3:
            this->action = stavi;
            break;
        case 4:
            this->action = odbaci;
            break;
        default:
            cout << "Ubacen krivi index. Nije dobro";
            break;
        }
        this->stavka = stavka;
    }
};

class Deserializer {
private:
    void parseLine(vector<string>* arr, string line);
    void parseSymbIndex(map<string, int>* symbolIndex, string line);

public:
    vector<string> nezavrsniZnakovi;
    vector<string> zavrsniZnakovi;
    vector<string> sinkronizacijskiZnakovi;
    map<string, int> symbolIndex;
    map<int, vector<string>> stavke;
    vector<vector<Pair>> tablica;

    Deserializer();
    void printData();
};

#endif