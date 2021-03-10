//
// Created by johanna on 24.09.20.
//

#ifndef FILTERAPPLICATION_UTILS_H
#define FILTERAPPLICATION_UTILS_H

#include <opencv2/video/tracking.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;

cv::Mat makeCanvas(std::vector<cv::Mat>, int windowHeight, int nRows);

std::vector<double> linspace(double start_in, double end_in, int num_in);


#endif //FILTERAPPLICATION_UTILS_H
