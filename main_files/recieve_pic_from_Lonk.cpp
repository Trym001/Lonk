//
// Created by zaime on 12/7/2022.
//

#include <iostream>
#include <string>

#include "other/wasd_controller.hpp"
#include "tcp/tcp_client.hpp"
#include "other/camera.hpp"
#include "other/json.hpp"




int main(int argc, char **argv) {
    //std::string host = "localhost";
    std::string host = "10.25.47.143";
    std::string port_camera = "9091";
    std::string port_send = "9094";
    //std::string port_2 = "9091";
    if (argc == 3) {
        // assuming <hostname> <port>
        host = argv[1];
        port_camera = argv[2];
    }
    tcp_client Get_Camera(host, port_camera);
    tcp_client Send_msg(host, port_send);

    camera_library camera;
    controller ctrl;
    json_parsing j_parser;


    int speed = 0;
    int heading = 0;

    while (true){
    try {
        //tcp_client Send(host, port_2);
        Get_Camera.listen();
        bool stop{false};
        while(!stop){
            Mat img = camera.get_img_from_bits(Get_Camera.get_video());
            cv::imshow("Lonk", img);
            int key = waitKey(1);
            if (key == 'q') {
                stop = true;
            }
            char input;

            if(camera.find_blue(img, 2)){

                std::cout << "found object" << std::endl;

            }



            std::cin>>input;

            ctrl.calculate(input);

            std::cout << "speed:"<< ctrl.speed() << " heading:" << ctrl.heading() << std::endl;

            json j = {{"speed", ctrl.speed()}, {"heading", ctrl.heading()}};
            std::string msg = j_parser.write_json(j);
            Send_msg.send_message(msg);


        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }}
}


