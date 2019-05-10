#include <fstream>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <ctype.h>

#include "libraw/libraw.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/core/core.hpp> // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <opencv2/features2d.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

//number of images in folder ../RAWImages/...
const int number_images = 98; //8 oder 28 oder 506
const int first_image = 10; //0 oder 48 oder 449 oder 10
//const int second_image = 11; //1 oder 49 oder 450
const int detection_loop = 50;
const int MAX_COUNT = 100;

//const string path_key[2] = {"/home/mtzschoppe/Documents/git/3D-VSoC-demonstrator/Algorithm_OpenCV/RAWImages/example_",".dng"};
const string path_key[2] = {"/home/mtzschoppe/Desktop/face images/3/filename000",".png"}; //zweistellige Zahlen
//const string path_key[] = {"/home/mtzschoppe/Desktop/face images/2/filename00",".png"}; //dreistellige Zahlen

//const string WindowName = "Face Detection example";

//functions
static void help()
{
    // print a welcome message, and the OpenCV version
    cout << "\nThis is a demo of Lukas-Kanade optical flow lkdemo(),\n"
            "Using OpenCV version " << CV_VERSION << endl;
    cout << "\nIt uses camera by default, but you can provide a path to video as an argument.\n";
    cout << "\nHot keys: \n"
            "\tESC - quit the program\n"
            "\tr - auto-initialize tracking\n"
            "\tc - delete all the points\n"
            "\tn - switch the \"night\" mode on/off\n"
            "To add/remove a feature point click it\n" << endl;
}
Point2f point;
bool addRemovePt = false;
static void onMouse( int event, int x, int y, int /*flags*/, void* /*param*/ )
{
    if( event == EVENT_LBUTTONDOWN )
    {
        point = Point2f((float)x, (float)y);
        addRemovePt = true;
    }
}

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

    string file;
    string string_k;
    int detect_img = 1;

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

    //VideoCapture cap("/home/mtzschoppe/Downloads/sample-videos/head-pose-face-detection-male.mp4");
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Size subPixWinSize(10,10), winSize(31,31);
    bool nightMode = false;
    help();
    vector<Rect> Faces;
    namedWindow("LK Demo", cv::WINDOW_NORMAL);
    setMouseCallback( "LK Demo", onMouse, 0 );
    Mat gray, prevGray, frame;
    //vector<Point2f> points[2];
    int* Faces_height;
    int* Faces_width;
    int* Faces_x;
    int* Faces_y;
    Mat img, imga;
    /*
    if( !cap.isOpened() )
    {
        cerr << "Could not initialize capturing...\n";
        return 0;
    }
    */

    for (int v=0; v < detect_img; v++) {
        //TODO read image
        //path to image
        //convert int k to string
        stringstream ss;
        ss << first_image;
        string_k = ss.str();
        //get file path
        file = path_key[0] + string_k + path_key[1];

        /*
        //TODO LibRaw
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
        //TODO LibRaw end
        */



        img = imread(file,1);
        //TODO read image end

        //convert from 16 bit to 8 bit
        //img.convertTo(img, CV_8UC1, 1/256.0);

        for (int g=0; g<detection_loop; g++){
            cvtColor(img, gray, COLOR_BGR2GRAY);
            Detector.process(gray);
            Detector.getObjects(Faces);

            for (size_t i = 0; i < Faces.size(); i++)
            {
                rectangle(img, Faces[i], Scalar(255,255,0));
            }

        }
        if (Faces.size() == 0) {
            detect_img++;
        }
        if (detect_img > 3) {
            cout << "break" << endl;
            break;
        }
    }

    if (Faces.size() == 0) {
        cerr << "Cannot detect face" << endl;
    } else {
        Faces_width = new int [Faces.size()] ();
        Faces_height = new int [Faces.size()] ();
        Faces_x = new int [Faces.size()] ();
        Faces_y = new int [Faces.size()] ();

        cout << "faces detected: " << Faces.size()  << endl;

        for (int p=0; p<Faces.size(); p++) {
            Faces_height[p] = Faces[p].height;
            Faces_width[p] = Faces[p].width;
            Faces_x[p] = Faces[p].x;
            Faces_y[p] = Faces[p].y;
            cout << "face " << p+1 << " data:\nheight: " << Faces_height[p] << "\twidth: " << Faces_width[p] << "\n x: " << Faces_x[p] << "\ty: " << Faces_y[p] << endl;
        }

        cout << "image detected: " << detect_img << endl;
        cout << "image size: " << img.size() << endl;
        imshow("LK Demo", img);
        waitKey(0);
    }

    int m = detect_img + first_image;
    char c;

    vector<Point2f> points[Faces.size()*2];

    for(;;)
    {
        //TODO read image:
        //path to image
        //convert int k to string
        stringstream ss;
        ss << m;
        string_k = ss.str();
        //get file path
        file = path_key[0] + string_k + path_key[1];

        /*
        //TODO LibRaw
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
        //TODO LibRaw end
        */

        img = imread(file,1);

        //convert from 16 bit to 8 bit
        //img.convertTo(img, CV_8UC1, 1/256.0); //can't use for png
        //TODO read image end

        //TODO tracking:
        cout << file << endl;
        cvtColor(img, gray, COLOR_BGR2GRAY);

        /*
        if( nightMode ) {
            img = Scalar::all(0);
        }*/
        //Mat a(Faces_x, Faces_y);



        for(int q=0; q<(Faces.size()); q++) {
            cout << "q = " << q << endl;
            Mat mask = Mat::zeros(img.size(), CV_8UC1);
            Mat roi(mask, cv::Rect(Faces_x[q], Faces_y[q], Faces_width[q], Faces_height[q]));
            roi = Scalar(255, 255, 255);
            cout << "ROI: " << roi.size() << endl;
            cout << "Mask: " << mask.size() << endl;
            cout << "Faces: " << Faces_x[q] << endl;
            // automatic initialization
            goodFeaturesToTrack(gray, points[q * 2 + 1], MAX_COUNT, 0.01, 10, mask, 3, 3, 0, 0.04);
            cornerSubPix(gray, points[q * 2 + 1], subPixWinSize, Size(-1, -1), termcrit);
            addRemovePt = false;

            //cout << points[q*2+1] << endl;
            cout << "points0: " << points[0] << endl;
            cout << "points1: " << points[1] << endl;
            //cout << "points2: " << points[2] << endl;
            //cout << "points3: " << points[3] << endl;

            cout << "q = " << q << endl;


            if (!points[q * 2].empty()) {
                vector<uchar> status;
                vector<float> err;
                if (prevGray.empty())
                    gray.copyTo(prevGray);
                calcOpticalFlowPyrLK(prevGray, gray, points[q * 2], points[q * 2 + 1], status, err, winSize, 3,
                                     termcrit, 0, 0.001);
                size_t i, k;
                for (i = k = 0; i < points[q * 2 + 1].size(); i++) {
                    if (addRemovePt) {
                        if (norm(point - points[q * 2 + 1][i]) <= 5) {
                            addRemovePt = false;
                            continue;
                        }
                    }
                    if (!status[i])
                        continue;
                    points[q * 2 + 1][k++] = points[q * 2 + 1][i];
                    //Point2f a(Faces_x, Faces_y);
                    circle(img, points[q * 2+1][i], 3, Scalar(0, 255, 0), -1, 8);
                    //if (m > (detect_img + first_image +4) && q == 0) {
                    //    circle(img, points[q * 2+3][i], 3, Scalar(0, 255, 0), -1, 8);
                    //}
                }
                points[q * 2 + 1].resize(k);

            }
            if (addRemovePt && points[q * 2 + 1].size() < (size_t) MAX_COUNT) {
                vector<Point2f> tmp;
                tmp.push_back(point);
                cornerSubPix(gray, tmp, winSize, Size(-1, -1), termcrit);
                points[q * 2 + 1].push_back(tmp[0]);
                addRemovePt = false;
            }


            imshow("LK Demo", img);
            c = (char) waitKey(15);

        }
            if (c == 27) {
                break;
            }
            switch (c) {
                case 'c':
                    points[0].clear();
                    points[1].clear();
                    break;
                case 'n':
                    nightMode = !nightMode;
                    break;
            }
            for (int i=0; i<Faces.size(); i++) {
                std::swap(points[i*2 +1], points[i*2]);
            }
            cv::swap(prevGray, gray);

        if (m > number_images) {
            break;
        }
        m++;
    }
    //TODO tracking end

    Detector.stop();
    iProcessor.recycle();

    return 0;
}