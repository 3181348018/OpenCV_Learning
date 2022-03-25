#include<opencv2/opencv.hpp>
using namespace cv;
int main()
{
    Mat imageSource=imread("C:/Users/31813/Desktop/fire.jpg");
	Mat image_gray,binary;
	Mat image;
    // rotate(imageSource,image_gray,0);
	// cvtColor(imageSource,image_gray,COLOR_BGR2GRAY);
    pyrMeanShiftFiltering(imageSource, image_gray, 21, 21);
    // blur(imageSource,image_gray,Size(9,9));
    double scale = 1;
    Size dsize = Size(image_gray.cols*scale,image_gray.rows*scale);
    Mat result = Mat(dsize,CV_32S);
    resize(image_gray,result,dsize);
    imshow("result",result);
    imwrite("C:/Users/31813/Desktop/img3.png",result);
    waitKey(0);
	return 0;
}