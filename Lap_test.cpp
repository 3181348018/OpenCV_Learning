#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;
int main()
{
    Mat img_src = imread("box3.png",0);
    imshow("img_src",img_src);
    Mat img_Lap;
    Laplacian(img_src,img_Lap,CV_16S,3,1,0);
    convertScaleAbs(img_Lap, img_Lap);
    imshow("img_Lap",img_Lap);
    waitKey();
    return 0;
}