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
    if(!this->file_exists(this->filename)) {
        std::cerr << "File Not Found" << std::endl;
        exit(EXIT_FAILURE);
    }

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

void fi::Flesch_Index::Analyze() {
    this->syllable_count();
    this->word_count();
    this->sentence_count();

    this->_fi = 206.835 - (84.6 * (double)(this->Syllables() /
                (double)this->Words())) - (1.015 * (double)(this->Words() /
                (double)this->Sentences()));
}

void fi::Flesch_Index::syllable_count() {
    char vowels[] = {'a', 'e', 'i', 'u'};

    unsigned int syllables = 0;
    std::string delimiter = " ";

    for(auto sentence : this->sentences) {
        size_t pos = 0;
        std::string token;
        while((pos = sentence.find(delimiter)) != std::string::npos) {
            syllables++;
            token = sentence.substr(0, pos);

            //TODO: Find syllables
            int count = 0;
            for(auto c : token) {
                bool constant = 0;
                bool vowelFound = false;

                for(auto vowel : vowels) {
                    if(c == vowel) {
                        if(count == (token.size() - 1)) {
                            if(vowel == 'e') {
                                break;
                            }
                        }
                        vowelFound = true;
                        if(count == 0) {
                            syllables++;
                            break;
                        }
                        if(constant) {
                            syllables++;
                        }
                        break;
                    }
                }

                if(!vowelFound) {
                    constant = true;
                }else{
                    constant = false;
                }
                vowelFound = false;
                count++;
            }

            sentence.erase(0, pos + delimiter.length());
        }
    }

    this->_syllable_count = syllables;
}
void fi::Flesch_Index::word_count() {
    unsigned int words = 0;
    std::string delimiter = " ";

    for(auto sentence : this->sentences) {
        size_t pos = 0;
        std::string token;
        while((pos = sentence.find(delimiter)) != std::string::npos) {
            token = sentence.substr(0, pos);
            words++;
            sentence.erase(0, pos + delimiter.length());
        }
    }
    this->_word_count = words;
}
void fi::Flesch_Index::sentence_count() {
    unsigned int sentences = 0;
    sentences = (unsigned int)this->sentences.size();
    this->_sentence_count = sentences;
}

void fi::Flesch_Index::Print() {
    std::cout << "Syllables:\t" << this->Syllables() << std::endl;
    std::cout << "Words:\t\t" << this->Words() << std::endl;
    std::cout << "Sentences:\t" << this->Sentences() << std::endl;
    std::cout << "FI:\t\t" << this->FI() << std::endl;
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
