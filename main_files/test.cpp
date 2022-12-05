#include <iostream>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
#include "tcp/tcp_client.hpp"
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
#include "controller/system_timer.hpp"

int main(int argc, char** argv) {
    std::string host = "10.25.47.143";
    std::string receiverPort = "9090";
    std::string senderPort = "9091";

    tcp_client get(host, receiverPort);
    system_timer stopWatch;
    json_parsing jsonParsing;

    try
    {
        // establish connection with Lonk (RVR/raspberry).
        get.listen();
        while(true)
        {
            // receive message from Lonk.
            auto receivedMessage = get.get_message();
            // convert to usable data (a struct containing distSensors)
            received_data parsedRMessage = json_parsing::read_json(receivedMessage);

            // tell Lonk where to go
            where_go whereGo;
            whereGo.onwards(
                    parsedRMessage.drivingdata.heading,
                    parsedRMessage.imu.yaw,
                    parsedRMessage.distSensor.front
            );
            whereGo.turn(
                    parsedRMessage.distSensor.left,
                    parsedRMessage.distSensor.right
                    );

        }
    }   catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}



