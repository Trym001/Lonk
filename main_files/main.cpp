#include <iostream>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <thread>
#include <chrono>
#include <numeric>
#include <vector>


struct plonk {
public:
    void f1(){
        t1 = std::make_unique<std::thread>([&]{
            std::vector<int> x(3);
            for(int n : x) {
                std::unique_lock<std::mutex> lock(m);
                std::cout << "t1 waiting... \n";
                cv.wait(lock, [&] { return i == 0; });
                std::cout << "t1 working... \n";
                lock.unlock();
                i = 1;
                cv.notify_all();
            }
        });
    }

    void f2(){
        t2 = std::make_unique<std::thread>([&] {
            std::vector<int> x(3);
            for(int n : x) {
                std::unique_lock<std::mutex> lock(m);
                std::cout << "t2 waiting...\n";
                cv.wait(lock, [&] { return i == 1; });
                std::cout << "t2 working... \n";
                lock.unlock();
                i = 2;
                cv.notify_all();
            }
        });
    }

    void f3(){
        t3 = std::make_unique<std::thread>([&] {
            std::vector<int> x(3);
            for(int n : x){
                std::unique_lock<std::mutex> lk(m);
                std::cout << "t3 waiting... \n";
                cv.wait(lk, [&] { return i == 2; });
                std::cout << "t3 working... \n";
                lk.unlock();
                i = 0;
                cv.notify_all();
            }

        });
    }

    virtual ~plonk() {
        t1->join();
        t2->join();
        t3->join();

    }

private:
    std::unique_ptr<std::thread> t1;
    std::unique_ptr<std::thread> t2;
    std::unique_ptr<std::thread> t3;
    int i = 0;
    std::mutex m;
    std::condition_variable cv;
};

int main()
{
    {
        plonk t;
        t.f1();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        t.f2();
        // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        t.f3();

        /*char input;
        std::cin >> input;
         */
    }
}