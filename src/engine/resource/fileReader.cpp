#include "fileReader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <fstream>
#include <sstream>

#include <vector>

#include "consoleMsg/consoleMsg.hpp"

string FileReader::readStr(const string &path) {
    std::ifstream fileStream;
    std::stringstream ss;
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        fileStream.open(PathParser::parse(path));
        ss << fileStream.rdbuf();
        fileStream.close();
    }
    catch (std::ifstream::failure e) {
        ConsoleMsg::errorMsg("FAILED TO READ FILE AT " + path);
        exit(-1);
    }

    return ss.str();
}

unsigned char* FileReader::readImage(const string &path, int* width, int* height, int* componentNum) {
    // Flip the texture vertically
    stbi_set_flip_vertically_on_load(true);

    string test = PathParser::parse(path);
    unsigned char* textureData = stbi_load(PathParser::parse(path).c_str(), width, height, componentNum, 0);

    return textureData;
}
