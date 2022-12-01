
#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string>

#include "network_helper.hpp"

#include <nlohmann/json.hpp>
#include <fstream>

using boost::asio::ip::tcp;
using json = nlohmann::json;


json j = "{\"speed\": 6000, \"heading\": 330}"_json;
std::string client_message = j.dump();
std::string msg = client_message;

//std::cout << msg << std::endl



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

        tcp::resolver resolver(io_service);
        auto endpoints = resolver.resolve(host, port);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoints);


        //sende melding
        /*
        int msgSize = static_cast<int>(msg.size());

        socket.send(boost::asio::buffer(int_to_bytes(msgSize), 4));
        socket.send(boost::asio::buffer(msg));
        */

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