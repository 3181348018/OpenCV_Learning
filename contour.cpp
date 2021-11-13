#include "core/core.hpp"  
#include "highgui.hpp"  
#include "imgproc/imgproc.hpp"  
#include "iostream"
 
using namespace std; 
using namespace cv;  
 
int main()  
{
	Mat imageSource=imread("box.jpg",0);
	imshow("Source Image",imageSource);
	Mat image;
	GaussianBlur(imageSource,image,Size(3,3),0);
	Mat lap_dst, dst;
	// Laplacian(image,lap_dst,CV_16S,3,1,0);
	// absdiff(image,lap_dst,image);
	// image = image - lap_dst;
	// Canny(image,image,50,250);//��ͼ����б�Ե���
	threshold(image,image,0,255,cv::THRESH_OTSU); //Opencv Otsu�㷨
	//��ȡ�Զ���� ��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
    Mat element = getStructuringElement(MORPH_RECT, Size(18, 18)); 
    morphologyEx(image,image,MORPH_OPEN,element);//��ͼ�������̬ѧ����������
	imshow("��Ե���",image);
	vector<vector<Point>> contours;//vector�����������һ��vector��������������ŵ�
	vector<Vec4i> hierarchy;//vector�ڴ洢��άint������hierarchy[i][0] ~hierarchy[i][3]���ֱ��ʾ��i�������ĺ�һ��������ǰһ������������������Ƕ�������������
	findContours(image,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE,Point());
	Mat imageContours=Mat::zeros(image.size(),CV_8UC1);
	Mat Contours=Mat::zeros(image.size(),CV_8UC1);  //����
	for(int i=0;i<contours.size();i++)
	{
		//contours[i]������ǵ�i��������contours[i].size()������ǵ�i�����������е����ص���
		for(int j=0;j<contours[i].size();j++) 
		{
			//���Ƴ�contours���������е����ص�
			Point P=Point(contours[i][j].x,contours[i][j].y);
			Contours.at<uchar>(P)=255;
		}
 
		//���hierarchy��������
		char ch[256];
		sprintf(ch,"%d",i);
		string str=ch;
		cout<<"����hierarchy�ĵ�" <<str<<" ��Ԫ������Ϊ��"<<endl<<hierarchy[i]<<endl<<endl;
 
		//��������
		drawContours(imageContours,contours,i,Scalar(255),1,8,hierarchy);
	}
	imshow("Contours Image",imageContours); //����
	imshow("Point of Contours",Contours);   //����contours�ڱ�������������㼯
	waitKey(0);
	return 0;
}
