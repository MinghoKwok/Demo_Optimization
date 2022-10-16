#ifndef DEMO_HELP_FUNC_HPP
#define DEMO_HELP_FUNC_HPP

#include <iostream>
#include <regex>

#define SIMD_MATCH      // delete this line if SIMD is not needed
#ifdef  SIMD_MATCH
#include "rejit.h"
char buffer[64*1024];
#endif

using namespace std;

vector<string> get_match(string reg, string inputStr) {
    vector<string> res;
#ifdef  SIMD_MATCH
    rejit::Regej re(reg);
    vector<rejit::Match> matches;   // match results
    // match by RE
    re.MatchAll(inputStr, &matches);
    // transform to string
    size_t s = (matches[0].end - matches[0].begin)/sizeof(char);
    memcpy(buffer, matches[0].begin, s);
    buffer[s] = '\0';
    string matchRes(buffer);
    // store
    res.push_back(matchRes);
#else
    regex ip_reg(reg);
    smatch matchResult;

    if (regex_match(inputStr,matchResult,ip_reg)) {
        res.push_back((string)matchResult[0]);

        // below is to extract the offset and code when we need
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


#endif //DEMO_HELP_FUNC_HPP
