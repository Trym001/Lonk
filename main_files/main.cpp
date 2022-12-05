#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "tcp/tcp_client.hpp"
#include "controller/system_timer.hpp"
#include "other/json.hpp"



using boost::asio::ip::tcp;
using json = nlohmann::json;


//json j = "{\"speed\": 20, \"heading\": 10}"_json;
json j = "{\"speed\": 20, \"heading\": 10}";

std::string msg = j.dump();
//std::string msg = client_message;

//std::cout << msg << std::endl

int main(int argc, char **argv) {
    //std::string host = "localhost";
    std::string host = "10.25.47.143";
    std::string port = "9098";
    //std::string port_2 = "9091";
    if (argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        port = argv[2];
    }
    tcp_client Get(host, port);
    system_timer timer;
    json_parsing json_struct;

    try {


        //tcp_client Send(host, port_2);
        Get.listen();
        while(true){
            auto img = Get.get_message();

            cv::imshow("mamma", img);

        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


