#include "core.hpp"
#include "opencv.hpp"
using namespace cv;
int main()
{
    Mat img_src = imread("box.jpg",0);
    threshold(img_src,img_src,0,255,cv::THRESH_OTSU); //图像的二值化处理
    Mat img_open;
    //获取自定义核 第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    Mat element = getStructuringElement(MORPH_RECT, Size(18, 18)); 
    morphologyEx(img_src,img_open,MORPH_OPEN,element);//对图像进行形态学处理，开运算
    imshow("img_open",img_open);
    waitKey();
    return 0;

}