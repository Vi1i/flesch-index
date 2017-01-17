#ifndef FLESCH_INDEX
#define FLESCH_INDEX

#include <string>
#include <vector>

namespace fi {
    class Flesch_Index {
    public:
        Flesch_Index(std::string& filename);
        void Read();
    private:
        std::string filename;
        std::vector<std::string> sentences;

        void Init();
        bool file_exists(const std::string& name);
        std::string strip(const std::string &s);
        std::string to_lower(const std::string &s);

    };
}
#endif
