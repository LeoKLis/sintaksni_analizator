#include "deserializer.h"

Deserializer::Deserializer()
{
    ifstream file("input.txt");
    string line;
    getline(file, line);
    parseLine(&nezavrsniZnakovi, line);

    getline(file, line);
    parseLine(&zavrsniZnakovi, line);

    getline(file, line);
    parseLine(&sinkronizacijskiZnakovi, line);

    getline(file, line);
    parseSymbIndex(&symbolIndex, line);

    while (getline(file, line)) {
        if (line.length() == 0)
            break;
        if (line[line.size() - 1] != ' ')
            line.push_back(' ');
        int spaceIndex = line.find(" ");
        int idx = stoi(line.substr(0, spaceIndex));
        line = line.substr(spaceIndex + 1);
        vector<string> production;
        while ((spaceIndex = line.find(" ")) != string::npos) {
            string prodEl = line.substr(0, spaceIndex);
            line = line.substr(spaceIndex + 1);
            production.push_back(prodEl);
        }
        stavke.insert({ idx, production });
    }

    while (getline(file, line)) {
        if (line[line.size() - 1] != ' ')
            line.push_back(' ');
        int spaceIndex;
        vector<Pair> tempVec;
        while ((spaceIndex = line.find(" ")) != string::npos) {
            string el = line.substr(0, spaceIndex);
            int commaIndex = el.find(",");
            int act = stoi(el.substr(0, commaIndex));
            int sta = stoi(el.substr(commaIndex + 1));
            tempVec.push_back(Pair(act, sta));
            line = line.substr(spaceIndex + 1);
        }
        tablica.push_back(tempVec);
    }

    file.close();


    for(auto i:nezavrsniZnakovi)
        indexToSymbol.push_back(i);
    for(auto i: zavrsniZnakovi)
        indexToSymbol.push_back(i);


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

void Deserializer::parseSymbIndex(map<string, int>* symbolIndex, string line)
{
    if (line[line.size() - 1] != ' ')
        line.push_back(' ');
    int spaceIndex;
    while ((spaceIndex = line.find(" ")) != string::npos) {
        string symb = line.substr(0, spaceIndex);
        line = line.substr(spaceIndex + 1);
        spaceIndex = line.find(" ");
        int idx = stoi(line.substr(0, spaceIndex));
        line = line.substr(spaceIndex + 1);
        symbolIndex->insert({ symb, idx });
    }
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

    for (auto it : symbolIndex) {
        cout << it.first << " " << it.second << endl;
    }
    cout << endl;

    for (auto it : stavke) {
        cout << it.first << ": ";
        cout << it.second[0] << " -> ";
        for (auto se = it.second.begin() + 1; se != it.second.end(); se++) {
            cout << *se << " ";
        }
        cout << endl;
    }

    int counter = 0;
    for (auto it : tablica) {
        cout << counter << ": ";
        for (auto se : it) {
            cout << se.action << "," << se.stavka << " ";
        }
        cout << endl;
        counter += 1;
    }
}
