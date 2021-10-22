#include "core/core.hpp"
#include "mainwindow.h"
#include <QApplication>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
using namespace cv;
using namespace std;
 
Mat globalMat;//ȫ�ֱ���
void RGBHistogram(Mat intMat);//rgb��ֱ��ͼͳ��
void GrayHistogram(Mat intMat);//��ʾ�Ҷ�ֱ��ͼ
void GrayHistogram2(Mat intMat);//��ʾ�Ҷ�ֱ��ͼ
void EqualizeHistogram(Mat intMat);//��ֵ��ͼ���Ҷ�ֱ��ͼ
void EqualizeHistogram2(Mat intMat);//��ֵ��ͼ���Ҷ�ֱ��ͼ
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//QT mainwindow�����Դ����ù�
    globalMat = imread("D:/source.png", 1 );//j����ͼƬ��0Ϊ�Ҷȣ�1Ϊ��ɫ
    RGBHistogram(globalMat);//rgb ֱ��ͼ
    GrayHistogram(globalMat);//��ʾ�Ҷ�ֱ��ͼ
    GrayHistogram2(globalMat);//��ʾ�Ҷ�ֱ��ͼ
    EqualizeHistogram(globalMat);//��ֵ��ͼ���Ҷ�ֱ��ͼ
    EqualizeHistogram2(globalMat);//��ֵ��ͼ���Ҷ�ֱ��ͼ
    MainWindow w;//QT mainwindow�����Դ����ù�
    w.show();//QT mainwindow�����Դ����ù�
    return a.exec();//QT mainwindow�����Դ����ù�
}
 
//RGBHistogram�ڵ�ֱ��ͼ
void RGBHistogram(Mat intMat)
{
    /// �ָ��3����ͨ��ͼ�� ( R, G �� B )
    vector<Mat> rgb_planes;
    split(intMat, rgb_planes );
    /// �趨bin��Ŀ
    int histSize = 255;
    /// �趨ȡֵ��Χ ( R,G,B) )
    float range[] = { 0, 255 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    Mat r_hist, g_hist, b_hist;
    /// ����ֱ��ͼ:
    calcHist( &rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    // ����ֱ��ͼ����
    int hist_w = 400; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
    /// ��ֱ��ͼ��һ������Χ [ 0, histImage.rows ]
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// ��ֱ��ͼ�����ϻ���ֱ��ͼ
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
    }
 
    /// ��ʾֱ��ͼ
    namedWindow("RGB Histogram", CV_WINDOW_AUTOSIZE );//��������
    imshow("RGB Histogram", histImage );//��������ʾֱ��ͼ
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//��������
    imshow("original image", globalMat );//��������ʾԭͼ
    waitKey(0);
}
 
//�ڵ�ֱ��ͼ
void GrayHistogram(Mat intMat)
{
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
//    �� cvQueryHistValue_*D ���� 1D, 2D, 3D �� N-D ֱ��ͼ��ָ��ֱ����
//    ��ֵ����ϡ��ֱ��ͼ�����������ֱ��ͼ�в����ڣ��������� 0, ���Ҳ���
//    ���µ�ֱ���顣
 
    IplImage *src= &IplImage(intMat);
    IplImage* gray_plane= cvCreateImage(cvGetSize(src),8,1);//����ԭͼ����һ���հ�ͼ
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);//ת�ɻҶ�ͼ
 
    int hist_size = 256;    //ֱ��ͼ�ߴ�
    int hist_height = 256;//ֱ��ͼ�߶�
    float range[] = {0,255};  //�Ҷȼ��ķ�Χ
    float* ranges[]={range};//����һ������
 
    //����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);     
    // ************************cvCreateHist()�������� *******************************
    //        ���� cvCreateHist ����һ��ָ���ߴ��ֱ��ͼ�����ҷ��ش�����ֱ��ͼ��
    //        ָ�롣 �������� ranges �� 0, ��ֱ����ķ�Χ�����ɺ���
    //        cvSetHistBinRanges ����cvCalcHist �� cvCalcBackProject�Ժ�ָ����
    //        ��Ȼ cvCalcHist �� cvCalcBackProject���Դ��� 8-����ͼ���������
    //        ���κ�ֱ����ķ�Χ�������Ƕ�������ȷ�0..255 ֮��Ŀռ䡣
    ///
    //            CvHistogram* cvCreateHist( int dims,           // ֱ��ͼά������Ŀ
    //                                      int* sizes,          //ֱ��ͼά���ߴ������
    //                                      int type,            // ֱ��ͼ�ı�ʾ��ʽ
    //                                      float** ranges=NULL, //ͼ�з��鷶Χ������
    //                                      int uniform=1 );     //��һ����ʶ
    //         type 
    //         ֱ��ͼ�ı�ʾ��ʽ: CV_HIST_ARRAY ��ζ��ֱ��ͼ���ݱ�ʾΪ��ά�ܼ����� CvMatND;
    //         CV_HIST_TREE ��ζ��ֱ��ͼ���ݱ�ʾΪ��άϡ������  CvSparseMat;
    //        ranges
    //         ͼ�з��鷶Χ������. ��������ȡ���ڲ��� uniform ��ֵ�������Χ���ô���ȷ����ʱ����ֱ��
    //         ͼ���������ӳ�䣨backprojected ����ÿ�������Ӧ������ͼ����ĸ�/����ֵ��
    //        uniform
    //        ��һ����ʶ�� �����Ϊ 0����ranges[i]��0<=i<cDims������ע�� cDimsΪ
    //        ֱ��ͼ��ά�������ڻҶ�ͼΪ 1����ɫͼΪ 3���ǰ�������Ԫ�صķ�Χ���飬
    //        ����ֱ��ͼ��iά���Ͻ���½硣�ڵ�iά�ϵ��������� [lower,upper]���ָ�
    //        �� dims[i] ����ȵĿ飨����ע�� dims[i]��ʾֱ��ͼ��iά�Ŀ���������Щ��
    //        ����ȷ���������صĵ� i ��ֵ������ע�����ڲ�ɫͼ�� iȷ��R, G,����B��
    //        �Ķ�Ӧ�Ŀ飻���Ϊ 0����ranges[i]�ǰ���dims[i]+1��Ԫ�صķ�Χ���飬��
    //        ��lower0, upper0, lower1, upper1 == lower2, ..., upperdims[i]-1, ����lowerj ��
    //        upperj�ֱ���ֱ��ͼ��iά�ϵ� j ����������½磨����������صĵ� i ��
    //        ֵ�����κ�����£�����ֵ���������һ��ֱ������ָ���ķ�Χ�⣬������
    //        �� cvCalcHist ���������һᱻ���� cvCalcBackProject ���㡣
 
 
    cvCalcHist(&gray_plane,gray_hist,0,0); //����Ҷ�ͼ���һάֱ��ͼ
    //    ����ͼ�� image(s) ��ֱ��ͼ
    //    ���� cvCalcHist ���㵥ͨ�����ͨ��ͼ���ֱ��ͼ�� ��������ֱ�����
    //    ����Ԫ�ؿɴ���Ӧ����ͼ���ͬ��λ����ȡ��
    //    void cvCalcHist( IplImage** image,         //����ͼ��
    //                     CvHistogram* hist,        //����ͼ��
    //                     int accumulate=0,         // �ۼƱ�ʶ
    //                     const CvArr* mask=NULL ); // ���� mask, ȷ������ͼ����ĸ����ر�����
    //    image
    //    ����ͼ�� s (��ȻҲ����ʹ�� CvMat** ).
    //    hist
    //    ����ͼ��
    //    accumulate
    //    �ۼƱ�ʶ��������ã���ֱ��ͼ�ڿ�ʼʱ�������㡣���������֤����Ϊ��
    //    ��ͼ�����һ��������ֱ��ͼ���������߸���ֱ��ͼ��
    //    mask
    //    ���� mask, ȷ������ͼ����ĸ����ر�����
 
 
    int scale = 2;
    //����һ��һάֱ��ͼ�ġ�ͼ����������Ϊ�Ҷȼ���������Ϊ���ظ�����*scale��
      IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);
    //    IplImage* cvCreateImage( CvSize size, int depth, int channels );
    //    size ͼ�����.
    //    depth
    //    ͼ��Ԫ�ص�λ��ȣ����������������֮һ:
    //    IPL_DEPTH_8U - �޷��� 8 λ����
    //    IPL_DEPTH_8S - �з��� 8 λ����
    //    IPL_DEPTH_16U - �޷��� 16 λ����
    //    IPL_DEPTH_16S - �з��� 16 λ����
    //    IPL_DEPTH_32S - �з��� 32 λ����
    //    IPL_DEPTH_32F - �����ȸ�����
    //    IPL_DEPTH_64F - ˫���ȸ�����
    //    channels
    //    ÿ��Ԫ�أ����أ�ͨ����.������ 1, 2, 3 �� 4.ͨ���ǽ����ȡ�ģ�����ͨ��
    //    �Ĳ�ɫͼ�����������ǣ�
    //    b0 g0 r0 b1 g1 r1 ...
    //    ��Ȼͨ�� IPL ͼ���ʽ���Դ����ǽ����ȡ��ͼ�񣬲���һЩ OpenCV Ҳ
    //    �ܴ�����, �����������ֻ�ܴ��������ȡͼ��.
 
      cvZero(hist_image);//�����ʹ�ñ���Ϊ��ɫ
    //ͳ��ֱ��ͼ�е����ֱ����
    float max_value = 0;
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);
 
    //�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��
    for(int i=0;i<hist_size;i++)
    {
        float bin_val = cvQueryHistValue_1D(gray_hist,i); //����i�ĸ���
        int intensity = cvRound(bin_val*hist_height/max_value);  //Ҫ���Ƶĸ߶�
        cvRectangle(hist_image,//ͼ��
                    cvPoint(i*scale,hist_height-1),//���ε�һ������
                    cvPoint((i+1)*scale - 1, hist_height - intensity),//���εĶԽ����϶���
                    Scalar(0,255,255) ); //������ɫ (RGB) �����ȣ��Ҷ�ͼ�� ��(grayscale image��CV_RGB(0,255,255)
    }
    //��������ͼʵ��
    cvRectangle(hist_image,            //ͼ��
                cvPoint(20,200),       //���ε�һ������
                cvPoint(200,300),      //���εĶԽ����϶���
                Scalar(0,255,255),     //������ɫ (RGB) �����ȣ��Ҷ�ͼ�� ��(grayscale image��
                1,                     //��ɾ��ε������Ĵ�ϸ�̶ȡ�ȡ��ֵʱ���� CV_FILLED���������������ɫ�ʵľ��Ρ�
                1,                     //���������͡���cvLine������
                0);                    //������С����λ����
 
    Mat outMat=cvarrToMat(hist_image);//IplImageתMat
    // ��ʾֱ��ͼ
    namedWindow("Gray Histogram", CV_WINDOW_AUTOSIZE );//��������
    imshow("Gray Histogram", outMat);//��������ʾֱ��ͼ
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//��������
    imshow("original image", globalMat );//��������ʾԭͼ
    waitKey(0);
}
 
//�׵�ֱ��ͼ���ҶȽ׵�
void GrayHistogram2(Mat intMat)
{
    //cvQueryHistValue_1D���������
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
 
    IplImage *src= &IplImage(intMat);//����ͼƬת����IplImage
    IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);//��ɫת�Ҷ�
 
    int hist_size = 256;    //ֱ��ͼ�ߴ�
    int hist_height = 256; //ֱ��ͼ�ߴ�
    float range[] = {0,255};  //�Ҷȼ��ķ�Χ
    float* ranges[]={range};//����һ������
 
    //����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    cvCalcHist(&gray_plane,gray_hist,0,0);//����Ҷ�ͼ���һάֱ��ͼ
 
    IplImage *pHistImage = cvCreateImage(cvSize(hist_size * 2, hist_height), IPL_DEPTH_8U, 1);
    cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height), CV_RGB(255, 255, 255), CV_FILLED);
    float fMaxHistValue = 0;//ͳ��ֱ��ͼ�е����ֱ����
    cvGetMinMaxHistValue(gray_hist, NULL, &fMaxHistValue, NULL, NULL);//ͳ��ֱ��ͼ�е����ֱ����
    //�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��
    for( int i = 0; i < 256; i++)
    {
        float fHistValue = cvQueryHistValue_1D(gray_hist, i); //����Ϊi��ֱ�����С
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) *256);  //Ҫ���Ƶĸ߶�
        cvRectangle(pHistImage,
                    cvPoint(i * 2, 256 - 1),
                    cvPoint((i + 1) * 2 - 1, 256 - nRealHeight),
                    cvScalar(i, 0, 0, 0),
                    CV_FILLED);
    }
    Mat outMat=cvarrToMat(pHistImage);//IplImageתMat
    // ��ʾֱ��ͼ
    namedWindow("Gray Histogram2", CV_WINDOW_AUTOSIZE );//��������
    imshow("Gray Histogram2", outMat);//��������ʾֱ��ͼ
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//��������
    imshow("original image", globalMat );//��������ʾԭͼ
    waitKey(0);
}
 
//�ڵ�ֱ��ͼ
void EqualizeHistogram(Mat intMat)
{
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
 
    IplImage *src= &IplImage(intMat);
    IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
    IplImage* EqualizeImg = cvCreateImage(cvGetSize(src),8,1);
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);
    cvEqualizeHist(gray_plane, EqualizeImg); // ���⻯
 
    int hist_size = 256;    //ֱ��ͼ�ߴ�
    int hist_height = 256;
    float range[] = {0,255};  //�Ҷȼ��ķ�Χ
    float* ranges[]={range};
    //����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    cvCalcHist(&EqualizeImg,gray_hist,0,0);//����Ҷ�ͼ���һάֱ��ͼ
    cvNormalizeHist(gray_hist,1.0);//��һ��ֱ��ͼ
 
    int scale = 2;
    //����һ��һάֱ��ͼ�ġ�ͼ��������Ϊ�Ҷȼ���������Ϊ���ظ�����*scale��
    IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);
    cvZero(hist_image);
    //ͳ��ֱ��ͼ�е����ֱ����
    float max_value = 0;
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);
    //�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��
    for(int i=0;i<hist_size;i++)
    {
        float bin_val = cvQueryHistValue_1D(gray_hist,i); //����i�ĸ���
        int intensity = cvRound(bin_val*hist_height/max_value);  //Ҫ���Ƶĸ߶�
        cvRectangle(hist_image,
                    cvPoint(i*scale,hist_height-1),
                    cvPoint((i+1)*scale - 1, hist_height - intensity),
                    CV_RGB(255,255,255));
    }
    Mat outMat=cvarrToMat(hist_image);//IplImageתMat
    Mat outMat2=cvarrToMat(EqualizeImg);//IplImageתMat
    // ��ʾֱ��ͼ
    namedWindow("Gray Histogram", CV_WINDOW_AUTOSIZE );//��������
    imshow("Gray Histogram", outMat);//��������ʾֱ��ͼ
    namedWindow("Equalize", CV_WINDOW_AUTOSIZE );//��������
    imshow("Equalize",outMat2);//��������ʾֱ��ͼ
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//��������
    imshow("original image", globalMat );//��������ʾԭͼ
    waitKey(0);
}
 
//�׵�ֱ��ͼ
void EqualizeHistogram2(Mat intMat)
{
    //cvQueryHistValue_1D���������
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
 
    IplImage *src= &IplImage(intMat);
    IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
    IplImage* EqualizeImg = cvCreateImage(cvGetSize(src),8,1);
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);//��ɫת�Ҷ�
    cvEqualizeHist(gray_plane, EqualizeImg); // ���⻯
 
    int hist_size = 256;    //ֱ��ͼ�ߴ�
    int hist_height = 256; //ֱ��ͼ�ߴ�
    float range[] = {0,255};  //�Ҷȼ��ķ�Χ
    float* ranges[]={range};
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);//����һάֱ��ͼ��ͳ��ͼ����[0 255]���صľ��ȷֲ�
    cvCalcHist(&EqualizeImg,gray_hist,0,0);//����Ҷ�ͼ���һάֱ��ͼ
    cvNormalizeHist(gray_hist,1.0);//��һ��ֱ��ͼ
    IplImage *pHistImage = cvCreateImage(cvSize(hist_size * 2, hist_height), IPL_DEPTH_8U, 1);
    cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height), CV_RGB(255, 255, 255), CV_FILLED);
    float fMaxHistValue = 0;//ͳ��ֱ��ͼ�е����ֱ����
    cvGetMinMaxHistValue(gray_hist, NULL, &fMaxHistValue, NULL, NULL);//ͳ��ֱ��ͼ�е����ֱ����
    //�ֱ�ÿ��ֱ�����ֵ���Ƶ�ͼ��
    for(int i = 0; i < 256; i++)
    {
        float fHistValue = cvQueryHistValue_1D(gray_hist, i); //����Ϊi��ֱ�����С
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) *256);  //Ҫ���Ƶĸ߶�
        cvRectangle(pHistImage,
                    cvPoint(i * 2, 256 - 1),
                    cvPoint((i + 1) * 2 - 1, 256 - nRealHeight),
                    cvScalar(i, 0, 0, 0),
                    CV_FILLED);
    }
    Mat outMat=cvarrToMat(pHistImage);//IplImageתMat
    Mat outMat2=cvarrToMat(EqualizeImg);//IplImageתMat
    // ��ʾֱ��ͼ
    namedWindow("Equalize Histogram", CV_WINDOW_AUTOSIZE );//��������
    imshow("Equalize Histogram", outMat);//��������ʾֱ��ͼ
    namedWindow("Equalize", CV_WINDOW_AUTOSIZE );//��������
    imshow("Equalize",outMat2);//��������ʾֱ��ͼ
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//��������
    imshow("original image", globalMat );//��������ʾԭͼ
    waitKey(0);
}