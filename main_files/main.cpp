#include <boost/asio.hpp>
#include <array>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
#include <fstream>

#include "tcp/tcp_client.hpp"
#include "controller/system_timer.hpp"
#include "other/json.hpp"
#include "../../Lonk/include/camera/detect_face.hpp"

#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char **argv) {
    //std::string host = "localhost";
    std::string host = "10.25.47.143";
    std::string port = "9094";
    //std::string port_2 = "9091";
    if (argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        port = argv[2];
    }
    tcp_client Get(host, port);

    try {
        Get.listen();
        bool stop{false};
        while(!stop){

            auto msg = Get.get_message();
        //std::cout << msg << std::endl;

            cv::Mat img = cv::imdecode(msg, 1);

            imshow("test", img);


            int key = waitKey(1);
                    if (key == 'q') {
                        stop = true;
                    }



//            try {
//
//                VideoCapture capture(0);
//                if (!capture.isOpened()) {
//                    std::cerr << "Unable to open camera.." << std::endl;
//                    return 1;
//                }
//
//                const std::string windowTitle{"Display image"};
//                namedWindow(windowTitle, WINDOW_AUTOSIZE);
//
//                Mat image;
//                bool stop{false};
//                while (!stop) {
//                    capture >> image;
//
//                    imshow("Display Image", image);
//                    int key = waitKey(1);
//                    if (key == 'q') {
//                        stop = true;
//                    }
//                }
//
//            } catch (const std::exception &ex) {
//                std::cerr << ex.what() << std::endl;
//                return 1;
//            }
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

