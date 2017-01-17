#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <sys/stat.h>

#include "flesch-index.hpp"

fi::Flesch_Index::Flesch_Index(std::string& filename) {
    this->filename = filename;

    this->Init();
}

void fi::Flesch_Index::Init() {

}

void fi::Flesch_Index::Read() {
    struct stat filestatus;
    stat(filename.c_str(), &filestatus );
    unsigned int filesize = filestatus.st_size;

    std::ifstream ifs(this->filename.c_str());

    if(ifs) {
        char c;
        std::string word = "";
        std::string sentence = "";
        int p_count = 0;
        while(ifs.get(c)) {
            switch(c) {
                case '.':
                case '?':
                case '!':
                    if(!word.empty()) {
                        if(!sentence.empty() && sentence.back() != ' ') {
                            sentence += ' ';
                        }
                        sentence += this->to_lower(this->strip(word));
                        word = "";
                    }
                    if(!sentence.empty()) {
                        this->sentences.push_back(sentence);
                        sentence = "";
                    }
                    break;
                case '\n':
                case ' ':
                    if(!sentence.empty() && sentence.back() != ' ') {
                        sentence += ' ';
                    }
                    sentence += this->to_lower(this->strip(word));
                    word = "";
                    break;
                default:
                    word += c;
                    break;
            }
        }
    }
}

/**
 * Simple check to make sure the file exists.
 */
bool fi::Flesch_Index::file_exists(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

/**
 * Strips a string of all characters that are not a member of alphanumeric
 */
std::string fi::Flesch_Index::strip(const std::string &s) {
    std::string result;
    result.reserve(s.length());

    std::remove_copy_if(s.begin(),
            s.end(),
            std::back_inserter(result),
            std::not1(std::ptr_fun(isalnum)));

    return result;
}

/**
 * Turns entire string into a lowercase version.
 */
std::string fi::Flesch_Index::to_lower(const std::string &s) {
    std::string result;
    result.reserve(s.length());

    std::transform(s.begin(),
            s.end(),
            std::back_inserter(result),
            ::tolower);

    return result;
}
