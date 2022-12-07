//
// Created by Bruker on 05.12.2022.
//
#include "data_parsing/received_data.hpp"
#include <string>
#include <chrono>
#include <thread>
#include <utility>
#include <memory>
#include <mutex>
#include "controller/big_brain.hpp"
using std::string, std::chrono::milliseconds;

std::string where_go::onwards(const int &heading, const int &yaw, const int &distFront) {
    // cv wait until receive-thread sends this_thread sends updated values (promise/future .get() can be useful)
    string lonkCommand{};
    if(heading == yaw and distFront > 150) {
        lonkCommand = "onwards";
    }
    return lonkCommand;
}

std::string where_go::turn(const int &distLeft, const int &distRight, string& lonkCommand) {
    if(std::empty(lonkCommand)) {
        if (distLeft > 150) {
            lonkCommand = "Left";
        } else if (distRight > 150) {
            lonkCommand = "Right";
        } else {
            lonkCommand = "Dead_end";
        }
    }
    return lonkCommand;
}