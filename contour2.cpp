#include<opencv2/opencv.hpp>
#include<core/core.hpp>
#include "imgproc/imgproc.hpp"  
#include "iostream"
 
using namespace std; 
using namespace cv;

int main()
{
    Mat src,dst;
    src = imread("box4.png");
    if(src.empty())
    {
        printf("can not load image \n");
        return -1;
    }
    namedWindow("input",1);
    imshow("input", src);
    dst = Mat::zeros(src.size(), CV_8UC3);

    blur(src,src,Size(3,3));//对输入的图像src进行均值滤波后用dst输出。
    cvtColor(src,src,COLOR_BGR2GRAY);
    Canny(src, src, 20, 80, 3, false);
    vector<vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    findContours(src, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    RNG rng(0);
    for(int i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
        drawContours(dst, contours, i, color, 2, 8, hierarchy, 0, Point(0,0));
    }
    namedWindow("output",1);
    imshow("output",dst);
    waitKey();
    return 0;
}
