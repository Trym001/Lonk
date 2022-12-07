//
// Created by zaime on 12/1/2022.
//

#ifndef LONK_TCP_CLIENT_HPP
#define LONK_TCP_CLIENT_HPP

#include "tcp/network_helper.hpp"

#include <boost/asio.hpp>
#include <string>
#include <fstream>
#include <iostream>

//src - https://github.com/AIS2203-H-2022/networking_demo

using boost::asio::ip::tcp;

struct tcp_client {
public:
    tcp_client(std::string& host, std::string &port) : host_(host), port_(port), socket(io_service){}

    void listen(){
        tcp::resolver resolver(io_service);
        auto endpoints_ = resolver.resolve(host_, port_);
        boost::asio::connect(socket, endpoints_);
        std::cout << "connected to host: " << host_ << " connected to port: " << port_ << std::endl;
    }

    template <typename send>

    void send_message(send msg){
        int msgSize = static_cast<int>(msg.size());
        socket.send(boost::asio::buffer(int_to_bytes(msgSize), 4));
        socket.send(boost::asio::buffer(msg));
    }

    auto get_message(){
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
        return data;
    }

private:
    boost::asio::io_service io_service;
    boost::system::error_code error;
    std::string host_;
    std::string port_;
    tcp::socket socket;

};
#endif //LONK_TCP_CLIENT_HPP
