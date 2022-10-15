#ifndef DEMO_HELPFUNC_HPP
#define DEMO_HELPFUNC_HPP

#include <iostream>
#include <regex>

#define SIMD_MATCH
#ifdef  SIMD_MATCH
#include "rejit.h"
char buffer[64*1024];
#endif

using namespace std;

vector<string> getMatch(string reg, string inputStr) {
    vector<string> res;
#ifdef  SIMD_MATCH
    rejit::Regej re(reg);
    vector<rejit::Match> matches;
    re.MatchAll(inputStr, &matches);
    size_t s = (matches[0].end - matches[0].begin)/sizeof(char);
    memcpy(buffer, matches[0].begin, s);
    buffer[s] = '\0';
    string matchRes(buffer);
    res.push_back(matchRes);
#else
    regex ip_reg(reg);
    smatch matchResult;

    if (regex_match(inputStr,matchResult,ip_reg)) {
        res.push_back((string)matchResult[0]);
//        for (size_t i = 1; i < matchResult.size(); ++i) {
//
//            res.push_back((string)matchResult[i]);
//        }
    } else {
        cout <<inputStr<<"-"<< "Not Match!" << endl;
    }
#endif

    return res;
}
//vector<string> rejitSIMD(string reg, string inputStr) {
//    rejit::Regej re(reg);
//    vector<rejit::Match> matches;
//
//    re.MatchAll(inputStr, &matches);
//}


#endif //DEMO_HELPFUNC_HPP
