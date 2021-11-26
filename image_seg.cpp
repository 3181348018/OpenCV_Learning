#include<opencv2/opencv.hpp>
#include "iostream"
using namespace cv;
using namespace std;
int main()
{
    Mat image_src = imread("./box4.png");
    cout << image_src.size() << endl;//1291*847
    Rect sel = Rect(210,100,710,747);
    Mat image_seg = image_src(sel);//根据所画矩形区域截取图片
    // rectangle(image_src,sel,(0,0,255),2);//在图像上画出矩形区域
    imshow("image_seg",image_seg);
    waitKey(0);
    return 0;
}