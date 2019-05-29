//
// Created by mtzschoppe on 29/05/19.
//

#ifndef RAWTOCV_DETECTION_H
#define RAWTOCV_DETECTION_H

#include <opencv2/objdetect.hpp>

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

#endif //RAWTOCV_DETECTION_H
