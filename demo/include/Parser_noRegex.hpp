#ifndef DEMO_PARSER_NO_REGEX_H
#define DEMO_PARSER_NO_REGEX_H

#include <iostream>
#include <fstream>

using namespace std;

void match_no_regex(string dataPath);

void match_no_regex(string dataPath) {
    ifstream myfile (dataPath);
    string tempStr;

    if(!myfile){
        cout << "Unable to open myfile";
        exit(1); // terminate with error
    }

    while (getline(myfile, tempStr)) {

        tempStr.erase(0, tempStr.find_first_not_of(" \t"));

        if (tempStr[1] == '*' && tempStr[0] == '/') { // Original: if (tempStr.find("/*") != tempStr.npos)
            // match offset and assembly code
            string offset = tempStr.substr(2, tempStr.find("*/") - 2);

            // erase all text before assembly code
            tempStr.erase(0, tempStr.find_first_of(" "));
            tempStr.erase(0, tempStr.find_first_not_of(" "));

            // Extract assembly code
            string code = tempStr.substr(0, tempStr.find_first_of(";"));

            // Print
            //cout << offset << "  " << code << endl;
        }
    }
}

#endif //DEMO_PARSER_NO_REGEX_H
