#ifndef __PARSER_REGEX_H__
#define __PARSER_REGEX_H__

#include <string>
#include <vector>

class parser_regex {
public:
    parser_regex();
    ~parser_regex();
    void initial(std::string dataPath);
    std::vector<std::pair<std::string, std::string>> match_regex();
    std::vector<std::pair<std::string, std::string>> match_regex_multiThread(int threadCount);
private:
    int lineNum_;
    char** allLines_ = nullptr;
};
#endif //__PARSER_REGEX_H__
