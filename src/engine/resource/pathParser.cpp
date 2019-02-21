#include "pathParser.hpp"
#include "config/root.h"
#include <cstring>

string PathParser::parse(const string &path) {
    if (strlen(rootDirectory) == 0) {
        return path;
    }

    return rootDirectory + string("/") + path;
}
