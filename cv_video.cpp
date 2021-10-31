#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "iostream"

using namespace cv;
using namespace std;
int main()
{
    VideoCapture cap;
    cap.open("./EP03.mp4");
    if(!cap.isOpened()){
        cout << "ͼƬδ�򿪣�";
        return 0;
    }
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int totalframes = cap.get(cv::CAP_PROP_FRAME_COUNT);
    int frameRate = cap.get(cv::CAP_PROP_FPS);
    cout << "��Ƶ���" << width << endl;
    cout << "��Ƶ�߶�" << height << endl;
    cout << "��Ƶ��֡��" << totalframes << endl;
    cout << "֡�ʣ�" << frameRate << endl;
    Mat frame;
    while(1){
        cap >> frame;
        if (frame.empty())
            break;
        imshow("Doggi",frame);
        waitKey(50);
    }
    cap.release();
    return 0;
}