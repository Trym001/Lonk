#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "tcp/tcp_client.hpp"
#include "controller/system_timer.hpp"
#include "data_parsing/json.hpp"

int fun(int& a)
{
    int b  = 2;
    if(a + b == 4)
    {
        a = 3;
        b = 6;
    }
    return b;
}


int main(int argc, char **argv) {
    int a = 2;
    std::cout << fun(a) << " " << a;
}
