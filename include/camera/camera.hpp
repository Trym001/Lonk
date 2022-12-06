//
// Created by zaime on 12/5/2022.
//

#ifndef LONK_CAMERA_HPP
#define LONK_CAMERA_HPP

#include <opencv2/opencv.hpp>

using namespace cv;
struct camera{
    template <typename cam_auto>

    bool open_camera(cam_auto cam){
        VideoCapture capture(cam);
        if (!capture.isOpened()) {
            std::cerr << "Unable to open camera.." << std::endl;
            return 1;
        }
        Mat image;
        bool stop{false};
    }
};



#endif //LONK_CAMERA_HPP
