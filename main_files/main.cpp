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


json j = "{\"speed\": 6000, \"heading\": 330}"_json;
std::string msg = j.dump();
//std::string msg = client_message;

//std::cout << msg << std::endl

int main(int argc, char **argv) {
    std::string host = "localhost";
    //std::string host = "10.25.47.143";
    std::string port = "9090";
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
        Get.listen();
        while(true){
            std::string mamma = json_struct.write_json(j);
            Get.send_message(mamma);
            //auto msg = Get.get_message();
            //json server_msg = json::parse(msg);
            //std::cout << "Got reply from server: " << (server_msg["left"] == 12) << std::endl;
            //std::cout << "Got reply from server: " << (server_msg) << std::endl;
            //std::cout << "Got reply from server: " << json_struct.read_json(msg) << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

