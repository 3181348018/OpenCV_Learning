#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"  
#include "opencv2/imgproc.hpp"  
#include "iostream"
using namespace cv;
using namespace std;
int thresh = 100;
Mat img_src;
int max_thresh = 255;
RNG rng(12345);
Mat element = getStructuringElement(MORPH_RECT,Size(3,3));//获取自定义卷积核进行开运算。
void thresh_callback(int, void*)
{
    Mat srcTemp = img_src.clone();
    // imshow("img_src",srcTemp);
    threshold(srcTemp,srcTemp,thresh,max_thresh,cv::THRESH_OTSU);
    morphologyEx(srcTemp,srcTemp,MORPH_OPEN,element);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(srcTemp,contours,hierarchy,cv::RETR_TREE,cv::CHAIN_APPROX_SIMPLE,Point(0,0));
    imshow("after findcontour",srcTemp);
    //凸包及凸包检测缺陷参数定义
    vector<vector<Point>> pointHull(contours.size());
    vector<vector<int>> intHull(contours.size());
    vector<vector<Vec4i>> hullDefect(contours.size());
    for(size_t i = 0;i < contours.size();i++)
    {
        convexHull(Mat(contours[i]),pointHull[i],false);

    }
    //绘制凸包
    Mat drawing = Mat::zeros(srcTemp.size(),CV_8UC3);
    for(size_t i = 0; i < contours.size(); i++)
    {
        Scalar color = Scalar(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));
        drawContours(drawing,contours,i,color,1,8,vector<Vec4i>(),0,Point());
        drawContours(drawing,pointHull,i,color,1,8,vector<Vec4i>(),0,Point());
    }
    imshow("result",drawing);
    // waitKey(0);
    // return 0;
}

int main()
{
    img_src = imread("box4.png");
    if(!img_src.data)
    {
        cout << "failed to read" << endl;
        system("pause");
        return 0;
    }
    cvtColor(img_src,img_src,cv::COLOR_BGR2GRAY);
    blur(img_src,img_src,Size(3,3));
    string src_windows = "SRC";
    namedWindow(src_windows,cv::WINDOW_AUTOSIZE);
    imshow("src_windows",img_src);
    //鼠标回调
    createTrackbar("Threshhold",src_windows,&thresh,max_thresh,thresh_callback);
    thresh_callback(0,0);
    waitKey(0);
    return 0;
    //
}
