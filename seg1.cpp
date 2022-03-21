
/*
读取图像，将原图上进行pyrMeanShiftFiltering()处理，保留更多的边缘信息，
在平滑区进行滤波，保证后面二值化时的效果更好，
转成单通道，二值处理，进行距离变换，将距离变换的结果归一化，找到山峰
再一次进行二值化处理，转到CV_8U类型的图像
进行轮廓发现，绘制轮廓，每次绘制轮廓时用不同的值对每个轮廓进行标记
关键：画一个圆作为标记这个不理解
对原图形态学腐蚀，去除干扰
使用watershed函数，得到maskers
根据masker中的像素值，索引颜色填充
*/

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	// Mat src = imread("./box4.png");
	VideoCapture cap;
	cap.open("./box.avi");
	if (!cap.isOpened())
	{
		cout << "图片未打开！";
		return 0;
	}
	for (;;)
	{
		Mat src;
		cap >> src;
		// Rect sel = Rect(210, 100, 710, 747);
		// src = src(sel);
		if (src.empty())
		{
			printf("could not load image...\n");
			return -1;
		}
		namedWindow("input image", cv::WINDOW_AUTOSIZE);
		// imshow("input image", src);
		// Mat kernel = (Mat_<int>(3,3)<<4,4,4,4,-32,4,4,4,4);
		Mat gray, binary, shifted;
		//锐化
		// filter2D(src,src,src.depth(),kernel);
		// 将灰度值相近的元素进行聚类，将颜色数据差距不大的像素点合成一个颜色，方便后续处理
		// 去边缘保留滤波，参数：输入图像，输出图像，空间窗的半径，色彩窗的半径
		pyrMeanShiftFiltering(src, shifted, 21, 21);

		// imshow("shifted", shifted);
		//滤波后的二值化
		cvtColor(shifted, gray, COLOR_BGR2GRAY);
		// Laplacian(gray,gray,CV_16S,1,1,0);
		// convertScaleAbs(gray,gray);
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
		// imshow("binary", binary);

		// distance transform
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);
		normalize(dist, dist, 0, 1, NORM_MINMAX); //归一化
		// imshow("distance result", dist);

		// binary
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);
		// threshold(dist, dist, 0,255,THRESH_BINARY | THRESH_OTSU);
		// imshow("distance binary", dist);

		// markers
		Mat dist_m;
		dist.convertTo(dist_m, CV_8UC1); //执行后，dist_m的像素值十分的小，扩大了1000倍，才看出来了轮廓
		// imshow("dist", dist);			 //差点误以为dist_m是一张黑图

		// finContours只支持CV_8UC1的格式，所以要进行通道转换
		vector<vector<Point>> contours;
		findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		// imshow("dist_m", dist_m);
		// create markers

		Mat markers = Mat::zeros(src.size(), CV_32SC1); // 如果使用 CV_8UC1 ，watershed 函数会报错
														//因为masker最后的边缘存储是-1，所以必须使用有符号的
		for (size_t t = 0; t < contours.size(); t++)
		{
			drawContours(markers, contours, static_cast<int>(t), Scalar::all(static_cast<int>(t) + 1), -1); //轮廓数字编号
		}
		circle(markers, Point(5, 5), 30, Scalar(255), -1);
		// 创建marker，标记的位置如果在要分割的图像块上会影响分割的结果，如果不创建，分水岭变换会无效
		// imshow("markers", markers * 10000);

		// 形态学操作 - 彩色图像，目的是去掉干扰，让结果更好
		Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
		morphologyEx(src, src, MORPH_ERODE, k); // 腐蚀，去粘连部位的干扰

		// 完成分水岭变换
		watershed(src, markers);
		Mat mark = Mat::zeros(markers.size(), CV_8UC1);
		markers.convertTo(mark, CV_8UC1);
		bitwise_not(mark, mark, Mat());
		// imshow("watershed result", mark);

		// generate random color
		vector<Vec3b> colors;
		for (size_t i = 0; i < contours.size(); i++)
		{
			int r = theRNG().uniform(0, 255);
			int g = theRNG().uniform(0, 255);
			int b = theRNG().uniform(0, 255);
			colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
		}

		// 颜色填充与最终显示
		Mat dst = Mat::zeros(markers.size(), CV_8UC3);
		int index = 0;
		for (int row = 0; row < markers.rows; row++)
		{
			for (int col = 0; col < markers.cols; col++)
			{
				index = markers.at<int>(row, col);
				if (index > 0 && index <= contours.size())
				{
					dst.at<Vec3b>(row, col) = colors[index - 1];
				}
				else
				{
					dst.at<Vec3b>(row, col) = Vec3b(0, 0, 0);
				}
			}
		}

		imshow("Final Result", dst);
		printf("number of objects : %d\n", contours.size());
		if (waitKey(10) >= 0)
			break;
		// waitKey(0);
	}
	return 0;
}