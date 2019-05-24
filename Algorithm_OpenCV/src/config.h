//
// Created by mtzschoppe on 24/05/19.
//

#ifndef RAWTOCV_CONFIG_H
#define RAWTOCV_CONFIG_H

#include <opencv2/objdetect.hpp>

// Sensor -> n ADC's -> crop image in n rows and m columns
class ADC {
public:
    static ADC& getInstance();
    int nb_ADCs_x;
    int nb_ADCs_y;
};

class CascadeDetectorAdapter: public cv::DetectionBasedTracker::IDetector
{
public:
    CascadeDetectorAdapter(cv::Ptr<cv::CascadeClassifier> detector):
            IDetector(),
            Detector(detector)
    {
        CV_Assert(detector);
    }

    void detect(const cv::Mat &Image, std::vector<cv::Rect> &objects) CV_OVERRIDE
    {
        Detector->detectMultiScale(Image, objects, scaleFactor, minNeighbours, 0, minObjSize, maxObjSize);
    }

    virtual ~CascadeDetectorAdapter() CV_OVERRIDE
    {}

private:
    CascadeDetectorAdapter();
    cv::Ptr<cv::CascadeClassifier> Detector;
};

#endif //RAWTOCV_CONFIG_H
