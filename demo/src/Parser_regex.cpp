
#include "../include/Parser_regex.h"
#include <fstream>
#include <regex>

using namespace std;

vector<string> getMatch(string reg, string inputStr) {
    regex ip_reg(reg);
    smatch matchResult;
    vector<string> res;

    if (regex_match(inputStr,matchResult,ip_reg))
    {
        for (size_t i = 1; i < matchResult.size(); ++i)
        {
            res.push_back((string)matchResult[i]);
        }
    } else {
        //cout << "Not Match!";
    }

    return res;
}

void match_regex(string dataPath) {
    ifstream myfile (dataPath);
    string tempStr;

    if(!myfile){
        cout << "Unable to open myfile";
        exit(1); // terminate with error
    }

    while (getline(myfile, tempStr)) {
        //cout << tempStr << endl;

        if (tempStr.find("/*") != tempStr.npos) {
            // match offset and assembly code
            vector<string> offset_code = getMatch("\\s*/\\*(.*)\\*/( +)(.*); (.*)", tempStr);
            cout << offset_code[0] << "  " << offset_code[2] << endl;
        }

    }
}