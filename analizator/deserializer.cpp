#include "deserializer.h"

Deserializer::Deserializer()
{
    ifstream file("./input.txt");
    string line;
    getline(file, line);
    parseLine(&nezavrsniZnakovi, line);

    getline(file, line);
    parseLine(&zavrsniZnakovi, line);

    getline(file, line);
    parseLine(&sinkronizacijskiZnakovi, line);

    while (getline(file, line)) {
        if (line[line.size() - 1] != ' ') line.push_back(' ');
        int spaceIndex;
        int colIndex = 0;
        vector<Pair> tempVec;
        string colChar;
        while ((spaceIndex = line.find(" ")) != string::npos) {
            colChar = nezavrsniZnakovi[colIndex];
            string el = line.substr(0, spaceIndex);
            if (el == "-"){
                colIndex++;
                line = line.substr(spaceIndex + 1);
                continue;
            }
            // tempVec.push_back(); Stao
            line = line.substr(spaceIndex + 1);
            colIndex++;
        }
        tablica.push_back(tempVec);
    }

    file.close();

    generateSymbMap();
}

void Deserializer::parseLine(vector<string>* arr, string line)
{
    if (line[line.size() - 1] != ' ')
        line.push_back(' ');
    int spaceIndex;
    while ((spaceIndex = line.find(" ")) != string::npos) {
        arr->push_back(line.substr(0, spaceIndex));
        line = line.substr(spaceIndex + 1);
    }
}

void Deserializer::generateSymbMap(){
    int count = 0;
    for(auto it : zavrsniZnakovi)
        symbolIndex.insert({it, count++});
    
    symbolIndex.insert({"$", count++});

    for(auto it : nezavrsniZnakovi)
        symbolIndex.insert({it, count++});
}

void Deserializer::printData()
{
    for (auto it : nezavrsniZnakovi) {
        cout << it << " ";
    }
    cout << endl;
    for (auto it : zavrsniZnakovi) {
        cout << it << " ";
    }
    cout << endl;
    for (auto it : sinkronizacijskiZnakovi) {
        cout << it << " ";
    }
    cout << endl;

    int counter = 0;
    for (auto it : akcija) {
        cout << counter << " ";
        for (auto se : zavrsniZnakovi) {
            if (akcija[counter].count(se) == 0)
                cout << se << " -> -\t";
            else
                cout << se << " -> " << akcija[counter].at(se) << "\t";
        }
        if (akcija[counter].count("$") == 0)
                cout << "$ -> -\t";
            else
                cout <<"$ -> " << akcija[counter].at("$") << "\t";
        counter += 1;
        cout << endl;
    }
    cout << endl;
    counter = 0;
    for (auto it : novoStanje) {
        cout << counter << " ";
        for (auto se : nezavrsniZnakovi) {
            if (novoStanje[counter].count(se) == 0)
                cout << se << " -> -\t";
            else
                cout << se << " -> " << novoStanje[counter].at(se) << "\t";
        }
        counter += 1;
        cout << endl;
    }
}

