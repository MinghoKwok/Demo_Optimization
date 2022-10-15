#include <iostream>
#include "parser_regex.h"

using namespace std;

int main() {
    string dataPath = "../dataset/castro.head_10000.txt";
    parser_regex* pr = new parser_regex();
    pr->initial(dataPath);

    pr->match_regex();

//    pr->match_regex_multiThread(4);

//    pr->match_regex_OpenMP(4);

//    pr->match_noRegex_OpenMP(4);

    return 0;
}
