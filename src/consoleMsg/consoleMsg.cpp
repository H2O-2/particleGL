#include "consoleMsg.hpp"

#include <iostream>

void ConsoleMsg::msg(string msg) {
    printMsg(msg, MsgColor::GREEN);
}

void ConsoleMsg::warningMsg(string msg) {
    printMsg("WARNING::" + msg, MsgColor::YELLOW);
}

void ConsoleMsg::errorMsg(string msg) {
    printMsg("ERROR::" + msg, MsgColor::RED);
}

void ConsoleMsg::printMsg(string msg, MsgColor color) {
    std::cout << "\033[1;" << color << "m" << msg << "\033[0m\n";
}
