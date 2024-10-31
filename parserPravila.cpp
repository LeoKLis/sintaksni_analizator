#include "parserPravila.h"

ParserPravila::ParserPravila()
{
    string line = "";
    int razmakIndex;

    // nezavrsni znakovi gramatike
    getline(cin, line);
    line = line.substr(line.find_first_of(' ') + 1);
    while ((razmakIndex = line.find_first_of(' ')) != string::npos) {
        nezavrsniZnakovi.push_back(line.substr(0, razmakIndex));
        line = line.substr(razmakIndex + 1);
    }
    nezavrsniZnakovi.push_back(line);

    // zavrsni znakovi gramatike
    getline(cin, line);
    line = line.substr(line.find_first_of(' ') + 1);
    while ((razmakIndex = line.find_first_of(' ')) != string::npos) {
        zavrsniZnakovi.push_back(line.substr(0, razmakIndex));
        line = line.substr(razmakIndex + 1);
    }
    zavrsniZnakovi.push_back(line);

    // sinkronizacijski znakovi gramatike
    getline(cin, line);
    line = line.substr(line.find_first_of(' ') + 1);
    while ((razmakIndex = line.find_first_of(' ')) != string::npos) {
        sinkronizacijskiZnakovi.push_back(line.substr(0, razmakIndex));
        line = line.substr(razmakIndex + 1);
    }
    sinkronizacijskiZnakovi.push_back(line);

    string trenutnaProdukcija;
    while (getline(cin, line)) {
        if (line[0] != ' ') {
            trenutnaProdukcija = line;
            continue;
        }
        line = line.substr(1);

        vector<string> novoPravilo;
        while ((razmakIndex = line.find_first_of(' ')) != string::npos) {
            novoPravilo.push_back(line.substr(0, razmakIndex));
            line = line.substr(razmakIndex + 1);
        }
        novoPravilo.push_back(line);
        produkcije[trenutnaProdukcija].push_back(novoPravilo);
    }
}

void ParserPravila::printPravila()
{
    for (auto it : nezavrsniZnakovi) {
        cout << it << ", ";
    }
    cout << "\n\n";
    for (auto it : zavrsniZnakovi) {
        cout << it << ", ";
    }
    cout << "\n\n";
    for (auto it : sinkronizacijskiZnakovi) {
        cout << it << ", ";
    }
    cout << "\n\n";
    for (auto it = produkcije.cbegin(); it != produkcije.cend(); it++) {
        for (auto se : it->second) {
            cout << it->first << " -> ";
            for (auto th : se) {
                cout << th << " ";
            }
            cout << endl;
        }
    }
}