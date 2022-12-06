#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>
#include <memory>
#include <chrono>

#include "tcp/tcp_client.hpp"
#include "controller/system_timer.hpp"
#include "data_parsing/json.hpp"

struct fun{
    void do_work(std::unique_ptr<std::thread>& t){
        t = std::make_unique<std::thread>([&]{
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "I had a good night sleep :)";
        });

    }
private:

};

int main(int argc, char **argv) {
    fun f;
    std::unique_ptr<std::thread> t;
    f.do_work(t);
    std::this_thread::sleep_for(std::chrono::seconds(2));
}
