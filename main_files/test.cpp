#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <condition_variable>
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
#include "controller/system_timer.hpp"

int main(int argc, char** argv) {
    std::string host = "10.25.47.143";
    std::string receiverPort = "9090";
    std::string senderPort = "9091";

    tcp_client get(&host, receiverPort);
    system_timer stopWatch;
    json_parsing jsonParsing;

    std::promise<received_data> promise;
    // std::shared_ptr<std::future<received_data>> (future) = std::make_shared<std::future<received_data>>(promise.get_future());
    auto future = promise.get_future();
    std::condition_variable cv;
    std::mutex m;

    try
    {
        // establish connection with Lonk (RVR/raspberry).
        get.listen();

        std::string receivedMessage;
        received_data parsedRMessage{};
        std::thread t1([&]{
            while(true){
                // receive message from Lonk.
                receivedMessage = get.get_message();
                {
                    const std::lock_guard<std::mutex> lock(m);
                    // convert to usable data (a struct containing distSensors)
                    parsedRMessage = json_parsing::read_json(receivedMessage);
                    promise.set_value(parsedRMessage);
                }
            }
        });
        // tell Lonk where to go
        std::thread whereGoThread([&]{
            tcp_client send(&host, senderPort);
            where_go whereGo;
            while(true) {
                received_data receivedMessage = future.get();

                std::string lonkCommand = whereGo.start(receivedMessage);

                // need some sort of parsing and putting in json-string.
                send.send_message(lonkCommand);
            }
        });

        // Show GUI with camera feed and all lonk values we are receiving.
        // Allow a manual overwrite of lonk controls that terminates thread(s) that write to lonk in the program.

    }   catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



