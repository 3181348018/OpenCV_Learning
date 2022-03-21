#include<opencv2/opencv.hpp>
using namespace cv;
int main()
{
    Mat imageSource=imread("box4.png");
	Mat image_gray,binary;
	Rect sel = Rect(210,100,710,747);
	imageSource = imageSource(sel);
	Mat image;
	cvtColor(imageSource,image_gray,COLOR_BGR2GRAY);
    threshold(image_gray,binary,0,255,THRESH_OTSU);
    imshow("binary",binary);
    waitKey(0);
	return 0;
}