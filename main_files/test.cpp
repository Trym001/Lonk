#include <mutex>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include "data_parsing/json.hpp"
#include "controller/big_brain.hpp"
#include "tcp/tcp_client.hpp"
#include "other/camera.hpp"
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
                return;
            }   catch(std::exception &e){
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void tcp_read_cam_thread(){
        readCam = std::make_unique<std::thread>([&]{
            tcp_client getCamFeed(host, camPort);
            camera_library camera;

            try {
                getCamFeed.listen();
                while(!completion){
                    //auto test = Get.get_video();

                    // cv::Mat img = cv::imdecode(test, IMREAD_COLOR);

                    Mat img = camera.get_img_from_bits(getCamFeed.get_video());
                    //std::cout << img << std::endl;
                    cv::imshow("Lonk", img);
                    bool found = camera.find_blue(img, 2);

                    int key = waitKey(1);
                    if (key == 'q' || found) {
                        completion = true;
                    }

                }
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    };

    bool terminate_program(){
        return completion;
    }

    virtual ~thread_manager() {
        completion = true;
        if( readLonk->joinable() )  { readLonk->join(); }

        if( decide->joinable() )    { decide->join(); }

        if( sendLonk->joinable() )  { sendLonk->join(); }

        if( readCam->joinable() )  { readCam->join(); }
    }

private:
    std::string host            = "10.25.47.143";
    std::string receiverPort    = "9093";
    std::string senderPort      = "9094";
    std::string camPort         = "9091";

    // system_timer stopWatch;

    std::condition_variable cv;
    std::mutex m;
    bool completion;
    int i = 0;

    std::shared_ptr<received_data> parsedRMessage = std::make_shared<received_data>(received_data());
    std::string lonkCommand;

    std::unique_ptr<std::thread> readLonk;
    std::unique_ptr<std::thread> decide;
    std::unique_ptr<std::thread> sendLonk;
    std::unique_ptr<std::thread> readCam;

};


int main(int argc, char** argv) {
    try
    {

        {
            thread_manager threadManager;

            threadManager.read_lonk_thread();

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            threadManager.send_lonk_thread();


            while(!threadManager.terminate_program()){
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
        return 0;
    }   catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}



