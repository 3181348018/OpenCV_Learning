#include<opencv2/opencv.hpp>
#include "iostream"
 
using namespace std; 
using namespace cv;  
 
int main()  
{
	Mat imageSource=imread("box4.png");
	Mat image_gray;
	Rect sel = Rect(210,100,710,747);
	imageSource = imageSource(sel);
	imshow("Source Image",imageSource);
	Mat image;
	cvtColor(imageSource,image_gray,COLOR_BGR2GRAY);
	GaussianBlur(image_gray,image,Size(3,3),0);
	Mat lap_dst, dst;
	threshold(image,image,0,255,cv::THRESH_OTSU); //Opencv Otsu算法
	//获取自定义核 第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
    Mat element = getStructuringElement(MORPH_RECT, Size(7, 7)); 
    morphologyEx(image,image,MORPH_OPEN,element);//对图像进行形态学处理，开运算
	//去除图像边缘锯齿
	//对图像进行中值滤波
	medianBlur(image,image,3);
	Mat element_dilate = getStructuringElement(MORPH_RECT, Size(5,5)); 
	dilate(image,image,element_dilate,Point(-1, -1), 1);
	imshow("边缘检测",image);
	vector<vector<Point>> contours;//vector容器里面放了一个vector容器，子容器里放点
	vector<Vec4i> hierarchy;//vector内存储四维int向量，hierarchy[i][0] ~hierarchy[i][3]，分别表示第i个轮廓的后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号
	findContours(image,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_NONE,Point());
	Mat imageContours=Mat::zeros(image.size(),CV_8UC1);
	Mat Contours=Mat::zeros(image.size(),CV_8UC3);  //绘制轮廓的图,注意CV_8UC1为单通道图像，cv_8UC3为双通道图像
	RNG rng(0);
	vector<Point> cnt;
	vector<vector<Vec4i>> defects(contours.size());
	vector<vector<Point>> hull(contours.size());
	Vec4i def;
	for(int i=0;i<contours.size();i++)
	{
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数
		// for(int j=0;j<contours[i].size();j++) 
		// {
		// 	//绘制出contours向量内所有的像素点
		// 	Point P=Point(contours[i][j].x,contours[i][j].y);
		// 	Contours.at<uchar>(P)=255;
		// }
 
		//输出hierarchy向量内容
		// char ch[256];
		// sprintf(ch,"%d",i);
		// string str=ch;
		// cout<<"向量hierarchy的第" <<str<<" 个元素内容为："<<endl<<hierarchy[i]<<endl<<endl;
		Scalar color = Scalar(rng.uniform(0,255), rng.uniform(0,255), rng.uniform(0,255));

		//绘制轮廓
		drawContours(Contours,contours,i,color,1,8,hierarchy,0, Point(0,0));
		//求取凸包数量
		cnt = contours[i];
		convexHull(cnt,hull[i]);
		drawContours(Contours,hull,i,Scalar(0,0,255),1);
		// if(hull.size() >= 1)
		// 	convexityDefects(cnt,hull[i],defects[i]);//寻找凸缺陷
		// for(int j = 0;j < defects[i].size();j++)
		// {
		// 	def = defects[i][j];
		// 	Point start = contours[i][def[0]];//起始点
		// 	Point end = contours[i][def[1]];
		// 	Point far = contours[i][def[2]];
		// 	int depth = def[3]/256;
		// 	Contours.at<uchar>(start) = 255;
		// 	Contours.at<uchar>(end) = 255;
		// 	// line(Contours,start,far,Scalar(0,0,255),2);
		// 	// line(Contours,end,far,Scalar(255,0,0),2);
		// }
	}

	imshow("Contours Image",Contours); //轮廓
	// imshow("Point of Contours",Contours);   //向量contours内保存的所有轮廓点集
	waitKey(0);
	return 0;
	//在凸包检测后由于边缘存在锯齿导致缺陷检测不理想。可能去除二值图像的边缘突出部效果会更好一些
	//尝试过中值滤波+膨胀操作效果并不好，容易将我们关心的缺陷也给抹去。
}

//去除二值图像的边缘突出部
//uthreshold、vthreshold分别表示突出部的宽度阈值和高度阈值
//type代表突出部的颜色，0表示黑色，1代表白色
void delete_jut(Mat& src, Mat& dst, int uthreshold, int vthreshold, int type)
{   
	int threshold;
	src.copyTo(dst);
	int height = dst.rows;
	int width = dst.cols;
	int k;  //用于循环计数传递到外部
	for (int i = 0; i < height - 1; i++)
	{
		uchar* p = dst.ptr<uchar>(i);//p为dst第i行第一个像素的指针
		for (int j = 0; j < width - 1; j++)
		{
			if (type == 0)
			{
				//行消除
				if (p[j] == 255 && p[j + 1] == 0)
				{
					if (j + uthreshold >= width)
					{
						for (int k = j + 1; k < width; k++)
							p[k] = 255;
					}
					else
					{
						for (k = j + 2; k <= j + uthreshold; k++)
						{
							if (p[k] == 255) break;
						}
						if (p[k] == 255)
						{
							for (int h = j + 1; h < k; h++)
								p[h] = 255;
						}
					}
				}
				//列消除
				if (p[j] == 255 && p[j + width] == 0)
				{
					if (i + vthreshold >= height)
					{
						for (k = j + width; k < j + (height - i)*width; k += width)
							p[k] = 255;
					}
					else
					{
						for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
						{
							if (p[k] == 255) break;
						}
						if (p[k] == 255)
						{
							for (int h = j + width; h < k; h += width)
								p[h] = 255;
						}
					}
				}
			}
			else  //type = 1
			{
				//行消除
				if (p[j] == 0 && p[j + 1] == 255)
				{
					if (j + uthreshold >= width)
					{
						for (int k = j + 1; k < width; k++)
							p[k] = 0;
					}
					else
					{
						for (k = j + 2; k <= j + uthreshold; k++)
						{
							if (p[k] == 0) break;
						}
						if (p[k] == 0)
						{
							for (int h = j + 1; h < k; h++)
								p[h] = 0;
						}
					}
				}
				//列消除
				if (p[j] == 0 && p[j + width] == 255)
				{
					if (i + vthreshold >= height)
					{
						for (k = j + width; k < j + (height - i)*width; k += width)
							p[k] = 0;
					}
					else
					{
						for (k = j + 2 * width; k <= j + vthreshold*width; k += width)
						{
							if (p[k] == 0) break;
						}
						if (p[k] == 0)
						{
							for (int h = j + width; h < k; h += width)
								p[h] = 0;
						}
					}
				}
			}
		}
	}
}

