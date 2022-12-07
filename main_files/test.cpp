#include <mutex>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
// #include "controller/system_timer.hpp"


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
            std::string receivedMessage;
            while(!completion){
                // receive message from Lonk.
                receivedMessage = get.get_message();
                // convert to usable data (parsedRMessage is a pointer)
                json_parsing::read_json(receivedMessage, parsedRMessage, m);
                i = 1;
                cv.notify_one();
            }
        });
    }

    void decide_thread(){
        decide = std::make_unique<std::thread>([&]{
            where_go whereGo;
            int heading, yaw, front, left, right;
            while(!completion){
                {
                    std::unique_lock<std::mutex> lock(m);
                    cv.wait(lock,[&]{return i == 1;});
                    i = 0;

                    heading = parsedRMessage->drivingdata.heading;
                    yaw = parsedRMessage->imu.yaw;
                    front = parsedRMessage->distSensor.front;
                    left = parsedRMessage->distSensor.left;
                    right = parsedRMessage->distSensor.right;
                }

                std::unique_lock<std::mutex> lk(m2);
                i = 1;
                lonkCommand = whereGo.onwards(heading, yaw, front);
                lonkCommand = whereGo.turn(left, right, lonkCommand);
                lk.unlock();
                cv.notify_all();
            }

        });
    }

    void send_lonk_thread(){
        sendLonk = std::make_unique<std::thread>([&]{
            tcp_client send(host, senderPort);
            while(!completion){
                std::unique_lock<std::mutex> lk(m2);
                cv.wait(lk, [&]{return i == 1;});
                send.send_message(lonkCommand);
                i = 0;
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

    // system_timer stopWatch;

    std::condition_variable cv;
    std::mutex m;
    std::mutex m2;
    bool completion;
    int i = 0;

    std::shared_ptr<received_data> parsedRMessage = std::make_shared<received_data>(received_data());
    std::string lonkCommand;

    std::unique_ptr<std::thread> readLonk;
    std::unique_ptr<std::thread> decide;
    std::unique_ptr<std::thread> sendLonk;

};


int main(int argc, char** argv) {
    try
    {
        thread_manager threadManager;
        threadManager.read_lonk_thread();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        threadManager.decide_thread();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        threadManager.send_lonk_thread();
        char input;
        std::cin >> input;
        return 0;

    }   catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



