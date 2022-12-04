#include <iostream>
#include "other/json.hpp"
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include "controller/controller.hpp"
#include "tcp/tcp_client.hpp"

int main(int argc, char** argv) {
    std::string host = "10.25.47.143";
    std::string receiverPort = "9090";
    std::string senderPort = "9091";

    tcp_client get(host, recieverPort);
    system_timer stopWatch;
    json_parsing jsonParsing;

    try
    {
        get.listen();
        while(true)
        {
            auto receivedMessage = get.get_message();
            receivedMessage = jsonParsing::read_json(receivedMessage);

        }
    }
}



