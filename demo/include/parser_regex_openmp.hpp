#ifndef DEMO_PARSER_REGEX_OPENMP_HPP
#define DEMO_PARSER_REGEX_OPENMP_HPP

#include "helpFunc.hpp"
#include <iostream>
#include <fstream>


class parser_regex_openmp {
public:
    static void match_regex_openmp();
    static void initial(string dataPath);

public:
    static vector<string> allLine;
};

void parser_regex_openmp::initial(string dataPath) {
    ifstream myfile (dataPath);
    string tempStr;

    if(!myfile){
        cout << "Unable to open myfile";
        exit(1); // terminate with error
    }

    // Store
    while (getline(myfile, tempStr)) {
        allLine.push_back(tempStr);
    }
}

void parser_regex_openmp::match_regex_openmp() {
#pragma omp parallel for
    for (string str : allLine) {
        if (str.find("/*") != str.npos) {
            // match offset and assembly code
            vector<string> offset_code = getMatch("\\s*/\\*(.*)\\*/( +)(.*); (.*)", str);
            cout << offset_code[0] << "  " << offset_code[2] << endl;
        }
    }
}

vector<string> parser_regex_openmp::allLine;

#endif //DEMO_PARSER_REGEX_OPENMP_HPP
