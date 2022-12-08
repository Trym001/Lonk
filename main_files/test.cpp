#include <mutex>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
#include "tcp/tcp_client.hpp"
// #include "controller/system_timer.hpp"


struct thread_manager{
public:
    thread_manager() {
        completion = false;
    }

    void read_lonk_thread()
    {
        readLonk = std::make_unique<std::thread>([&]{
            tcp_client client(host, receiverPort);
            // json_parsing jsonParsing;
            // establish connection with Lonk (RVR/raspberry).
            try { client.listen(); }
            catch(const std::exception &e) { std::cerr << e.what() << std::endl; }

            decide_thread();

            std::string receivedMessage;
            try {
                while (!completion) {
                    // receive message from Lonk.
                    //std::this_thread::sleep_for(std::chrono::seconds(5));
                    receivedMessage = client.get_message();
                    // std::cout << i << ": read waiting...\n";
                    //cv.wait(lock, [&] { return i == 0; });
                    // std::cout << i << ": read working...\n";
                    // convert to usable data (parsedRMessage is a pointer)
                    std::unique_lock<std::mutex> lock(m);
                    json_parsing::read_json(receivedMessage, parsedRMessage, m);
                    lock.unlock();
                    //i = 1;
                    //cv.notify_all();
                    // std::cout << parsedRMessage->distSensor.front << "=================";
                }
            }   catch(const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void decide_thread(){
        decide = std::make_unique<std::thread>([&]{
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            where_go whereGo;
            int heading, yaw, front, left, right;
            try {
                while (!completion) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(200));
                    std::unique_lock<std::mutex> lock(m);
                    //std::cout << i << ": dec waiting for read...\n";
                    cv.wait(lock, [&] { return i == 0; });
                    //std::cout << i << ": dec working: read...\n";

                    heading = parsedRMessage->drivingdata.heading;
                    yaw = parsedRMessage->imu.yaw;
                    front = parsedRMessage->distSensor.front;
                    left = parsedRMessage->distSensor.left;
                    right = parsedRMessage->distSensor.right;
                    lock.unlock();

                    lonkCommand = whereGo.onwards(heading, yaw, front);
                    lonkCommand = whereGo.turn(left, right, lonkCommand);
                    i = 1;
                    cv.notify_all();
                }
            }   catch (const std::exception &e){
                std::cerr << e.what() << std::endl;
            }

        });
    }

    void send_lonk_thread(){
        sendLonk = std::make_unique<std::thread>([&]{
            tcp_client server(host, senderPort);
            server.listen();
            try {
                while (!completion) {
                    std::unique_lock<std::mutex> lock(m);
                    //std::cout << i << ": send waiting for dec...\n";
                    cv.wait(lock, [&] { return i == 1; });

                    json j_lonkCommand = lonkCommand;
                    lonkCommand = json_parsing::write_json(j_lonkCommand);
                    server.send_message(lonkCommand);

                    std::cout << lonkCommand << "\n";

                    lock.unlock();
                    std::this_thread::sleep_for(std::chrono::seconds (3));
                    i = 0;
                    cv.notify_all();
                }
                std::cout << "out of loop\n";
                return;
            }   catch(std::exception &e){
                std::cerr << e.what() << std::endl;
            }
        });
    }


    virtual ~thread_manager() {
        completion = true;
        if( readLonk->joinable() )  { readLonk->join(); }
        std::cout << "readlonk is dead\n";

        if( decide->joinable() )    { decide->join(); }

        if( sendLonk->joinable() )  { sendLonk->join(); }
        std::cout << "sendlonk is dead\n";
    }

private:
    std::string host = "10.25.47.143";
    std::string receiverPort = "9093";
    std::string senderPort = "9094";

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

        {
            thread_manager threadManager;

            threadManager.read_lonk_thread();

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            threadManager.send_lonk_thread();


            char input;
            std::cin >> input;
        }
        return 0;
    }   catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



