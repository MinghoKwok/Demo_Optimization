#ifndef DEMO_HELPFUNC_HPP
#define DEMO_HELPFUNC_HPP

#include <iostream>
#include <regex>

using namespace std;

vector<string> getMatch(string reg, string inputStr);

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


#endif //DEMO_HELPFUNC_HPP
