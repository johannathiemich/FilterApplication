//
// Created by johanna on 19.09.20.
//

#include "ImageManipulation.h"

using namespace IM;
using namespace std;

ImageManipulation::ImageManipulation() {
}

ImageManipulation::ImageManipulation(const Mat* data) {
    data->copyTo(this->original_image);
    //cvtColor(this->original_color, this->original_image, COLOR_BGR2GRAY);
    canny_image.create(data->size(), data->type());
    histogram_image.create(data->size(), data->type());
    templ = imread("./images/drone_marker.png", IMREAD_GRAYSCALE);
}

ImageManipulation::~ImageManipulation() {
    this->original_image.release();
    this->canny_image.release();
    this->histogram_image.release();
}

void ImageManipulation::set_original_image(const Mat &original_image) {
    this->original_image = original_image;
    //cvtColor(original_image, this->original_image, COLOR_BGR2GRAY);
    canny_image.create(original_image.size(), original_image.type());
    histogram_image.create(original_image.size(), original_image.type());
    templ = imread("./images/drone_marker.png", IMREAD_GRAYSCALE);
}

Mat* ImageManipulation::CannyThreshold(const int blur_filter_size, const int low_threshold, const int ratio, const int kernel_size)
{
    cv::Mat detected_edges;
    detected_edges.create(this->original_image.size(), this->original_image.type());
    blur(this->original_image, detected_edges, Size(blur_filter_size, blur_filter_size));
    Canny(detected_edges, this->canny_image, low_threshold, low_threshold*ratio, kernel_size);
    return &this->canny_image;
};

Mat* ImageManipulation::Histogram() {
    equalizeHist(this->original_image, histogram_image);
    return &this->histogram_image;
}

double ImageManipulation::SimpleTemplateMatch(const int match_method, Point& result, std::vector<double> &scales) {
    Mat resized;
    double min_val, max_val, temp_score, max_score, max_scale;
    max_score = -1;
    Point min_loc, max_loc, temp_result;

    temp_result = Point(-1, -1);
    for (auto const& value: scales){
        Size size = Size(this->original_image.cols * value, this->original_image.rows * value);
        if ((size.height < this->templ.cols) || size.width < this->templ.rows) {
            break;
        }
        resize(this->canny_image, resized, size);
        matchTemplate(resized, this->templ, this->templ_result, match_method);
        normalize(this->templ_result, this->templ_result, 0, 1, NORM_MINMAX, -1, Mat());
        minMaxLoc(this->templ_result, &min_val, &max_val, &min_loc, &max_loc, Mat());
        if (match_method == TM_SQDIFF || match_method == TM_SQDIFF_NORMED) {
            temp_result = min_loc;
            temp_score = min_val;
        } else {
            temp_result = max_loc;
            temp_score = max_val;
        }
        if ((temp_result.x == -1 && temp_result.y == -1) || (temp_score > max_score)) {
            max_score = temp_score;
            result = temp_result;
            max_scale = value;
        }
    }
    return max_scale;
}

void ImageManipulation::AdvancedTemplateMatch() {
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    String status = "Target not found";

    findContours(this->canny_image, contours, hierarchy,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    for (auto const& value: contours) {
        double peri = arcLength(value, true);
        vector<Point> approx;
        approxPolyDP(value, approx, 0.01 * peri, true);

        if (approx.size() >= 4 && approx.size() <= 6) {
            Rect rect = boundingRect(approx);
            double aspect_ratio = double(rect.width) / rect.height;

            double area = contourArea(value);

            vector<Point> hull;
            convexHull(value, hull);
            double hullArea = contourArea(hull);
            double solidity = area / hullArea;

            bool keep_dims = rect.width > 25 && rect.height > 25;
            bool keep_solidity = solidity > 0.9;
            bool keep_aspect_ratio = aspect_ratio >= 0.8 && aspect_ratio <= 1.2;

            if (keep_dims && keep_solidity && keep_aspect_ratio) {
                status = "Target found";
                vector<vector<Point>> cnts = {value};
                drawContours(this->original_image, cnts, -1, (0, 0, 255), 4);
                Moments mom = moments(approx);
                int cX = int(mom.m10 / mom.m00);
                int cY = int(mom.m01 / mom.m00);

                int startX = int(cX - (rect.width * 0.15));
                int endX = int(cX + (rect.width * 0.15));

                int startY = int(cY - (rect.height * 0.15));
                int endY = int(cY + (rect.height * 0.15));

                line(this->original_image, Point(startX, cY), Point(endX, cY), (0, 0, 255), 3);
                line(this->original_image, Point(cX, startY), Point(cX, endY), (0, 0, 255), 3);

                putText(this->original_image, status, Point(20, 30), FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 255), 2);
            }
        }
    }
}



