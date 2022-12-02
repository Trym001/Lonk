//
// Created by zaime on 12/1/2022.
//

#ifndef LONK_TCP_CLIENT_HPP
#define LONK_TCP_CLIENT_HPP

#include "network_helper.hpp"


#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string>



#include <fstream>

using boost::asio::ip::tcp;



class tcp_client {
public:
    tcp_client(std::string &host, std::string &port) : host_(host), port_(port), socket(io_service){}



    void listen(){
        //boost::asio::io_service io_service;
        tcp::resolver resolver(io_service);
        auto endpoints_ = resolver.resolve(host_, port_);
        boost::asio::connect(socket, endpoints_);
    }

    template <typename send>

    void send_message(send msg){

        int msgSize = static_cast<int>(msg.size());

        socket.send(boost::asio::buffer(int_to_bytes(msgSize), 4));
        socket.send(boost::asio::buffer(msg));

    }

    auto get_message(){
        //boost::system::error_code error;
        //std::array<unsigned char, 4> sizeBuf{};
        boost::asio::read(socket, boost::asio::buffer(sizeBuf), boost::asio::transfer_exactly(4), error);
        if (error) {
            throw boost::system::system_error(error);
        }
        //boost::asio::streambuf buf;
        size_t len = boost::asio::read(socket, buf, boost::asio::transfer_exactly(bytes_to_int(sizeBuf)), error);
        if (error) {
            throw boost::system::system_error(error);
        }
        std::string data(boost::asio::buffer_cast<const char *>(buf.data()), len);

        return data;
        //fått melding og parse jason filen
        //json server_msg = json::parse(data);
        //std::cout << "Got reply from server: " << (server_msg["left"] == 12) << std::endl;
        //std::cout << "Got reply from server: " << (server_msg) << std::endl;
    }


private:
    //from listen
    boost::asio::io_service io_service;

    std::string intervar_;


    //from get msg
    boost::system::error_code error;

    std::array<unsigned char, 4> sizeBuf{};

    boost::asio::streambuf buf;

    //std::string host;
    //std::string port;
    //int socket;

    std::string host_;

    std::string port_;

    tcp::socket socket;

};


#endif //LONK_TCP_CLIENT_HPP
