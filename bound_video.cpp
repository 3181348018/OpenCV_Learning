#include "opencv2/opencv.hpp"
#include "opencv2/imgproc.hpp"  
#include "opencv2/highgui.hpp"
#include "opencv2/core.hpp"  
#include "iostream"
using namespace cv;
using namespace std;
RNG rng(12345);
int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    // cap.open("./EP03.mp4");
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat edges;
    namedWindow("edges",1);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        cvtColor(frame, edges, cv::COLOR_RGB2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        // threshold(edges,edges,0,255,cv::THRESH_OTSU);
        // imshow("edges", edges);
        findContours(edges,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,Point(0,0));
        Mat drawing = Mat::zeros(edges.size(),CV_8UC3);
        for(size_t i = 0; i < contours.size(); i++)
        {
            Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
            drawContours(drawing,contours,i,color,1,8,vector<Vec4i>(),0,Point());
        }
        imshow("result",drawing);
        if(waitKey(30) >= 0) 
            break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}