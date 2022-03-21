#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\opencv.hpp>
#include<iostream>
 
using namespace std;
using namespace cv;

int main(){
    Mat img_src = imread("./box4.png");
	if (img_src.empty()) {
		printf("could not load image...\n");
		return -1;
	}
    namedWindow("input image", cv::WINDOW_AUTOSIZE);
	imshow("input image", img_src);
    Mat img_filter,img_gray,img_threah;
    pyrMeanShiftFiltering(img_src,img_filter,21,21);
    cvtColor(img_filter,img_gray,COLOR_BGR2GRAY);
    threshold(img_gray,img_threah,0,255,THRESH_OTSU);
    imshow("After Threshhold",img_threah);
    waitKey(0);
    return 0;
}