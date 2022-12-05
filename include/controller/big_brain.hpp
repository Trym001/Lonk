//
// Created by Bruker on 05.12.2022.
//

#ifndef LONK_BIG_BRAIN_HPP
#define LONK_BIG_BRAIN_HPP

#include <string>
#include "tcp/tcp_client.hpp"


struct where_go {
    where_go(std::string  host, std::string  port, const int &heading, const int &yaw, const int &distFront,
             const int &distLeft, const int &distRight);

    virtual ~where_go();

    // drive onwards
    std::string onwards(const int& heading, const int& yaw, const int& distFront);

    // bzz bzzz msg from Lonk: "I haf stoopt" (condition variable from )
    // if venstre clear
    // else if høyre clear
    // else 180
    std::string turn(const int& distLeft, const int& distRight);

private:
    std::string host_;
    std::string senderPort_;
};


#endif //LONK_BIG_BRAIN_HPP
