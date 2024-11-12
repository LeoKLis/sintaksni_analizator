#include "serializer.h"

Serializer::Serializer(vector<vector<Pair>> table, map<int, vector<string>> stavke, vector<string> zavrsniZnakovi, vector<string> nezavrsniZnakovi, vector<string> sinkronizacijskiZnakovi, map<string, int> symbolIndex)
{
    ofstream file("analizator/input.txt");

    for(string el : nezavrsniZnakovi){
        file << el << " ";
    }
    file << "\n";

    for(string el : zavrsniZnakovi){
        file << el << " ";
    }
    file << "\n";

    for(string el : sinkronizacijskiZnakovi){
        file << el << " ";
    }
    file << "\n";

    for(auto el : symbolIndex){
        file << el.first << " " << el.second << " ";
    }
    file << "\n";

    for(auto el : stavke){
        file << el.first << " ";
        for(auto se : el.second){
            file << se << " ";
        }
        file << "\n";
    }
    file << "\n";

    for(auto it : table){
        for(Pair se : it){
            file << se.action << "," << se.stavka << " ";
        }
        file << "\n";
    }
}