#include <iostream>  
#include <fstream>
#include <opencv2/opencv.hpp>

#define Pic_Path "/box106.jpg" 
#define Pic_Name "model1.jpg"

using namespace std;
int main (void)
{
    string pic = string(Pic_Path) + string(Pic_Name);
    cout << pic << endl;
	
    cv::Mat gray_src; 
    gray_src = cv::imread(pic.c_str(),0);
    cv::namedWindow("原始图片",cv::WINDOW_AUTOSIZE);
    cv::imshow("原始图片",gray_src);
    
    

    cv::Mat dst;
    cv::GaussianBlur(gray_src,dst,cv::Size(3,3),0,0);
    
    

    // cv::Mat gray_src;
    // cv::cvtColor(dst,gray_src,COLOR_YUV2BGR_NV12);
    // cv::namedWindow("gray pic",cv::WINDOW_AUTOSIZE);
    // cv::imshow("gray pic",gray_src);
    

    cv::Mat lap_dst;
    cv::Laplacian(gray_src,lap_dst,CV_16S,3,1,0);

    cv::convertScaleAbs(lap_dst,lap_dst);

    cv::threshold(lap_dst,lap_dst,0,255,cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::namedWindow("laplance pic",cv::WINDOW_AUTOSIZE);
    cv::imshow("laplance pic",lap_dst);
    
    
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
