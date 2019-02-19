#include "fileReader.hpp"

#include <fstream>
#include <sstream>

#include "consoleMsg/consoleMsg.hpp"

string FileReader::read(const string &path) {
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        fileStream.open(path.c_str());
        ss << fileStream.rdbuf();
        fileStream.close();
    }
    catch (std::ifstream::failure e) {
        ConsoleMsg::errorMsg("FAILED TO READ FILE AT " + path);
        exit(-1);
    }

    return ss.str();
}
