#include <iostream>
#include <nlohmann/json.hpp>
#include <string>



#include "other/wasd_controller.hpp"
#include "tcp/tcp_client.hpp"
#include "other/json.hpp"







int main(int argc, char **argv) {
    //std::string host = "localhost";
    std::string host = "10.25.47.143";
    std::string port = "9091";
    //std::string port_2 = "9091";
    if (argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        port = argv[2];
    }
    tcp_client Get(host, port);
    controller ctrl;
    json_parsing pikk;

    int speed = 0;
    int heading = 0;

    while(true){
        try {
            Get.listen();

            char input;
            std::cin>>input;

            ctrl.calculate(input);

            std::cout << "speed:"<< ctrl.speed() << " heading:" << ctrl.heading() << std::endl;

            //json j = "{\"speed\": 5, \"heading\": 330}"_json;
            json j = {{"speed", ctrl.speed()}, {"heading", ctrl.heading()}};
            //std::cout << "speed:"<< j << " heading:" << j["speed"] << std::endl;
            std::string msg = pikk.write_json(j);
            //std::string test = "test";
            Get.send_message(msg);


            }
         catch (const std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
}}

//int main(){
//    int speed = 0;
//    int heading = 0;
//    controller ctrl;
//    while(true){
//        char input;
//        std::cin>>input;
//
//        ctrl.calculate(input);
//
//        std::cout << "speed:"<< ctrl.speed() << " heading:" << ctrl.heading() << std::endl;
//    }
//
//    return 0;
//}