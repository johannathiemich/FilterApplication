//
// Created by johanna on 19.09.20.
//

#ifndef FILTERAPPLICATION_IMAGEMANIPULATION_H
#define FILTERAPPLICATION_IMAGEMANIPULATION_H

#include <iostream>
#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

namespace IM {
    class ImageManipulation {
    public:
        Mat original_image;
        Mat canny_image;
        Mat histogram_image;
        Mat templ;
        Mat templ_result;
        ImageManipulation();
        ImageManipulation(Mat* data);
        ~ImageManipulation();

        Mat* CannyThreshold(int blur_filter_size, int low_threshold, int ratio, int kernel_size);
        Mat* Histogram();
        double SimpleTemplateMatch(int match_method, Point &result, std::vector<double> &scales);
        void AdvancedTemplateMatch();
        void set_original_image(Mat &original_image);
    };
}


#endif //FILTERAPPLICATION_IMAGEMANIPULATION_H
