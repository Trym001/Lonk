//
// Created by Bruker on 05.12.2022.
//

#ifndef LONK_PATH_FINDER_HPP
#define LONK_PATH_FINDER_HPP

#include <string>
#include <thread>
#include <memory>


struct where_go {
    // drive onwards
    std::string onwards(const int& heading, const int& yaw, const int& front);

    // bzz bzzz msg from Lonk: "I haf stoopt" (condition variable)
    // if venstre clear
    // else if høyre clear
    // else 180
    std::string turn(const int& left, const int& right, std::string& lonkCommand);

private:
};

#endif //LONK_PATH_FINDER_HPP
