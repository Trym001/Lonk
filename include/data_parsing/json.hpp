//
// Created by zaime on 12/3/2022.
//

#ifndef LONK_JSON_HPP
#define LONK_JSON_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "data_parsing/received_data.hpp"


using json = nlohmann::json;


struct json_parsing{
public:
    static received_data convert(const json& msgData) {
        // get distance sensor values
        int left  = msgData["sensors"]["left"].get<int>();
        int front = msgData["sensors"]["front"].get<int>();
        int right = msgData["sensors"]["right"].get<int>();

        // possibly more values will be passed over...

        // putting data in an object that can be passed around to functions.
        received_data receivedData{};
        receivedData.distSensor.front = front;
        receivedData.distSensor.left  = left;
        receivedData.distSensor.right = right;

        return receivedData;
    }

    static received_data read_json(const std::string& msg2){
        return convert(json::parse(msg2));
    }

    static json write_json(const json& msg1){
        std::string dump = msg1.dump();
        return dump;
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
