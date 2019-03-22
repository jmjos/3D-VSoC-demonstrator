#include <fstream>
#include <iostream>
#include <stdio.h>

#include "libraw/libraw.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>

using namespace std;
using namespace cv;

/*
class CascadeDetector {
public:

private:
};
*/

int main(int arg_num, char *arg_vec[]) {

    LibRaw iProcessor;

    std::string file = "/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/pikes-peak.nef";
    if (iProcessor.open_file(file.c_str()) != LIBRAW_SUCCESS) {
        fprintf(stderr, "Cannot open %s: %s\n", file.c_str(), libraw_strerror(iProcessor.open_file(file.c_str())));
    }
    iProcessor.unpack();
    iProcessor.dcraw_process();

    int ret = 0;
    libraw_processed_image_t *image = iProcessor.dcraw_make_mem_image(&ret);

    auto img = cv::Mat(image->height, image->width, CV_16UC3);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            int linindex = (i * image->width + j)*3;
            cv::Vec3s tripel = cv::Vec3s((short)image->data[linindex+2]*256, (short)image->data[linindex +1]*256, (short)image->data[linindex ]*256);
            img.at<cv::Vec3s>(i, j) = tripel;
        }
    }

    /*function for facetracking
    cv::FeatureDetector::detect();
    //Tracker function
    createFaceDetectionMaskGenerator();
    */

    cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display window", img);
    cv::waitKey(0);

    iProcessor.recycle();

    return 0;
}