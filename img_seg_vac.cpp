#include<opencv2/opencv.hpp>
#include "iostream"
using namespace cv;
using namespace std;

int main()
{
    Mat img_src = imread("./box4.png");
    Rect sel = Rect(210,100,710,747);
    img_src = img_src(sel);
    Mat img_gray,img_thresh,img_open;
    cvtColor(img_src,img_gray,COLOR_BGR2GRAY);
    threshold(img_gray,img_thresh,0,255,THRESH_OTSU);//OTSU∂˛÷µ‘ÀÀ„
    imshow("img_thresh",img_thresh);
    Mat element = getStructuringElement(MORPH_RECT,Size(7,7));
    morphologyEx(img_thresh,img_open,MORPH_OPEN,element);
    imshow("img_open",img_open);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(img_open,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_NONE,Point());
    Mat contour = Mat::zeros(img_open.size(),CV_8UC3);
    for(int i=0;i<contours.size();i++)
    {
        drawContours(contour,contours,i,Scalar(0,0,255),2);
    }
    imshow("contour",contour);
    waitKey(0);
    return 0;
}
