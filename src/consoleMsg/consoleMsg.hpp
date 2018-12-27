#pragma once

#include <string>

using std::string;

class ConsoleMsg {
public:
    static void msg(string msg);
    static void warningMsg(string msg);
    static void errorMsg(string msg);
private:
    enum MsgColor {
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        DEFAULT  = 39,
    };

    ConsoleMsg(); // This class should be solely static
    static void printMsg(string msg, MsgColor color);
};
