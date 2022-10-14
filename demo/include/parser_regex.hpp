#ifndef DEMO_PARSER_REGEX_H
#define DEMO_PARSER_REGEX_H

#include "helpFunc.hpp"
#include <iostream>
#include <fstream>

class parser_regex {
public:
    static void match_regex();

    static void initial(string dataPath);

public:
    static vector<string> allLine;
};

void parser_regex::match_regex() {
    for (string str : allLine) {
        if (str.find("/*") != str.npos) {
            // match offset and assembly code
            vector<string> offset_code = getMatch("\\s*/\\*(.*)\\*/( +)(.*); (.*)", str);
            cout << offset_code[0] << "  " << offset_code[2] << endl;
        }
    }
}

void parser_regex::initial(string dataPath) {
    ifstream myfile(dataPath);
    string tempStr;

    if (!myfile) {
        cout << "Unable to open myfile";
        exit(1); // terminate with error
    }

    // Store
    while (getline(myfile, tempStr)) {
        allLine.push_back(tempStr);
    }
}

vector<string> parser_regex::allLine;



#endif //DEMO_PARSER_REGEX_H
