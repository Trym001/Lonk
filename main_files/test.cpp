#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <condition_variable>
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
#include "controller/system_timer.hpp"

std::mutex m;
std::condition_variable cv;

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

        std::string receivedMessage;
        received_data parsedRMessage{};
        std::thread t1([&]{ // make a function in the tcp class that spawns the tread and runs this bit of
                                       // code repeatedly and makes the data available for the data processing thread.
            bool doBreak = false;
            while(true){
                // receive message from Lonk.
                receivedMessage = get.get_message();
                // convert to usable data (a struct containing distSensors)
                parsedRMessage = json_parsing::read_json(receivedMessage);
                if(doBreak){
                    break;
                }
            }
        });

        // tell Lonk where to go
        where_go whereGo(
                host,
                senderPort,
                parsedRMessage.drivingdata.heading,
                parsedRMessage.imu.yaw,
                parsedRMessage.distSensor.front,
                parsedRMessage.distSensor.left,
                parsedRMessage.distSensor.right
                );

        // Show GUI with camera feed and all lonk values we are receiving.
        // Allow a manual overwrite of lonk controls that terminates thread(s) that write to lonk in the program.

    }   catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



