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

            std::string receivedMessage;
            try {
                while (!completion) {
                    // receive message from Lonk.
                    std::unique_lock<std::mutex> lock(m);
                    std::cout << "read waiting...\n";
                    cv.wait(lock, [&] { return i == 1; });
                    i = 0;
                    std::cout << "read working...\n";

                    receivedMessage = client.get_message();
                    // convert to usable data (parsedRMessage is a pointer)
                    json_parsing::read_json(receivedMessage, parsedRMessage, m);
                    lock.unlock();
                    i = 1;
                    cv.notify_one();
                    std::this_thread::sleep_for(std::chrono::milliseconds(20));
                    //std::cout << parsedRMessage->distSensor.front << std::endl;
                }
            }   catch(const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void decide_thread(){
        decide = std::make_unique<std::thread>([&]{
            where_go whereGo;
            int heading, yaw, front, left, right;
            try {
                while (!completion) {
                    {
                        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
                        std::unique_lock<std::mutex> lock(m);
                        std::cout << "dec waiting for read...\n";
                        lock.unlock();
                        cv.wait(lock, [&] { return i == 1; });
                        std::cout << "dec working: read...\n";
                        i = 0;

                        heading = parsedRMessage->drivingdata.heading;
                        yaw = parsedRMessage->imu.yaw;
                        front = parsedRMessage->distSensor.front;
                        left = parsedRMessage->distSensor.left;
                        right = parsedRMessage->distSensor.right;
                    }

                    std::unique_lock<std::mutex> lk(m2);
                    lonkCommand = whereGo.onwards(heading, yaw, front);
                    lonkCommand = whereGo.turn(left, right, lonkCommand);
                    lk.unlock();
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
                    //json kuk = "Left";
                    //std::string kuk2 = json_parsing::write_json(kuk);
                    //server.send_message(kuk2);
                    //std::cout << "left was sent \n";
                    std::unique_lock<std::mutex> lk(m2);
                    std::cout << "send waiting for dec...\n";
                    cv.wait(lk, [&] { return i == 1; });
                    std::cout << "send working...\n";

                    json j_lonkCommand = lonkCommand;
                    lonkCommand = json_parsing::write_json(j_lonkCommand);
                    server.send_message(lonkCommand);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
        readLonk->join();
        std::cout << "readlonk is dead\n";
        decide->join();
        sendLonk->join();
        std::cout
        << "sendlonk is dead\n";
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
    int i = 1;

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

            threadManager.decide_thread();
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



