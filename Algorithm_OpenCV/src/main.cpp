#include <fstream>
#include <iostream>
#include <stdio.h>

#include "libraw/libraw.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp> // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/mat.hpp>

using namespace std;
using namespace cv;

//number of images in folder ../RAWImages/...
const int number_images = 8;

const string path_key[2] = {"/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/RAWImages/example_",".dng"};

const string WindowName = "Face Detection example";

class CascadeDetectorAdapter: public DetectionBasedTracker::IDetector
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

int main(int arg_num, char *arg_vec[]) {

    LibRaw iProcessor;

    namedWindow(WindowName, cv::WINDOW_NORMAL);

    string file;
    string string_k;

    std::string cascadeFrontalfilename = samples::findFile("/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/opencv/opencv/data/lbpcascades/lbpcascade_frontalface.xml");

    cv::Ptr<cv::CascadeClassifier> cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> MainDetector = makePtr<CascadeDetectorAdapter>(cascade);

    if ( cascade->empty() )
    {
        printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
        return 2;
    }

    cascade = makePtr<cv::CascadeClassifier>(cascadeFrontalfilename);
    cv::Ptr<DetectionBasedTracker::IDetector> TrackingDetector = makePtr<CascadeDetectorAdapter>(cascade);

    if ( cascade->empty() )
    {
        printf("Error: Cannot load %s\n", cascadeFrontalfilename.c_str());
        return 2;
    }

    DetectionBasedTracker::Parameters params;
    DetectionBasedTracker Detector(MainDetector, TrackingDetector, params);

    if (!Detector.run())
    {
        printf("Error: Detector initialization failed\n");
        return 2;
    }

    for (int k=0; k<number_images; k++) {

        //convert int k to string
        stringstream ss;
        ss << k;
        string_k = ss.str();
        //get file path
        file = path_key[0] + string_k + path_key[1];

        //LibRaw
        if (iProcessor.open_file(file.c_str()) != LIBRAW_SUCCESS) {
            fprintf(stderr, "Cannot open %s: %s\n", file.c_str(), libraw_strerror(iProcessor.open_file(file.c_str())));
        }
        iProcessor.unpack();
        iProcessor.dcraw_process();

        int ret = 0;
        libraw_processed_image_t *image = iProcessor.dcraw_make_mem_image(&ret);

        auto img = cv::Mat(image->height, image->width, CV_16UC3); //CV_16UC3

        //create image from raw to colour
        for (int i = 0; i < image->height; i++) {
            for (int j = 0; j < image->width; j++) {
                int linindex = (i * image->width + j)*3;
                cv::Vec3s tripel = cv::Vec3s((short)image->data[linindex+2]*256, (short)image->data[linindex +1]*256, (short)image->data[linindex ]*256);
                img.at<cv::Vec3s>(i, j) = tripel;
            }
        }

        //convert from 16 bit to 8 bit
        img.convertTo(img, CV_8UC1, 1/256.0);

        //Mat ReferenceFrame;
        Mat GrayFrame;
        vector<Rect> Faces;

        do
        {
            //image >> ReferenceFrame;

            //detect face
            cvtColor(img, GrayFrame, COLOR_BGR2GRAY);
            Detector.process(GrayFrame);
            Detector.getObjects(Faces);

            //draw rectangle around the face
            for (size_t i = 0; i < Faces.size(); i++)
            {
                rectangle(img, Faces[i], Scalar(0,255,0));
            }

            imshow(WindowName, img);
        } while (waitKey(30) < 0);
    }

    Detector.stop();
    iProcessor.recycle();

    return 0;
}