//
// Created by zaime on 12/6/2022.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#ifndef LONK_CAMERA_HPP
#define LONK_CAMERA_HPP


struct camera_library{
public:
    // source for finding color: https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html
    bool find_blue(Mat imgOriginal, int sens_){

        Mat imgHSV;

        cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

        Mat imgThresholded;

        inRange(imgHSV, Scalar(iLowH_, iLowS_, iLowV_), Scalar(iHighH_, iHighS_, iHighV_), imgThresholded); //Threshold the image

        //morphological opening (removes small objects from the foreground)
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //morphological closing (removes small holes from the foreground)
        dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

        //Calculate the moments of the thresholded image
        Moments oMoments = moments(imgThresholded);

        double dArea = oMoments.m00;

        if (dArea > (sens_*100000))
        {
            found_blue_ = true;
        }
        else{
            found_blue_ = false;
        }
        return found_blue_;
    }
    template <typename feed>
    Mat get_img_from_bits(feed bit_feed){
        cv::Mat img = cv::imdecode(bit_feed, IMREAD_COLOR);
        return img;
    }

private:
    bool found_blue_ = false;
    int iLowH_ = 50;
    int iHighH_ = 110;

    int iLowS_ = 150;
    int iHighS_ = 255;

    int iLowV_ = 60;
    int iHighV_ = 255;
};



#endif //LONK_CAMERA_HPP
