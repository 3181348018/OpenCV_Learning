#include "core.hpp"
#include "opencv.hpp"
using namespace cv;
int main()
{
    Mat img_src = imread("box.jpg",0);
    threshold(img_src,img_src,0,255,cv::THRESH_OTSU); //ͼ��Ķ�ֵ������
    Mat img_open;
    //��ȡ�Զ���� ��һ������MORPH_RECT��ʾ���εľ���ˣ���Ȼ������ѡ����Բ�εġ������͵�
    Mat element = getStructuringElement(MORPH_RECT, Size(18, 18)); 
    morphologyEx(img_src,img_open,MORPH_OPEN,element);//��ͼ�������̬ѧ����������
    imshow("img_open",img_open);
    waitKey();
    return 0;

}