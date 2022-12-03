//
// Created by zaime on 12/3/2022.
//

//#include "controller/system_timer.hpp"
//#include "controller/controller.hpp"
#include <iostream>
#include "other/json.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

int main(){


    json_parsing mamma;

    std::string write = mamma.write_json("{\"speed\": 6000, \"heading\": 330}");

    json read = mamma.read_json(write);

    std::cout << (read["speed"]) << std::endl;

    return 0;
}



