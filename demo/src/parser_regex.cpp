#include <iostream>
#include <fstream>
#include <thread>
#include <string.h>
#include <algorithm>
#include <filesystem>
#include <stdio.h>
#include <omp.h>
#include "help_func.hpp"
#include "parser_regex.h"

using namespace std;

#define REGEX_EXPRESION "\\s*/\\*(.*)\\*/( +)(.*); (.*)"

parser_regex::parser_regex() {

}

parser_regex::~parser_regex() {
    if(allLines_ != nullptr) {
        for (int i = 0; i < lineNum_; i++) {
            free(allLines_[i]);
        }
        free(allLines_);
    }
}

void parser_regex::initial(string dataPath) {
//    ifstream inFile(dataPath);
//    if (!inFile) {
//        cout << "Unable to open " << dataPath << endl;
//        exit(1); // terminate with error
//    }

    // Read and Store all lines in input file
    char buf[64 * 1024];        // Buffer for fgets()
    FILE *fp;
    fp = fopen(dataPath.c_str(), "r");
    vector<char*> lines;        // Store the line temporarily

    // Iterate all lines
    while (!feof(fp)) {
        fgets(buf, sizeof buf, fp);
        string str(buf);
        const char* tempCStr = str.c_str();
//        cout << tempCStr << endl;
        size_t tempStrSize = str.size();
        char* line = (char*) malloc(tempStrSize * sizeof (char));
        lines.push_back(line);
        memcpy(line, tempCStr, tempStrSize);
        line[tempStrSize-1] = '\0';                                    // replace '\n'
    }

    // Update member variables
    lineNum_ = lines.size();
    allLines_ = (char**) malloc (lineNum_*sizeof (char *));
    for (int i = 0; i < lineNum_; i++) {
        allLines_[i] = lines[i];
    }
}

std::vector<std::pair<std::string, std::string>> parser_regex::match_regex() {
    std::vector<std::pair<std::string, std::string>> store;           // This type is to do further job (extract offset and code from one line) easily
    for (int i = 0; i < lineNum_; i++) {
        string str(allLines_[i]);
        if (str.find("/*") != str.npos) {   // To avoid unuseful matching
            vector<string> offset_code = get_match(REGEX_EXPRESION,str);
            store.push_back(pair<string, string>(offset_code[0], offset_code[0]));
        }
    }
    return store;
}

std::vector<std::pair<std::string, std::string>> parser_regex::match_regex_multiThread(int threadCount) {
    std::vector<std::pair<std::string, std::string>> store;
    // Matching function, call in each thread
    auto match = [&](int threadId, int dataIdxStart, int dataIdxEnd, char** data, std::vector<std::pair<std::string, std::string>>* store, mutex* m){
//        m->lock();
//        cout<<threadId<<endl;
//        cout<<dataIdxStart << "-" << dataIdxEnd <<endl;
//        m->unlock();
        vector<pair<string, string>> store_part;
        for (int j = dataIdxStart; j < dataIdxEnd; j++) {
            string str(data[j]);
            if (str.find("/*") != str.npos) {
                vector<string> offset_code = get_match(REGEX_EXPRESION, str);
                //cout << offset_code[0] << "  " << offset_code[2] << endl;
                store_part.push_back(pair<string, string>(offset_code[0], offset_code[0]));
            }
        }

        // Lock, store the result safely
        m->lock();
        store->insert((*store).end(), store_part.begin(), store_part.end());
        m->unlock();
    };

    int step = lineNum_ / threadCount;  // The number of lines needed to be matched in each thread
    mutex m;
    std::vector<thread> threads;
    for (int i = 0; i < threadCount; i++) {
        int start = i * step;                                                       // The start position in allLines_ in current thread
        int end = i == (threadCount - 1) ? lineNum_ : start + step;                 // Avoid out of bound
        threads.push_back(thread(match, i, start, end, allLines_, &store, &m));   // Run each thread
    }

    // Join threads
    for_each(threads.begin(), threads.end(),
                  [](thread &t)
                  {
                      t.join();
                  }
    );
    return store;
}

std::vector<std::pair<std::string, std::string>> parser_regex::match_regex_OpenMP(int threadCount) {
    std::vector<std::pair<std::string, std::string>> store;
    omp_set_num_threads(threadCount);
    std::vector<std::vector<std::pair<std::string, std::string>>>  store_thread(4); // Store results of each thread distinctly

#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < lineNum_; i++) {
            string str(allLines_[i]);
            if (str.find("/*") != str.npos) {
                vector<string> offset_code = get_match(REGEX_EXPRESION,str);
                //cout<< offset_code[0] << ":" << offset_code[2] <<endl;
                store_thread[omp_get_thread_num()].push_back(pair<string, string>(offset_code[0], offset_code[0])); // Store in the corresponding position of array by num of thread
            }
        }
    }

    // Out of omp, combine results of each thread
    for (std::vector<std::pair<std::string, std::string>> st : store_thread) {
        store.insert(store.end(), st.begin(), st.end());
    }

    return store;
}

std::vector<std::pair<std::string, std::string>> parser_regex::match_noRegex_OpenMP(int threadCount) {
    std::vector<std::pair<std::string, std::string>> store;
    omp_set_num_threads(threadCount);
    std::vector<std::vector<std::pair<std::string, std::string>>>  store_thread(4);

    // OpenMP
#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < lineNum_; i++) {
            string str(allLines_[i]);

            if (str.empty())
                continue;

            // Define string variable of what we want to extract
            string offset;
            string code;


            // (1) Erase all space " " before other characters.
            str.erase(0, str.find_first_not_of(" "));


            // (2) Judge if the head of str is "/*"
            // If the conditional statement is true, we could confirm that this str is what we need due to the format of the input file.
            if (str.size() >= 0 && str[1] == '*' && str[0] == '/') {

                // (3) Match offset and assembly code
                // Then we could gain the content between "/*" and "*/" by their positions in str.
                auto pos1 = str.find("*/");
                if (pos1 == str.npos) {
                    cout << "Format Wrong: No offset." << endl;
                    continue;	// Remind: the code we analyze now is in a for loop, which is ommitted
                }
                else {
                    offset = str.substr(2, pos1 - 2);
                }


                // (4) Erase all text before assembly code
                auto pos2 = str.find_first_of(" ");	// There must be space between "*/" and assembly code.
                if (pos2 == str.npos) {
                    cout << "Format Wrong: No space between offset and assembly code." << endl;
                    continue;
                } else {
                    str.erase(0, pos2);
                }
                // Erase all space before assembly code.
                auto pos3 = str.find_first_not_of(" ");
                if (pos3 == str.npos) {
                    cout << "Format Wrong: No assembly code after offset." << endl;
                    continue;
                } else {
                    str.erase(0, pos3);
                }


                // (5) Extract assembly code
                // By finding the position of ";", we could confirm the range of the sub string corresponding to code.
                auto pos4 = str.find_first_of(";");
                if (pos4 == str.npos) {
                    cout << "Format Wrong: No assembly code after offset." << endl;
                    continue;
                } else {
                    code = str.substr(0, pos4);
                }

                // Finally, store them 
                store_thread[omp_get_thread_num()].push_back(pair<string, string>(offset, code));
            }

        }
    }

    for (std::vector<std::pair<std::string, std::string>> st : store_thread) {
        store.insert(store.end(), st.begin(), st.end());
    }

    return store;
}





