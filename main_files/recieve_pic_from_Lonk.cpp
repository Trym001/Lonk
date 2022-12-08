//
// Created by zaime on 12/7/2022.
//

#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "tcp/tcp_client.hpp"
#include "other/camera.hpp"





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
    camera_library camera;

    try {


        //tcp_client Send(host, port_2);
        Get.listen();
        bool stop{false};
        while(!stop){
            //auto test = Get.get_video();

            // cv::Mat img = cv::imdecode(test, IMREAD_COLOR);

            Mat img = camera.get_img_from_bits(Get.get_video());
            //std::cout << img << std::endl;
            cv::imshow("Lonk", img);
            bool found = camera.find_blue(img, 2);

            int key = waitKey(1);
            if (key == 'q') {
                stop = true;
            }

        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}


