//
// Created by Bruker on 05.12.2022.
//
#include "data_parsing/received_data.hpp"
#include <string>
#include <chrono>
#include <memory>
#include "controller/big_brain.hpp"

using std::string, std::chrono::milliseconds;

string where_go::onwards(const int &heading, const int &yaw, const int &front) {
    // cv wait until receive-thread sends this_thread sends updated values (promise/future .get() can be useful)
    string lonkCommand = "not_onwards";
    if (front > 200) {
        lonkCommand = "onwards";
    }
    return lonkCommand;
}

string where_go::turn(const int &left, const int &right, string &lonkCommand) {
    if (lonkCommand == "not_onwards") {
        if (left > 300) {
            lonkCommand = "Left";
        } else if (right > 300) {
            lonkCommand = "Right";
        } else {
            lonkCommand = "Dead_end";
        }
    }
    return lonkCommand;
}