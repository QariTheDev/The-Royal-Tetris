#pragma once

#include<fstream>
#include <iostream>

using namespace std;

void saveNameToFile(string& name) {
    ofstream fout;
    fout.open("top_5_list.txt", ios::app);

    if (fout.is_open()) {
        fout << name << "\t\t"; //temporarily
        fout.close();
    }
    else {
        cout << "Error!!! Unable to open file" << endl;
    }
}

void saveScoreToFile(int& score) {
    ofstream fout;
    fout.open("top_5_list_txt", ios::app);

    if (fout.is_open()) {
        fout << score << endl;
        fout.close();
    }
    else {
        cout << "Error!!! Unable to open file" << endl;
    }
}

vector<string> readFromFile(string fileName) {
    vector<string> contents;
    ifstream fin(fileName);

    if (fin.is_open()) {
        string line;

        while (getline(fin, line)) {
            contents.push_back(line);
        }
        fin.close();
    }

    return contents;
}
