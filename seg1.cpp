
/*
��ȡͼ�񣬽�ԭͼ�Ͻ���pyrMeanShiftFiltering()������������ı�Ե��Ϣ��
��ƽ���������˲�����֤�����ֵ��ʱ��Ч�����ã�
ת�ɵ�ͨ������ֵ�������о���任��������任�Ľ����һ�����ҵ�ɽ��
��һ�ν��ж�ֵ������ת��CV_8U���͵�ͼ��
�����������֣�����������ÿ�λ�������ʱ�ò�ͬ��ֵ��ÿ���������б��
�ؼ�����һ��Բ��Ϊ�����������
��ԭͼ��̬ѧ��ʴ��ȥ������
ʹ��watershed�������õ�maskers
����masker�е�����ֵ��������ɫ���
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
		cout << "ͼƬδ�򿪣�";
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
		//��
		// filter2D(src,src,src.depth(),kernel);
		// ���Ҷ�ֵ�����Ԫ�ؽ��о��࣬����ɫ���ݲ�಻������ص�ϳ�һ����ɫ�������������
		// ȥ��Ե�����˲�������������ͼ�����ͼ�񣬿ռ䴰�İ뾶��ɫ�ʴ��İ뾶
		pyrMeanShiftFiltering(src, shifted, 21, 21);

		// imshow("shifted", shifted);
		//�˲���Ķ�ֵ��
		cvtColor(shifted, gray, COLOR_BGR2GRAY);
		// Laplacian(gray,gray,CV_16S,1,1,0);
		// convertScaleAbs(gray,gray);
		threshold(gray, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
		// imshow("binary", binary);

		// distance transform
		Mat dist;
		distanceTransform(binary, dist, DistanceTypes::DIST_L2, 3, CV_32F);
		normalize(dist, dist, 0, 1, NORM_MINMAX); //��һ��
		// imshow("distance result", dist);

		// binary
		threshold(dist, dist, 0.4, 1, THRESH_BINARY);
		// threshold(dist, dist, 0,255,THRESH_BINARY | THRESH_OTSU);
		// imshow("distance binary", dist);

		// markers
		Mat dist_m;
		dist.convertTo(dist_m, CV_8UC1); //ִ�к�dist_m������ֵʮ�ֵ�С��������1000�����ſ�����������
		// imshow("dist", dist);			 //�������Ϊdist_m��һ�ź�ͼ

		// finContoursֻ֧��CV_8UC1�ĸ�ʽ������Ҫ����ͨ��ת��
		vector<vector<Point>> contours;
		findContours(dist_m, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
		// imshow("dist_m", dist_m);
		// create markers

		Mat markers = Mat::zeros(src.size(), CV_32SC1); // ���ʹ�� CV_8UC1 ��watershed �����ᱨ��
														//��Ϊmasker���ı�Ե�洢��-1�����Ա���ʹ���з��ŵ�
		for (size_t t = 0; t < contours.size(); t++)
		{
			drawContours(markers, contours, static_cast<int>(t), Scalar::all(static_cast<int>(t) + 1), -1); //�������ֱ��
		}
		circle(markers, Point(5, 5), 30, Scalar(255), -1);
		// ����marker����ǵ�λ�������Ҫ�ָ��ͼ����ϻ�Ӱ��ָ�Ľ�����������������ˮ��任����Ч
		// imshow("markers", markers * 10000);

		// ��̬ѧ���� - ��ɫͼ��Ŀ����ȥ�����ţ��ý������
		Mat k = getStructuringElement(MORPH_RECT, Size(3, 3), Point(-1, -1));
		morphologyEx(src, src, MORPH_ERODE, k); // ��ʴ��ȥճ����λ�ĸ���

		// ��ɷ�ˮ��任
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

		// ��ɫ�����������ʾ
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