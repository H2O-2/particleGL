#include <string>

typedef std::string string;

class FileReader {
public:
    static string read(const string &path);
}
