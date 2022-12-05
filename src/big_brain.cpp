//
// Created by Bruker on 05.12.2022.
//

#include "controller/big_brain.hpp"
#include <string>
#include <chrono>
#include <thread>
using std::string, std::chrono::milliseconds;

std::string where_go::onwards(const int &heading, const int &yaw, const int &distFront) {
    // cv wait until receive-thread sends this_thread sends updated values (promise/future .get() can be useful)
    string lonkCommand{};
    if(heading == yaw and distFront > 150) {
        lonkCommand = "onwards";
    }   else {
        std::this_thread::sleep_for(milliseconds(10));
        // notify receive-thread new data is needed.
    }
    return lonkCommand;
}

std::string where_go::turn(const int &distLeft, const int &distRight) {
    // bzz bzzz msg from Lonk: "I haf stoopt" (condition variable from receiver-thread)

    string lonkCommand{};
    if(distLeft > 150){
        lonkCommand = "Left";
    }   else if(distRight > 150){
        lonkCommand = "Right";
    }   else{
        lonkCommand = "Dead_end";
    }
    return lonkCommand;
}

where_go::where_go() {
    // SPAWN THREAD THAT MAKES DECISIONS HERE
    // IT NEEDS TO HAVE AN ITERATION OF TCP CLASS THAT CAN SEND TO LONK
}

where_go::~where_go() {
    // JOIN THREAD THAT WAS SPAWNED!
}
