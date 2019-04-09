#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <algorithm>

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


//functions for facial features detection
//static void detectFaces(Mat&, vector<Rect_<int> >&, string);
static void detectFacialFeaures(Mat& img, const vector<Rect_<int> > faces, string);
static void detectEyes(Mat&, vector<Rect_<int> >&, string);


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
    string input_image_path;
    string face_cascade_path, eye_cascade_path;

    cv::CommandLineParser parser(arg_num, arg_vec, "{eyes|/home/mtzschoppe/Downloads/opencv/data/haarcascades/haarcascade_eye.xml|}");

    //input_image_path = parser.get<string>("@image");
    //face_cascade_path = parser.get<string>("@facexml");
    eye_cascade_path = parser.has("eyes") ? parser.get<string>("eyes") : "";


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

            //detectFaces(img, Faces, face_cascade_path);
            detectFacialFeaures(img, Faces, eye_cascade_path);

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

/*
static void detectFaces(Mat& img, vector<Rect_<int> >& faces, string cascade_path)
{
    CascadeClassifier face_cascade;
    face_cascade.load(samples::findFile(cascade_path));

    if (!face_cascade.empty())
        face_cascade.detectMultiScale(img, faces, 1.15, 3, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    return;
}
 */

static void detectFacialFeaures(Mat& img, const vector<Rect_<int> > faces, string eye_cascade)
{
    for(unsigned int i = 0; i < faces.size(); ++i)
    {
        // Mark the bounding box enclosing the face
        Rect face = faces[i];
        //rectangle(img, Point(face.x, face.y), Point(face.x+face.width, face.y+face.height), Scalar(255, 0, 0), 1, 4);

        // Eyes, nose and mouth will be detected inside the face (region of interest)
        Mat ROI = img(Rect(face.x, face.y, face.width, face.height));

        // Check if all features (eyes, nose and mouth) are being detected
        bool is_full_detection = false;
        if( (!eye_cascade.empty()) /*&& (!nose_cascade.empty()) && (!mouth_cascade.empty())*/ )
            is_full_detection = true;

        // Detect eyes if classifier provided by the user

        if(!eye_cascade.empty())
        {
            vector<Rect_<int> > eyes;
            detectEyes(ROI, eyes, eye_cascade);

            // Mark points corresponding to the centre of the eyes
            for(unsigned int j = 0; j < eyes.size(); ++j)
            {
                Rect e = eyes[j];
                circle(ROI, Point(e.x+e.width/2, e.y+e.height/2), 10, Scalar(255, 0, 0), -1, 8);
                // rectangle(ROI, Point(e.x, e.y), Point(e.x+e.width, e.y+e.height), Scalar(0, 255, 0), 1, 4);
            }
        }
        /*
        // Detect nose if classifier provided by the user
        double nose_center_height = 0.0;
        if(!nose_cascade.empty())
        {
            vector<Rect_<int> > nose;
            detectNose(ROI, nose, nose_cascade);

            // Mark points corresponding to the centre (tip) of the nose
            for(unsigned int j = 0; j < nose.size(); ++j)
            {
                Rect n = nose[j];
                circle(ROI, Point(n.x+n.width/2, n.y+n.height/2), 3, Scalar(0, 255, 0), -1, 8);
                nose_center_height = (n.y + n.height/2);
            }
        }

        // Detect mouth if classifier provided by the user
        double mouth_center_height = 0.0;
        if(!mouth_cascade.empty())
        {
            vector<Rect_<int> > mouth;
            detectMouth(ROI, mouth, mouth_cascade);

            for(unsigned int j = 0; j < mouth.size(); ++j)
            {
                Rect m = mouth[j];
                mouth_center_height = (m.y + m.height/2);

                // The mouth should lie below the nose
                if( (is_full_detection) && (mouth_center_height > nose_center_height) )
                {
                    rectangle(ROI, Point(m.x, m.y), Point(m.x+m.width, m.y+m.height), Scalar(0, 255, 0), 1, 4);
                }
                else if( (is_full_detection) && (mouth_center_height <= nose_center_height) )
                    continue;
                else
                    rectangle(ROI, Point(m.x, m.y), Point(m.x+m.width, m.y+m.height), Scalar(0, 255, 0), 1, 4);
            }
        }
        */

    }

    return;
}

static void detectEyes(Mat& img, vector<Rect_<int> >& eyes, string cascade_path)
{
    CascadeClassifier eyes_cascade;
    eyes_cascade.load(samples::findFile(cascade_path, !cascade_path.empty()));

    if (!eyes_cascade.empty())
        eyes_cascade.detectMultiScale(img, eyes, 1.20, 5, 0|CASCADE_SCALE_IMAGE, Size(30, 30));
    return;
}


