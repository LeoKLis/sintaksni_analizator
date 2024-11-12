#include "simulator.h"

Simulator::Simulator(Deserializer d)
{
    this->d = d;
    stog.push({ new Node("0"), 0 });
}

void Simulator::strip(string& line)
{
    int front = 0;
    while (line[front] == ' ')
        front++;

    int back = line.length() - 1;
    while (line[back] == ' ')
        back--;

    line = line.substr(0, back + 1);
    line = line.substr(front);
}

void Simulator::parseLine(string line, string* uniform, int* rowNum, string* lexUnit)
{
    int spaceIndex = line.find(" ");
    if (spaceIndex == string::npos) {
        *uniform = line;
        return;
    }
    *uniform = line.substr(0, spaceIndex);
    line = line.substr(spaceIndex + 1);
    spaceIndex = line.find(" ");
    *rowNum = stoi(line.substr(0, spaceIndex));
    line = line.substr(spaceIndex + 1);
    strip(line);
    *lexUnit = line;
}

void Simulator::simulate()
{
    string uniform, lexUnit, line;
    int rowNum, spaceIndex;
    // ifstream file("../primjeri/primjeri/simplePpjLang_veci.in");
    getline(cin, line);
    parseLine(line, &uniform, &rowNum, &lexUnit);
    bool prihvat = false;
    while (!prihvat) {
        Pair akcija = d.tablica[stog.top().second][d.symbolIndex[uniform]];
        if (akcija.action == pomakni) { // Ok
            Node* leaf = new Node(line);
            stog.push({ leaf, akcija.stavka });
            if (getline(cin, line)) {
                parseLine(line, &uniform, &rowNum, &lexUnit);
            } else {
                line = "$";
                uniform = "$";
                rowNum = -1;
                lexUnit = "";
            }
        } else if (akcija.action == reduciraj) { // Ok
            string localUniform, localLexUnit;
            int localRowNum;
            vector<string> redux = d.stavke.at(akcija.stavka);
            string nameRedux = redux[0];
            Node* node = new Node(nameRedux);
            if (redux.size() == 2 && redux[1] == "$") {
                Node* epsNode = new Node("$");
                tree.connectNode(epsNode, node);
                int checkNum = stog.top().second;
                int outNum = d.tablica[checkNum][d.symbolIndex[nameRedux]].stavka;
                stog.push({ node, outNum });
                continue;
            }
            for (int i = redux.size() - 1; i >= 1; i--) {
                pair<Node*, int> stogTop = stog.top();
                parseLine(stogTop.first->contents, &localUniform, &localRowNum, &localLexUnit);
                if (localUniform != redux[i]) {
                    cout << "Greska! Nes se desilo! Gasim!" << endl;
                    // cin.close();
                    return;
                }
                tree.connectNode(stogTop.first, node);
                stog.pop();
            }
            int checkNum = stog.top().second;
            int outNum = d.tablica[checkNum][d.symbolIndex[nameRedux]].stavka;
            stog.push({ node, outNum });
        } else if (akcija.action == prihvati) { // Ok
            pair<Node*, int> stogTop = stog.top();
            tree.markAsRoot(stogTop.first);
            prihvat = true;
        } else if (akcija.action == odbaci) {
            while (true) {
                if (find(d.sinkronizacijskiZnakovi.begin(), d.sinkronizacijskiZnakovi.end(), uniform) != d.sinkronizacijskiZnakovi.end())
                    break;
                getline(cin, line);
                parseLine(line, &uniform, &rowNum, &lexUnit);
            }
            while ((d.tablica[stog.top().second][d.symbolIndex[uniform]]).action == odbaci) {
                stog.pop();
            }
        }
    }
    // cin.close();
    tree.traverse();
}
