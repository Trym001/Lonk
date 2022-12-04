#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "tcp/tcp_client.hpp"
#include "controller/system_timer.hpp"
#include "other/json.hpp"


//json j = "{\"speed\": 6000, \"heading\": 330}"_json;
//std::string msg = j.dump();
//std::string msg = client_message;

//std::cout << msg << std::endl

int main(int argc, char **argv) {
    //faan er argc og argv??
    //std::cout << "argc: " << argc << " argv: " << argv << std::endl;

    //std::string host = "localhost";
    std::string host = "10.25.47.143";
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


        //tcp_client Send(host, port_2);
        Get.listen();
        while(true){
            //Get.send_message(msg);

            auto msg = Get.get_message();
            //json server_msg = json::parse(msg);
            //std::cout << "Got reply from server: " << (server_msg["left"] == 12) << std::endl;
            //std::cout << "Got reply from server: " << (server_msg) << std::endl;
            auto parsed_msg = json_struct.read_json(msg);
            std::cout << "Got reply from server: " << (parsed_msg) << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

/*

int main(int argc, char **argv) {
    //std::string host = "localhost";
    std::string host = "10.25.47.143";
    std::string port = "9090";
    if (argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        port = argv[2];
    }

    try {
        //connecte me server
        boost::asio::io_service io_service;

        main_files::resolver resolver(io_service);
        auto endpoints = resolver.resolve(host, port);
        //std::cout << (endpoints) << std::endl;

        main_files::socket socket(io_service);
        boost::asio::connect(socket, endpoints);


        //sende melding
        /*
        int msgSize = static_cast<int>(msg.size());

        socket.send(boost::asio::buffer(int_to_bytes(msgSize), 4));
        socket.send(boost::asio::buffer(msg));


        //lese melding
        boost::system::error_code error;
        while (true)
        {
            std::array<unsigned char, 4> sizeBuf{};
            boost::asio::read(socket, boost::asio::buffer(sizeBuf), boost::asio::transfer_exactly(4), error);
            if (error) {
                throw boost::system::system_error(error);
            }
            boost::asio::streambuf buf;
            size_t len = boost::asio::read(socket, buf, boost::asio::transfer_exactly(bytes_to_int(sizeBuf)), error);
            if (error) {
                throw boost::system::system_error(error);
            }
            std::string data(boost::asio::buffer_cast<const char *>(buf.data()), len);
            //fått melding og parse jason filen
            json server_msg = json::parse(data);
            //std::cout << "Got reply from server: " << (server_msg["left"] == 12) << std::endl;
            std::cout << "Got reply from server: " << (server_msg) << std::endl;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
*/

