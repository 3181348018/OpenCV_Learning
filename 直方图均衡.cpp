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
 
Mat globalMat;//全局变量
void RGBHistogram(Mat intMat);//rgb的直方图统计
void GrayHistogram(Mat intMat);//显示灰度直方图
void GrayHistogram2(Mat intMat);//显示灰度直方图
void EqualizeHistogram(Mat intMat);//均值化图及灰度直方图
void EqualizeHistogram2(Mat intMat);//均值化图及灰度直方图
 
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);//QT mainwindow程序自带不用管
    globalMat = imread("D:/source.png", 1 );//j加载图片，0为灰度，1为彩色
    RGBHistogram(globalMat);//rgb 直方图
    GrayHistogram(globalMat);//显示灰度直方图
    GrayHistogram2(globalMat);//显示灰度直方图
    EqualizeHistogram(globalMat);//均值化图及灰度直方图
    EqualizeHistogram2(globalMat);//均值化图及灰度直方图
    MainWindow w;//QT mainwindow程序自带不用管
    w.show();//QT mainwindow程序自带不用管
    return a.exec();//QT mainwindow程序自带不用管
}
 
//RGBHistogram黑底直方图
void RGBHistogram(Mat intMat)
{
    /// 分割成3个单通道图像 ( R, G 和 B )
    vector<Mat> rgb_planes;
    split(intMat, rgb_planes );
    /// 设定bin数目
    int histSize = 255;
    /// 设定取值范围 ( R,G,B) )
    float range[] = { 0, 255 } ;
    const float* histRange = { range };
    bool uniform = true;
    bool accumulate = false;
    Mat r_hist, g_hist, b_hist;
    /// 计算直方图:
    calcHist( &rgb_planes[0], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &rgb_planes[2], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    // 创建直方图画布
    int hist_w = 400; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );
    Mat histImage( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );
    /// 将直方图归一化到范围 [ 0, histImage.rows ]
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    /// 在直方图画布上画出直方图
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
 
    /// 显示直方图
    namedWindow("RGB Histogram", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("RGB Histogram", histImage );//窗口中显示直方图
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("original image", globalMat );//窗口中显示原图
    waitKey(0);
}
 
//黑底直方图
void GrayHistogram(Mat intMat)
{
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
//    宏 cvQueryHistValue_*D 返回 1D, 2D, 3D 或 N-D 直方图的指定直方块
//    的值。对稀疏直方图，如果方块在直方图中不存在，函数返回 0, 而且不创
//    建新的直方块。
 
    IplImage *src= &IplImage(intMat);
    IplImage* gray_plane= cvCreateImage(cvGetSize(src),8,1);//根据原图创建一个空白图
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);//转成灰度图
 
    int hist_size = 256;    //直方图尺寸
    int hist_height = 256;//直方图高度
    float range[] = {0,255};  //灰度级的范围
    float* ranges[]={range};//创建一个数组
 
    //创建一维直方图，统计图像在[0 255]像素的均匀分布
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);     
    // ************************cvCreateHist()函数解释 *******************************
    //        函数 cvCreateHist 创建一个指定尺寸的直方图，并且返回创建的直方图的
    //        指针。 如果数组的 ranges 是 0, 则直方块的范围必须由函数
    //        cvSetHistBinRanges 或者cvCalcHist 和 cvCalcBackProject稍后指定。
    //        虽然 cvCalcHist 和 cvCalcBackProject可以处理 8-比特图像而无需设
    //        置任何直方块的范围，但它们都被假设等分0..255 之间的空间。
    ///
    //            CvHistogram* cvCreateHist( int dims,           // 直方图维数的数目
    //                                      int* sizes,          //直方图维数尺寸的数组
    //                                      int type,            // 直方图的表示格式
    //                                      float** ranges=NULL, //图中方块范围的数组
    //                                      int uniform=1 );     //归一化标识
    //         type 
    //         直方图的表示格式: CV_HIST_ARRAY 意味着直方图数据表示为多维密集数组 CvMatND;
    //         CV_HIST_TREE 意味着直方图数据表示为多维稀疏数组  CvSparseMat;
    //        ranges
    //         图中方块范围的数组. 它的内容取决于参数 uniform 的值。这个范围的用处是确定何时计算直方
    //         图或决定反向映射（backprojected ），每个方块对应于输入图像的哪个/哪组值。
    //        uniform
    //        归一化标识。 如果不为 0，则ranges[i]（0<=i<cDims，译者注： cDims为
    //        直方图的维数，对于灰度图为 1，彩色图为 3）是包含两个元素的范围数组，
    //        包括直方图第i维的上界和下界。在第i维上的整个区域 [lower,upper]被分割
    //        成 dims[i] 个相等的块（译者注： dims[i]表示直方图第i维的块数），这些块
    //        用来确定输入象素的第 i 个值（译者注：对于彩色图像， i确定R, G,或者B）
    //        的对应的块；如果为 0，则ranges[i]是包含dims[i]+1个元素的范围数组，包
    //        括lower0, upper0, lower1, upper1 == lower2, ..., upperdims[i]-1, 其中lowerj 和
    //        upperj分别是直方图第i维上第 j 个方块的上下界（针对输入象素的第 i 个
    //        值）。任何情况下，输入值如果超出了一个直方块所指定的范围外，都不会
    //        被 cvCalcHist 计数，而且会被函数 cvCalcBackProject 置零。
 
 
    cvCalcHist(&gray_plane,gray_hist,0,0); //计算灰度图像的一维直方图
    //    计算图像 image(s) 的直方图
    //    函数 cvCalcHist 计算单通道或多通道图像的直方图。 用来增加直方块的
    //    数组元素可从相应输入图像的同样位置提取。
    //    void cvCalcHist( IplImage** image,         //输入图像
    //                     CvHistogram* hist,        //输入图像
    //                     int accumulate=0,         // 累计标识
    //                     const CvArr* mask=NULL ); // 操作 mask, 确定输入图像的哪个象素被计数
    //    image
    //    输入图像 s (虽然也可以使用 CvMat** ).
    //    hist
    //    输入图像
    //    accumulate
    //    累计标识。如果设置，则直方图在开始时不被清零。这个特征保证可以为多
    //    个图像计算一个单独的直方图，或者在线更新直方图。
    //    mask
    //    操作 mask, 确定输入图像的哪个象素被计数
 
 
    int scale = 2;
    //创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）
      IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);
    //    IplImage* cvCreateImage( CvSize size, int depth, int channels );
    //    size 图像宽、高.
    //    depth
    //    图像元素的位深度，可以是下面的其中之一:
    //    IPL_DEPTH_8U - 无符号 8 位整型
    //    IPL_DEPTH_8S - 有符号 8 位整型
    //    IPL_DEPTH_16U - 无符号 16 位整型
    //    IPL_DEPTH_16S - 有符号 16 位整型
    //    IPL_DEPTH_32S - 有符号 32 位整型
    //    IPL_DEPTH_32F - 单精度浮点数
    //    IPL_DEPTH_64F - 双精度浮点数
    //    channels
    //    每个元素（像素）通道号.可以是 1, 2, 3 或 4.通道是交叉存取的，例如通常
    //    的彩色图像数据排列是：
    //    b0 g0 r0 b1 g1 r1 ...
    //    虽然通常 IPL 图象格式可以存贮非交叉存取的图像，并且一些 OpenCV 也
    //    能处理他, 但是这个函数只能创建交叉存取图像.
 
      cvZero(hist_image);//这个宏使得背景为黑色
    //统计直方图中的最大直方块
    float max_value = 0;
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);
 
    //分别将每个直方块的值绘制到图中
    for(int i=0;i<hist_size;i++)
    {
        float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率
        int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度
        cvRectangle(hist_image,//图像
                    cvPoint(i*scale,hist_height-1),//矩形的一个顶点
                    cvPoint((i+1)*scale - 1, hist_height - intensity),//矩形的对角线上顶点
                    Scalar(0,255,255) ); //线条颜色 (RGB) 或亮度（灰度图像 ）(grayscale image）CV_RGB(0,255,255)
    }
    //创建方块图实例
    cvRectangle(hist_image,            //图像
                cvPoint(20,200),       //矩形的一个顶点
                cvPoint(200,300),      //矩形的对角线上顶点
                Scalar(0,255,255),     //线条颜色 (RGB) 或亮度（灰度图像 ）(grayscale image）
                1,                     //组成矩形的线条的粗细程度。取负值时（如 CV_FILLED）函数绘制填充了色彩的矩形。
                1,                     //线条的类型。见cvLine的描述
                0);                    //坐标点的小数点位数。
 
    Mat outMat=cvarrToMat(hist_image);//IplImage转Mat
    // 显示直方图
    namedWindow("Gray Histogram", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("Gray Histogram", outMat);//窗口中显示直方图
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("original image", globalMat );//窗口中显示原图
    waitKey(0);
}
 
//白底直方图带灰度阶的
void GrayHistogram2(Mat intMat)
{
    //cvQueryHistValue_1D这个宏必须加
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
 
    IplImage *src= &IplImage(intMat);//加载图片转换成IplImage
    IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);//彩色转灰度
 
    int hist_size = 256;    //直方图尺寸
    int hist_height = 256; //直方图尺寸
    float range[] = {0,255};  //灰度级的范围
    float* ranges[]={range};//创建一个数组
 
    //创建一维直方图，统计图像在[0 255]像素的均匀分布
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    cvCalcHist(&gray_plane,gray_hist,0,0);//计算灰度图像的一维直方图
 
    IplImage *pHistImage = cvCreateImage(cvSize(hist_size * 2, hist_height), IPL_DEPTH_8U, 1);
    cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height), CV_RGB(255, 255, 255), CV_FILLED);
    float fMaxHistValue = 0;//统计直方图中的最大直方块
    cvGetMinMaxHistValue(gray_hist, NULL, &fMaxHistValue, NULL, NULL);//统计直方图中的最大直方块
    //分别将每个直方块的值绘制到图中
    for( int i = 0; i < 256; i++)
    {
        float fHistValue = cvQueryHistValue_1D(gray_hist, i); //像素为i的直方块大小
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) *256);  //要绘制的高度
        cvRectangle(pHistImage,
                    cvPoint(i * 2, 256 - 1),
                    cvPoint((i + 1) * 2 - 1, 256 - nRealHeight),
                    cvScalar(i, 0, 0, 0),
                    CV_FILLED);
    }
    Mat outMat=cvarrToMat(pHistImage);//IplImage转Mat
    // 显示直方图
    namedWindow("Gray Histogram2", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("Gray Histogram2", outMat);//窗口中显示直方图
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("original image", globalMat );//窗口中显示原图
    waitKey(0);
}
 
//黑底直方图
void EqualizeHistogram(Mat intMat)
{
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
 
    IplImage *src= &IplImage(intMat);
    IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
    IplImage* EqualizeImg = cvCreateImage(cvGetSize(src),8,1);
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);
    cvEqualizeHist(gray_plane, EqualizeImg); // 均衡化
 
    int hist_size = 256;    //直方图尺寸
    int hist_height = 256;
    float range[] = {0,255};  //灰度级的范围
    float* ranges[]={range};
    //创建一维直方图，统计图像在[0 255]像素的均匀分布
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);
    cvCalcHist(&EqualizeImg,gray_hist,0,0);//计算灰度图像的一维直方图
    cvNormalizeHist(gray_hist,1.0);//归一化直方图
 
    int scale = 2;
    //创建一张一维直方图的“图，横坐标为灰度级，纵坐标为像素个数（*scale）
    IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);
    cvZero(hist_image);
    //统计直方图中的最大直方块
    float max_value = 0;
    cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);
    //分别将每个直方块的值绘制到图中
    for(int i=0;i<hist_size;i++)
    {
        float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率
        int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度
        cvRectangle(hist_image,
                    cvPoint(i*scale,hist_height-1),
                    cvPoint((i+1)*scale - 1, hist_height - intensity),
                    CV_RGB(255,255,255));
    }
    Mat outMat=cvarrToMat(hist_image);//IplImage转Mat
    Mat outMat2=cvarrToMat(EqualizeImg);//IplImage转Mat
    // 显示直方图
    namedWindow("Gray Histogram", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("Gray Histogram", outMat);//窗口中显示直方图
    namedWindow("Equalize", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("Equalize",outMat2);//窗口中显示直方图
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("original image", globalMat );//窗口中显示原图
    waitKey(0);
}
 
//白底直方图
void EqualizeHistogram2(Mat intMat)
{
    //cvQueryHistValue_1D这个宏必须加
#define cvQueryHistValue_1D( hist, idx0 )\
    ((float)cvGetReal1D( (hist)->bins, (idx0)))
 
    IplImage *src= &IplImage(intMat);
    IplImage* gray_plane = cvCreateImage(cvGetSize(src),8,1);
    IplImage* EqualizeImg = cvCreateImage(cvGetSize(src),8,1);
    cvCvtColor(src,gray_plane,CV_RGB2GRAY);//彩色转灰度
    cvEqualizeHist(gray_plane, EqualizeImg); // 均衡化
 
    int hist_size = 256;    //直方图尺寸
    int hist_height = 256; //直方图尺寸
    float range[] = {0,255};  //灰度级的范围
    float* ranges[]={range};
    CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);//创建一维直方图，统计图像在[0 255]像素的均匀分布
    cvCalcHist(&EqualizeImg,gray_hist,0,0);//计算灰度图像的一维直方图
    cvNormalizeHist(gray_hist,1.0);//归一化直方图
    IplImage *pHistImage = cvCreateImage(cvSize(hist_size * 2, hist_height), IPL_DEPTH_8U, 1);
    cvRectangle(pHistImage, cvPoint(0, 0), cvPoint(pHistImage->width, pHistImage->height), CV_RGB(255, 255, 255), CV_FILLED);
    float fMaxHistValue = 0;//统计直方图中的最大直方块
    cvGetMinMaxHistValue(gray_hist, NULL, &fMaxHistValue, NULL, NULL);//统计直方图中的最大直方块
    //分别将每个直方块的值绘制到图中
    for(int i = 0; i < 256; i++)
    {
        float fHistValue = cvQueryHistValue_1D(gray_hist, i); //像素为i的直方块大小
        int nRealHeight = cvRound((fHistValue / fMaxHistValue) *256);  //要绘制的高度
        cvRectangle(pHistImage,
                    cvPoint(i * 2, 256 - 1),
                    cvPoint((i + 1) * 2 - 1, 256 - nRealHeight),
                    cvScalar(i, 0, 0, 0),
                    CV_FILLED);
    }
    Mat outMat=cvarrToMat(pHistImage);//IplImage转Mat
    Mat outMat2=cvarrToMat(EqualizeImg);//IplImage转Mat
    // 显示直方图
    namedWindow("Equalize Histogram", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("Equalize Histogram", outMat);//窗口中显示直方图
    namedWindow("Equalize", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("Equalize",outMat2);//窗口中显示直方图
    namedWindow("original image", CV_WINDOW_AUTOSIZE );//创建窗口
    imshow("original image", globalMat );//窗口中显示原图
    waitKey(0);
}