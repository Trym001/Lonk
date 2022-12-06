#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>
#include <thread>
#include <mutex>
#include <memory>
#include <chrono>

#include "tcp/tcp_client.hpp"
#include "controller/system_timer.hpp"
#include "data_parsing/json.hpp"

struct thread_manager{
    thread_manager() {
        completion = false;
    }

    void do_work(){
        t = std::make_unique<std::thread>([&]{
            while(!completion) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                std::unique_lock<std::mutex> lock(m);
                std::cout << "I had a good night sleep :) \n";
            }
            return;
        });

    }

    virtual ~thread_manager() {
        completion = true;
        {
            std::unique_lock<std::mutex> lock(m);
            std::cout << "I found a wayyy";
        }
        t->join();
    }

private:
    std::unique_ptr<std::thread> t;
    std::mutex m;
    bool completion;
};

int main(int argc, char **argv) {
    thread_manager t;
    t.do_work();
    std::this_thread::sleep_for(std::chrono::seconds(4));
    return 0;
}
