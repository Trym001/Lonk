#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

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
    json_parsing pars;

    try {


        //tcp_client Send(host, port_2);
        Get.listen();
        while(true){
            auto msg = Get.get_message();
            std::cout << pars.read_json(msg) << std::endl;



        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


