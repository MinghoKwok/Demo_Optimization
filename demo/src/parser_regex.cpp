#include <iostream>
#include <fstream>
#include <thread>
#include <string.h>
#include <algorithm>
#include <filesystem>
#include <stdio.h>
#include "../include/helpFunc.hpp"
#include "../include/parser_regex.h"
#include "/usr/local/opt/libomp/include/omp.h"

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

    // Buffer
    char buf[64 * 1024];

    FILE *fp;
    fp = fopen(dataPath.c_str(), "r");

    // Store

    vector<char*> lines;
    while (!feof(fp)) {
        fgets(buf, sizeof buf, fp);
        string tempStr(buf);

        const char* tempCStr = tempStr.c_str();
//        cout << tempCStr << endl;
        size_t tempStrSize = tempStr.size();
        char* line = (char*) malloc(tempStrSize * sizeof (char));
        lines.push_back(line);
        memcpy(line, tempCStr, tempStrSize);
        line[tempStrSize-1] = '\0';
    }
    lineNum_ = lines.size();
    allLines_ = (char**) malloc (lineNum_*sizeof (char *));
    for (int i = 0; i < lineNum_; i++) {
        allLines_[i] = lines[i];
    }
}

std::vector<std::pair<std::string, std::string>> parser_regex::match_regex() {
    std::vector<std::pair<std::string, std::string>> store;
    for (int i = 0; i < lineNum_; i++) {
        string str(allLines_[i]);
//        cout<< str << "-"<<endl;
        if (str.find("/*") != str.npos) {
//            cout<< str << "-";
            vector<string> offset_code = getMatch(REGEX_EXPRESION,str);
//            cout<< offset_code[0] << ":" << offset_code[2] <<endl;
            store.push_back(pair<string, string>(offset_code[0], offset_code[2]));
        }
    }
    return store;
}

std::vector<std::pair<std::string, std::string>> parser_regex::match_regex_multiThread(int threadCount) {
    std::vector<std::pair<std::string, std::string>> store;
    auto match = [&](int threadId, int dataIdxStart, int dataIdxEnd, char** data, std::vector<std::pair<std::string, std::string>>* store, mutex* m){
        m->lock();
        cout<<threadId<<endl;
        cout<<dataIdxStart << "-" << dataIdxEnd <<endl;
        m->unlock();
        vector<pair<string, string>> store_part;
        for (int j = dataIdxStart; j < dataIdxEnd; j++) {
            string str(data[j]);
            if (str.find("/*") != str.npos) {
                vector<string> offset_code = getMatch(REGEX_EXPRESION, str);
                //cout << offset_code[0] << "  " << offset_code[2] << endl;
                store_part.push_back(pair<string, string>(offset_code[0], offset_code[2]));
            }
        }
        m->lock();
        store->insert((*store).end(), store_part.begin(), store_part.end());
        m->unlock();
    };
    int step = lineNum_ / threadCount;
    mutex m;
    std::vector<thread> threads;
    for (int i = 0; i < threadCount; i++) {
        int start = i * step;
        int end = i == (threadCount - 1) ? lineNum_ : start + step;
        threads.push_back(thread(match, i, start, end, allLines_, &store, &m));
    }
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
    std::vector<std::vector<std::pair<std::string, std::string>>>  store_thread(4);

#pragma omp parallel
    {
#pragma omp for
        for (int i = 0; i < lineNum_; i++) {
            string str(allLines_[i]);
//        cout<< str << "-"<<endl;
            if (str.find("/*") != str.npos) {
//            cout<< str << "-" << endl;
                vector<string> offset_code = getMatch(REGEX_EXPRESION,str);
                //cout<< offset_code[0] << ":" << offset_code[2] <<endl;
                store_thread[omp_get_thread_num()].push_back(pair<string, string>(offset_code[0], offset_code[2]));
            }
        }
    }
    for (std::vector<std::pair<std::string, std::string>> st : store_thread) {
        store.insert(store.end(), st.begin(), st.end());
    }

    return store;
}







