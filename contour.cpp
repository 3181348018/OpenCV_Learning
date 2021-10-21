#include "core/core.hpp"  
#include "highgui/highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include "iostream"
 
using namespace std; 
using namespace cv;  
 
int main()  
{
	Mat imageSource=imread("box106.jpg",0);
	imshow("Source Image",imageSource);
	Mat image;
	GaussianBlur(imageSource,image,Size(3,3),0);
	Mat lap_dst, dst;
	Laplacian(image,lap_dst,CV_16S,3,1,0);
	// absdiff(image,lap_dst,image);
	// image = image - lap_dst;
	Canny(image,image,50,250);//对图像进行边缘检测
	imshow("边缘检测",image);
	
	vector<vector<Point>> contours;//vector容器里面放了一个vector容器，子容器里放点
	vector<Vec4i> hierarchy;//vector内存储四维int向量，hierarchy[i][0] ~hierarchy[i][3]，分别表示第i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号
	findContours(image,contours,hierarchy,RETR_TREE,CHAIN_APPROX_SIMPLE,Point());
	Mat imageContours=Mat::zeros(image.size(),CV_8UC1);
	Mat Contours=Mat::zeros(image.size(),CV_8UC1);  //绘制
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
 
		//绘制轮廓
		drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);
	}
	imshow("Contours Image",imageContours); //轮廓
	imshow("Point of Contours",Contours);   //向量contours内保存的所有轮廓点集
	waitKey(0);
	return 0;
}
