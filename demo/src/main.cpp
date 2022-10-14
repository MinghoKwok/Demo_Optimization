#include <iostream>
#include <ctime>
#include "../include/parser_regex.h"

using namespace std;

int main() {
    string dataPath = "/Users/kwok/Documents/2022_Summer/Materials/castro.head_10000.txt";
    parser_regex* pr = new parser_regex();
    pr->initial(dataPath);

    pr->match_regex();

    pr->match_regex_multiThread(4);

    return 0;
}
