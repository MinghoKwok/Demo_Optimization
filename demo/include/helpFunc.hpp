#ifndef DEMO_HELPFUNC_HPP
#define DEMO_HELPFUNC_HPP

#include <iostream>
#include <regex>

#include "rejit.h"

using namespace std;

vector<string> original(string reg, string inputStr);
vector<string> rejitSIMD(string reg, string inputStr);  // SIMD

vector<string> getMatch(string reg, string inputStr) {
    return original(reg, inputStr);

}

vector<string> original(string reg, string inputStr) {
    regex ip_reg(reg);
    smatch matchResult;
    vector<string> res;

    if (regex_match(inputStr,matchResult,ip_reg)) {
        for (size_t i = 1; i < matchResult.size(); ++i) {
            res.push_back((string)matchResult[i]);
        }
    } else {
        cout <<inputStr<<"-"<< "Not Match!" << endl;
    }

    return res;
}

//vector<string> rejitSIMD(string reg, string inputStr) {
//    rejit::Regej re(reg);
//    vector<rejit::Match> matches;
//
//    re.MatchAll(inputStr, &matches);
//}


#endif //DEMO_HELPFUNC_HPP
