#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp> 
#include <iostream>

using namespace cv;
using namespace std;

//***************Otsu�㷨ͨ������䷽���ֵ������Ӧ��ֵ******************
int OtsuAlgThreshold(const Mat image);

int main(int argc,char *argv[])  
{  
	Mat image=imread("Lena.jpg");
	imshow("SoureImage",image);
	cvtColor(image,image,cv::COLOR_RGB2GRAY);	
	Mat imageOutput;
	Mat imageOtsu;	
	int thresholdValue=OtsuAlgThreshold(image);
	cout<<"��䷽��Ϊ�� "<<thresholdValue<<endl;
	threshold(image,imageOutput,thresholdValue,255,cv::THRESH_BINARY);
	threshold(image,imageOtsu,0,255,cv::THRESH_OTSU); //Opencv Otsu�㷨
	imshow("SoureImage",image);
	imshow("Output Image",imageOutput);
	imshow("Opencv Otsu",imageOtsu);	
	waitKey();
	return 0;  
}  
int OtsuAlgThreshold(const Mat image)
{
	if(image.channels()!=1)
	{
		cout<<"Please input Gray-image!"<<endl;
		return 0;
	}
	int T=0; //Otsu�㷨��ֵ
	double varValue=0; //��䷽���м�ֵ����
	double w0=0; //ǰ�����ص�����ռ����
	double w1=0; //�������ص�����ռ����
	double u0=0; //ǰ��ƽ���Ҷ�
	double u1=0; //����ƽ���Ҷ�
	double Histogram[256]={0}; //�Ҷ�ֱ��ͼ���±��ǻҶ�ֵ�����������ǻҶ�ֵ��Ӧ�����ص�����
	uchar *data=image.data;
	double totalNum=image.rows*image.cols; //��������
	//����Ҷ�ֱ��ͼ�ֲ���Histogram�����±��ǻҶ�ֵ�����������ǻҶ�ֵ��Ӧ���ص���
	for(int i=0;i<image.rows;i++)   //Ϊ������������û�а�rows��cols���������
	{
		for(int j=0;j<image.cols;j++)
		{
			Histogram[data[i*image.step+j]]++;
		}
	}
	for(int i=0;i<255;i++)
	{
		//ÿ�α���֮ǰ��ʼ��������
		w1=0;		u1=0;		w0=0;		u0=0;
		//***********����������ֵ����**************************
		for(int j=0;j<=i;j++) //�������ָ�ֵ����
		{
			w1+=Histogram[j];  //�����������ص�����
			u1+=j*Histogram[j]; //�������������ܻҶȺ�
		}
		// if(w1==0) //�����������ص���Ϊ0ʱ�˳�
		// {
		// 	break;
		// }
		u1=u1/w1; //��������ƽ���Ҷ�
		w1=w1/totalNum; // �����������ص�����ռ����
		//***********����������ֵ����**************************

		//***********ǰ��������ֵ����**************************
		for(int k=i+1;k<255;k++)
		{
			w0+=Histogram[k];  //ǰ���������ص�����
			u0+=k*Histogram[k]; //ǰ�����������ܻҶȺ�
		}
		if(w0==0) //ǰ���������ص���Ϊ0ʱ�˳�
		{
			break;
		}
		u0=u0/w0; //ǰ������ƽ���Ҷ�
		w0=w0/totalNum; // ǰ���������ص�����ռ����
		//***********ǰ��������ֵ����**************************

		//***********��䷽�����******************************
		double varValueI=w0*w1*(u1-u0)*(u1-u0); //��ǰ��䷽�����
		if(varValue<varValueI)
		{
			varValue=varValueI;
			T=i;
		}
	}
	return T;
}