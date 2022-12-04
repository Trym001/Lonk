#include <iostream>
#include <string>
#include <fstream>
#include "data_parsing/json.hpp"
#include <nlohmann/json.hpp>
#include "tcp/tcp_client.hpp"
#include "controller/controller.hpp"
#include "controller/system_timer.hpp"

int main(int argc, char** argv) {
    std::string host = "10.25.47.143";
    std::string receiverPort = "9090";
    std::string senderPort = "9091";

    tcp_client get(host, receiverPort);
    system_timer stopWatch;
    json_parsing jsonParsing;
    controller regulator;

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

            // calling control-system to process the data
            // When it is implemented in the values sent from Lonk "yaw" will be gotten from "parsedRMessage"
            int yaw = 0;
            int heading = regulator.p_controller(
                    &parsedRMessage.distSensor.left,
                    &parsedRMessage.distSensor.right,
                    &yaw /* **yaw** */
                    );


        }
    }   catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}



