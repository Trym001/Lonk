#include <mutex>
#include <string>
#include <thread>
#include <iostream>
#include <condition_variable>
#include <shared_mutex>
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
#include "controller/system_timer.hpp"




struct thread_manager{
public:
    thread_manager() {
        completion = false;
    }

    void read_lonk_thread()
    {
        readLonk = std::make_unique<std::thread>([&]{
            tcp_client get(host, receiverPort);
            // json_parsing jsonParsing;
            // establish connection with Lonk (RVR/raspberry).
            get.listen();
            while(!completion){
                // receive message from Lonk.
                std::string receivedMessage = get.get_message();
                // convert to usable data (parsedRMessage is a pointer)
                json_parsing::read_json(receivedMessage, parsedRMessage, m);
            }
        });
    }

    void decide_thread(){
        decide = std::make_unique<std::thread>([&]{
            where_go whereGo;
            while(!completion){
                int heading, yaw, front, left, right;
                {
                    const std::unique_lock<std::mutex> lock(m);
                    heading = parsedRMessage->drivingdata.heading;
                    yaw = parsedRMessage->imu.yaw;
                    front = parsedRMessage->distSensor.front;
                    left = parsedRMessage->distSensor.left;
                    right = parsedRMessage->distSensor.right;
                }

                std::string lonkCommand = whereGo.onwards(heading, yaw, front);


                lonkCommand = whereGo.turn(left, right, lonkCommand);
            }

        });
    }

    void send_lonk_thread(){
        sendLonk = std::make_unique<std::thread>([&]{
            tcp_client send(host, senderPort);
            while(!completion){

            }
        });
    }


    virtual ~thread_manager() {
        completion = true;
        readLonk->join();
        decide->join();
        sendLonk->join();
    }

private:
    std::string host = "10.25.47.143";
    std::string receiverPort = "9090";
    std::string senderPort = "9091";

    system_timer stopWatch;

    std::condition_variable cv;
    std::mutex m;
    bool completion;

    std::shared_ptr<received_data> parsedRMessage = std::make_shared<received_data>(received_data());

    std::unique_ptr<std::thread> readLonk;
    std::unique_ptr<std::thread> decide;
    std::unique_ptr<std::thread> sendLonk;

};


int main(int argc, char** argv) {


    try
    {
        ????;
    }   catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



