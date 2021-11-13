#include "core/core.hpp"  
#include "highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include "iostream"
 
using namespace std; 
using namespace cv;  
 
int main()  
{
	Mat imageSource=imread("box4.png");
	Mat image_gray;
	imshow("Source Image",imageSource);
	Mat image;
	cvtColor(imageSource,image_gray,COLOR_BGR2GRAY);
	GaussianBlur(image_gray,image,Size(3,3),0);
	Mat lap_dst, dst;
	// Canny(image,image,50,70);//对图像进行边缘检测
	threshold(image,image,0,255,cv::THRESH_OTSU); //Opencv Otsu算法
	//获取自定义核 第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    Mat element = getStructuringElement(MORPH_RECT, Size(7, 7)); 
    morphologyEx(image,image,MORPH_OPEN,element);//对图像进行形态学处理，开运算
	imshow("边缘检测",image);
	vector<vector<Point>> contours;//vector容器里面放了一个vector容器，子容器里放点
	vector<Vec4i> hierarchy;//vector内存储四维int向量，hierarchy[i][0] ~hierarchy[i][3]，分别表示第i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号
	findContours(image,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE,Point());
	Mat imageContours=Mat::zeros(image.size(),CV_8UC1);
	Mat Contours=Mat::zeros(image.size(),CV_8UC1);  //绘制
	RNG rng(0);
	vector<Point> cnt;
	vector<Vec4i> defects;
	vector<Point> hull;
	for(int i=0;i<contours.size();i++)
	{
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
		for(int j=0;j<contours[i].size();j++) 
		{
			//绘制出contours向量内所有的像素点
			Point P=Point(contours[i][j].x,contours[i][j].y);
			Contours.at<uchar>(P)=255;
		}
 
		//输出hierarchy向量内容
		char ch[256];
		sprintf(ch,"%d",i);
		string str=ch;
		cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<endl<<hierarchy[i]<<endl<<endl;
		Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));
		//绘制轮廓
		drawContours(imageSource,contours,i,color,3,8,hierarchy,0, Point(0,0));
		//求取凸包数量
		cnt = contours[i];
		convexHull(cnt,hull,false,false);
		if(hull.size() >= 1)
			convexityDefects(cnt,hull,defects);
		for(int j = 0;j < defects.size();j++)
		{
			
		}
	}

	imshow("Contours Image",imageSource); //轮廓
	// imshow("Point of Contours",Contours);   //向量contours内保存的所有轮廓点集
	waitKey(0);
	return 0;
}
