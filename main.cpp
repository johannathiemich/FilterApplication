#include <iostream>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include "Utils.h"
#include "ImageManipulation.h"

using namespace std;
using namespace IM;

int main() {
    //open the webcam plugged in the computer
    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    cv::Mat frame;
    Mat frame_gray;
    ImageManipulation im = ImageManipulation();

    int lowThreshold = 20;
    const int ratio = 3;
    const int kernel_size = 3;
    for(;;) {
        camera >> frame;
        if (frame.empty()) break;
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        std::vector<String> window_name_list = {"original", "gray", "Canny", "Histogram", "Template"};

        im.set_original_image(frame_gray);
        Mat* canny = im.CannyThreshold(kernel_size, lowThreshold, ratio, kernel_size);
        Mat* hist = im.Histogram();

        Point templ_match;
        std::vector<double> scales = linspace(1.0, 1.0, 1);
        //double max_scale = im.SimpleTemplateMatch(2, templ_match, scales);
        //cout << "max_scale:" << max_scale << endl;
        //cout << "result:" << templ_match << endl;

        //Mat templ_result = Mat(frame_gray);
        //rectangle(templ_result, templ_match, Point(templ_match.x + im.templ.cols * max_scale, templ_match.y + im.templ.rows * max_scale), Scalar::all(0), 2, 8, 0);

        im.AdvancedTemplateMatch();
        std::vector<cv::Mat> img_list = {frame, frame_gray, *canny, *hist};
        for (int i = 0; i < img_list.size(); i++) {
            cv::imshow(window_name_list[i], img_list[i]);
        }
        if (cv::waitKey(10) == 27) break;
    }

}
