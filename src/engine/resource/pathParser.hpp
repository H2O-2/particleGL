#include <string>

typedef std::string string;

// Convert relative path to absolute path

class PathParser {
public:
    static string parse(const string &path);
};
