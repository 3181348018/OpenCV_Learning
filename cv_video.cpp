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
        cout << "图片未打开！";
        return 0;
    }
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    int totalframes = cap.get(cv::CAP_PROP_FRAME_COUNT);
    int frameRate = cap.get(cv::CAP_PROP_FPS);
    cout << "视频宽度" << width << endl;
    cout << "视频高度" << height << endl;
    cout << "视频总帧率" << totalframes << endl;
    cout << "帧率：" << frameRate << endl;
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