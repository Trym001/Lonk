//
// Created by zaime on 12/3/2022.
//

#ifndef LONK_JSON_HPP
#define LONK_JSON_HPP

#include <string>
#include <nlohmann/json.hpp>


using json = nlohmann::json;


struct json_parsing{
public:
    static json write_json(json msg1){
        std::string dump = msg1.dump();
        return dump;
    }

    static json read_json(std::string msg2){
        return json::parse(msg2);
    }

private:
};

/*
 * send medling
json j = "{\"speed\": 6000, \"heading\": 330}"_json;
std::string client_message = j.dump();
std::string msg = client_message; */

/*
 * få melding
    json server_msg = json::parse(msg);
    //std::cout << "Got reply from server: " << (server_msg["left"] == 12) << std::endl;
    std::cout << "Got reply from server: " << (server_msg) << std::endl;
 */
#endif //LONK_JSON_HPP
