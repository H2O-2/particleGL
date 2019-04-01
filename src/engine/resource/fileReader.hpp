#pragma once

#include "pathParser.hpp"

class FileReader {
public:
    static string readStr(const string &path);
    static unsigned char* readImage(const string &path, int* width, int* height, int* componentNum);
};
